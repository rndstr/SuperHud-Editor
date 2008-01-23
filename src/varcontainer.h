// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
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
  VART_STRING,
  VART_POINT,
  VART_RECT
};

enum
{
  VARF_NONE = 0, ///< default flags
  // per default we only write the var if it has been set / previously found in config
  VARF_NOARCHIVE = 1, ///< never write to config
  VARF_ARCHIVEALWAYS = 2 ///< always write to config
};


struct PointVal
{
  float x, y;
  bool from_string( const wxString& str )
  {
    if( 2 != sscanf(str.mb_str(), "%f %f", &x, &y) )
      return false;
    return true;
  }
};


struct RectVal
{
  float x, y, w, h;
  bool from_string( const wxString& str )
  {
    if( 4 != sscanf(str.mb_str(), "%f %f %f %f", &x, &y, &w, &h) )
      return false;
    return true;
  }
};

class Var
{
  public:
    Var( const wxString& name, const wxString& def, int type = VART_ANY, int flags = VARF_NONE );
    virtual ~Var() {}

    int ival() const
    { 
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_INT, m_name );
      return m_specific.intval;
    }

    double dval() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_DOUBLE, m_name );
      return m_specific.doubleval;
    }
    bool bval() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_BOOL, m_name );
      return m_specific.boolval;
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

    PointVal pval() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_POINT, m_name );
      return m_specific.pointval;
    }

    RectVal rval() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_RECT, m_name );
      return m_specific.rectval;
    }


    wxString sval() const
    {
      return (m_isset ? m_value : m_def);
    }

    bool set( const wxString& str, bool isset = true )
    {
      m_isset = isset;
      m_value = str;
      if( m_type == VART_INT )
      {
        long val;
        m_value.ToLong(&val);
        m_specific.intval = static_cast<int>(val);
      }
      else if( m_type == VART_DOUBLE )
      {
        if( !she::ratio_string2double(m_value, &m_specific.doubleval) )
        {
          wxLogError(_("Invalid value for ratio (`%s')"), m_value.c_str());
          return false;
        }
      }
      else if( m_type == VART_BOOL )
      {
        m_specific.boolval = (m_value == wxT("true") || m_value == wxT("yes") || m_value == wxT("1"));
      }
      else if( m_type == VART_COLOR )
      {
        return colorval.set(m_value);
      }
      else if( m_type == VART_POINT )
      {
        return m_specific.pointval.from_string(m_value);
      }
      else if( m_type == VART_RECT )
      {
        return m_specific.rectval.from_string(m_value);
      }
      return true;
    }
    bool set_default()
    {
      return set(m_def);
    }

    
    /*
    operator bool() const { return bval(); }
    operator Color4() const { return cval(); }
    operator double() const { return dval(); }
    operator int() const { return ival(); }
    operator wxString() const { return sval(); }
    */


    wxString def() const { return m_def; }
    wxString name() const { return m_name; }


    wxString  m_name; ///< note that this is not all lowercase
    wxString  m_value;
    wxString  m_def;
    int       m_type;
    int       m_flags;
    bool      m_isset;

  private:
    union value_u
    {
      double    doubleval;
      int       intval;
      bool      boolval;
      PointVal  pointval;
      RectVal   rectval;
    } m_specific;
    Color4    colorval;
};


template <typename VarC>
class VarContainer
{
public:
  typedef VarC var_type;
  typedef std::map<wxString, var_type>    variables_type;
  typedef typename variables_type::iterator        it_variables;
  typedef typename variables_type::const_iterator  cit_variables;

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
    for( typename variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    {
      // read element
      read_var(it->second);
    }
  }
  virtual void save()
  {
    for( typename variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
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
    typename variables_type::const_iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable: `") + name + wxT("'"));
    return var->second;
  }

  bool set( const wxString& name, const wxString& val )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set(val);
  }
  bool setb( const wxString& name, bool bval )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( bval ? wxT("1") : wxT("0"));
  }
  bool seti( const wxString& name, int ival )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( wxString::Format(wxT("%i"), ival) );
  }
  bool setvar( const wxString& name, const wxVariant& variant )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( variant.MakeString() );
  }
  bool setwxc( const wxString& name, const wxColour& wxcol, int alpha = -1 )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    if( var->second.cval().set(wxcol) )
    { // ok looks like the color changed.. so let's update
      Color4 tmp;
      tmp.set(wxcol);
      if( -1 != alpha )
        tmp.set_a100(alpha);
      return var->second.set( tmp.to_string() );
    }
    return false;
  }

  bool set_default( const wxString& name )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set_default();
  }
  var_type& addvar( const wxString& name, const wxString& def = wxT(""), int type = VART_ANY, int flags = VARF_NONE )
  {
     it_variables it = m_vars.insert( m_vars.begin(), std::make_pair(name.Lower(), var_type(name, def, type, flags)) );
     return it->second;
  }
  var_type& addvari( const wxString& name, int def, int flags = VARF_NONE )
  {
    return addvar( name, wxString::Format(wxT("%d"), def), VART_INT, flags);
  }
  var_type& addvarc( const wxString& name, const Color4& def, int flags = VARF_NONE )
  {
    return addvar( name, def.to_string(), VART_COLOR, flags );
  }
  var_type& addvarb( const wxString& name, bool def, int flags = VARF_NONE )
  {
    return addvar( name, (def ? wxT("1") : wxT("0")), VART_BOOL, flags );
  }
  var_type& addvarp( const wxString& name, int xdef, int ydef, int flags = VARF_NONE )
  {
    return addvar( name, wxString::Format(wxT("%d %d"), xdef, ydef), VART_POINT, flags);
  }

  const variables_type& vars() const { return m_vars; }

  bool exists( const wxString& name, int type = VART_ANY )
  {
    typename variables_type::iterator var = m_vars.find(name.Lower());
    return (var != m_vars.end() && (VART_ANY == type || var->second.m_type == type));
  }

#ifndef NDEBUG
  void debug()
  {
    wxLogDebug(wxT("VarContainer<>::debug - all vars registered"));
    for( typename variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    {
      wxLogDebug(wxT("[%s] %s = %s"), it->first.c_str(), it->second.name().c_str(), it->second.sval().c_str());
    }
  }
#endif

protected:
  variables_type  m_vars;
};

typedef VarContainer<Var> varcont_type;

#endif // PREFS_H

