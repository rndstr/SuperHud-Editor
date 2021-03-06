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
  VART_VEC2,
  VART_VEC4
};

enum
{
  VARF_NONE = 0, ///< default flags
  // per default we only write the var if it has been set / previously found in config
  VARF_NOARCHIVE = 1, ///< never write to config
  VARF_ARCHIVEALWAYS = 2 ///< always write to config
};


struct Vec2 
{
  int x, y;
  bool from_string( const wxString& str )
  {
    float fx, fy;
    if( 2 != sscanf(str.mb_str(), "%f %f", &fx, &fy) )
      return false;
    x = (int)fx;
    y = (int)fy;
    return true;
  }
};


struct Vec4
{
  int x, y, w, h;
  bool from_string( const wxString& str )
  {
    float fx, fy, fw, fh;
    if( 4 != sscanf(str.mb_str(), "%f %f %f %f", &fx, &fy, &fw, &fh) )
      return false;
    x = (int)fx;
    y = (int)fy;
    w = (int)fw;
    h = (int)fh;
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

    Vec2 v2val() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_VEC2, m_name );
      return m_specific.vec2val;
    }

    Vec4 v4val() const
    {
      wxASSERT_MSG( m_type == VART_ANY || m_type == VART_VEC4, m_name );
      return m_specific.vec4val;
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
      else if( m_type == VART_VEC2 )
      {
        return m_specific.vec2val.from_string(m_value);
      }
      else if( m_type == VART_VEC4 )
      {
        return m_specific.vec4val.from_string(m_value);
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
      Vec2      vec2val;
      Vec4      vec4val;
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
    for( it_variables it = m_vars.begin(); it != m_vars.end(); ++it )
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
    cit_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable: `") + name + wxT("'"));
    return var->second;
  }

  bool set( const wxString& name, const wxString& val )
  {
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set(val);
  }
  bool setb( const wxString& name, bool bval )
  {
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( bval ? wxT("1") : wxT("0"));
  }
  bool seti( const wxString& name, int ival )
  {
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( wxString::Format(wxT("%i"), ival) );
  }
  bool setvar( const wxString& name, const wxVariant& variant )
  {
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set( variant.MakeString() );
  }
  bool setwxc( const wxString& name, const wxColour& wxcol, int alpha = -1 )
  {
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    if( var->second.cval().set(wxcol) || alpha != -1 )
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
    it_variables var = m_vars.find(name.Lower());
    wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
    return var->second.set_default();
  }
  var_type& addvar( const wxString& name, const wxString& def = wxT(""), int type = VART_ANY, int flags = VARF_NONE )
  {
     it_variables it = m_vars.insert( m_vars.begin(), std::make_pair(name.Lower(), var_type(name, def, type, flags)) );
     return it->second;
  }
  var_type& addvard( const wxString& name, double def, int flags = VARF_NONE )
  {
    return addvar( name, wxString::Format(wxT("%.3f"), def), VART_DOUBLE, flags);
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
  var_type& addvarv2( const wxString& name, int xdef, int ydef, int flags = VARF_NONE )
  {
    return addvar( name, wxString::Format(wxT("%d %d"), xdef, ydef), VART_VEC2, flags);
  }

  const variables_type& vars() const { return m_vars; }

  bool exists( const wxString& name, int type = VART_ANY )
  {
    it_variables var = m_vars.find(name.Lower());
    return (var != m_vars.end() && (VART_ANY == type || var->second.m_type == type));
  }

#ifndef NDEBUG
  void debug()
  {
    wxLogDebug(wxT("VarContainer<>::debug - all vars registered"));
    for( cit_variables it = m_vars.begin(); it != m_vars.end(); ++it )
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

