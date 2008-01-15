#ifndef PREFS_H
#define PREFS_H

#include "common.h"

#include <wx/string.h>
#include "color4.h"

#include <map>


enum
{
  PVT_ANY, ///< valid as well but we don't do checks upon int/float/boolval() and there is no conversion stored
  PVT_DOUBLE,
  PVT_INT,
  PVT_BOOL,
  PVT_COLOR,
  PVT_STRING
};

enum
{
  PVF_NONE = 0, ///< default flags
  // per default we only write the var if it has been set / previously found in config
  PVF_NOARCHIVE = 1, ///< never write to config
  PVF_ARCHIVEALWAYS = 2 ///< always write to config
};



// ugliest :x *careface*
class Variable
{
  public:
    Variable( const wxString& name, const wxString& def = wxT(""), int type = PVT_ANY, int flags = PVF_NONE );

    int ival() const
    { 
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_INT, m_name );
      return intval;
    }
    double dval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_DOUBLE, m_name );
      return doubleval;
    }
    bool bval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_BOOL, m_name );
      return boolval;
    }
    Color4 cval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_COLOR, m_name );
      return colorval;
    }
    wxColour wxcval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_COLOR, m_name );
      return colorval.to_wxColour();
    }

    wxString sval() const
    {
      return (m_isset ? m_value : m_def);
    }

    void read();
    void write();
    void set( const wxString& str, bool isset = true );

    operator bool() const { return bval(); }
    operator Color4() const { return cval(); }
    operator double() const { return dval(); }
    operator int() const { return ival(); }
    operator wxString() const { return sval(); }


  protected:
    wxString  m_name;
    wxString  m_value;
    wxString  m_def;
    int       m_type;
    int       m_flags;
    bool      m_isset;

  private:
    bool      m_uptodate;
    // store conversions
    // FIXME this is waste of space, union?
    double    doubleval;
    int       intval;
    bool      boolval;
    Color4    colorval;
};

typedef std::map<wxString, Variable> variables_type;



class Prefs
{
    
  public:
    /// initializes the config object
    /// @returns bool True if the configfile didn't yet exist aka "first start" otherwise false
    bool init();
    void shutdown();

    void load();
    /// stores all preferences in config file
    /// @arg from_prefs_dialog Whether we are coming from the preferences dialog (and thus only saving change that can be changed there)
    void save( bool from_prefs_dialog = false );

    //void set_aspectratio( const wxString& ar );
    //double get_aspectratio() const { return aspectratiod; }

    const Variable& var( const wxString& name ) const;
    void set( const wxString& name, const wxString& val );
    void setb( const wxString& name, bool bval );
    void seti( const wxString& name, int ival );


    /// returns the initial path to be used in a filedialog for the current game
    wxString filedialog_path() const;

  protected:
    variables_type  m_vars;

    void addvar( const wxString& name, const wxString& def = wxT(""), int type = PVT_ANY, int flags = PVF_NONE );


  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
};

#endif // PREFS_H
