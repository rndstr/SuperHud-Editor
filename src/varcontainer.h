// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef VARIABLE_H
#define VARIABLE_H

#include "common.h"

#include <wx/string.h>
#include "color4.h"

#include <map>


enum
{
  VART_ANY, ///< valid as well but we don't do checks upon int/float/boolval() and there is no conversion stored
  VART_DOUBLE,
  VART_INT,
  VART_BOOL,
  VART_COLOR,
  VART_STRING
};

enum
{
  VARF_NONE = 0, ///< default flags
  // per default we only write the var if it has been set / previously found in config
  VARF_NOARCHIVE = 1, ///< never write to config
  VARF_ARCHIVEALWAYS = 2 ///< always write to config
};



class Var
{
public:
  Var( const wxString& name, const wxString& def = wxT(""), int type = VART_ANY, int flags = VARF_NONE ) :
      m_name(name),
        m_value(wxT("")),
        m_def(def),
        m_type(type),
        m_flags(flags),
        m_isset(false)
      {
      }

      int ival() const
      { 
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_INT, m_name );
        return intval;
      }
      double dval() const
      {
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_DOUBLE, m_name );
        return doubleval;
      }
      bool bval() const
      {
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_BOOL, m_name );
        return boolval;
      }
      const Color4& cval() const
      {
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_COLOR, m_name );
        return colorval;
      }
      Color4& cval()
      {
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_COLOR, m_name );
        return colorval;
      }
      wxColour wxcval() const
      {
        wxASSERT_MSG( m_type == VART_ANY || m_type == VART_COLOR, m_name );
        return colorval.to_wxColour();
      }

      wxString sval() const
      {
        return (m_isset ? m_value : m_def);
      }

      void set( const wxString& str, bool isset = true )
      {
        m_isset = isset;
        m_value = str;
        if( m_type == VART_INT )
        {
          long val;
          m_value.ToLong(&val);
          intval = static_cast<int>(val);
        }
        else if( m_type == VART_DOUBLE )
        {
          if( !she::ratio_string2double(m_value, &doubleval) )
            wxLogError(wxT("Invalid value for aspectratio"));
        }
        else if( m_type == VART_BOOL )
        {
          boolval = (m_value == wxT("true") || m_value == wxT("yes") || m_value == wxT("1"));
        }
        else if( m_type == VART_COLOR )
        {
          colorval.set(m_value);
        }
      }
      void set_default()
      {
        set(m_def);
      }

      wxString def() const { return m_def; }

      operator bool() const { return bval(); }
      operator Color4() const { return cval(); }
      operator double() const { return dval(); }
      operator int() const { return ival(); }
      operator wxString() const { return sval(); }


  wxString  m_name;
  wxString  m_value;
  wxString  m_def;
  int       m_type;
  int       m_flags;
  bool      m_isset;

private:
  // FIXME this is waste of space, union?
  double    doubleval;
  int       intval;
  bool      boolval;
  Color4    colorval;
};


template <typename VarC>
class VarContainer
{
public:
  typedef VarC var_type;
  typedef std::map<wxString, var_type> variables_type;

public:
  VarContainer() {}
  virtual ~VarContainer() {}
  /// @returns bool True on success otherwise false
  virtual bool init() { return true; }
  virtual void cleanup()
  {
    m_vars.clear();
  }

  virtual void load()
  {
    for( variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    {
      // read element
      read_var(it->second);
    }
  }
  virtual void save()
  {
    for( variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    {
      write_var(it->second);
    }
  }

  virtual void read_var( var_type& var )
  {
    var.set_default();
  }

  virtual void write_var( const var_type& var ) const
  {

  }


  const var_type& var( const wxString& name ) const
  {
    variables_type::const_iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable: ") + name );
    return var->second;
  }

  void set( const wxString& name, const wxString& val )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    var->second.set(val);
  }
  void setb( const wxString& name, bool bval )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    var->second.set( bval ? wxT("true") : wxT("false"));
  }
  void seti( const wxString& name, int ival )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    var->second.set( wxString::Format(wxT("%i"), ival) );
  }
  void setvar( const wxString& name, const wxVariant& variant )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    var->second.set( variant.MakeString() );
  }
  void setwxc( const wxString& name, const wxColour& wxcol, int alpha = -1 )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    if( var->second.cval().set(wxcol) )
    { // ok looks like the color changed.. so let's update
      Color4 tmp;
      tmp.set(wxcol);
      if( -1 != alpha )
        tmp.set_a100(alpha);
      var->second.set( tmp.to_string() );
    }
  }
  void set_default( const wxString& name )
  {
    variables_type::iterator var = m_vars.find(name);
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    var->second.set_default();
  }
  void addvar( const wxString& name, const wxString& def = wxT(""), int type = VART_ANY, int flags = VARF_NONE )
  {
    m_vars.insert( std::make_pair(name, var_type(name, def, type, flags)) );
  }

  const variables_type& vars() const { return m_vars; }

  bool exists( const wxString& name, int type = VART_ANY )
  {
    variables_type::iterator var = m_vars.find(name);
    return (var != m_vars.end() && (VART_ANY == type || var->second.m_type == type));
  }

protected:
  variables_type  m_vars;
};

typedef VarContainer<Var> varcont_type;

#endif // PREFS_H

