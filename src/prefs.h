#ifndef __PREFS_H__
#define __PREFS_H__

#include <wx/string.h>
#include "color4.h"

#include <map>


enum
{
  PVT_ANY, ///< valid as well but we don't do checks upon int/float/boolval() and there is no conversion stored
  PVT_FLOAT,
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

    int intval() const
    { 
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_INT, m_name );
      return ival;
    }
    float floatval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_FLOAT, m_name );
      return fval;
    }
    bool boolval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_BOOL, m_name );
      return bval;
    }
    Color4 colorval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_COLOR, m_name );
      return cval;
    }

    wxString stringval() const
    {
      wxASSERT_MSG( m_type == PVT_ANY || m_type == PVT_STRING, m_name );
      return (m_isset ? m_value : m_def);
    }

    void read();
    void write();
    void set( const wxString& str, bool isset = true );

    operator bool() const { return boolval(); }
    operator Color4() const { return colorval(); }
    operator float() const { return floatval(); }
    operator int() const { return intval(); }
    operator wxString() const { return stringval(); }


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
    float     fval;
    int       ival;
    bool      bval;
    Color4    cval;
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

  // settings
  // NOARCHIVE = we don't write it.. only read
  public:

    /*
    // -- display
    wxString  perspective;
    bool      app_maximized;
    int       app_width, app_height;
    
    bool      grid;
    int       grid_x, grid_y;
    Color4    grid_color;
    bool      helper;
    Color4    helper_border;
    Color4    helper_fill;
    Color4    helper_border_selected;
    Color4    helper_fill_selected;
    bool      elements_collections;
    

    /// possible values 'q4max' or 'cpma'
    wxString game;

    // -- game specific
    // cpma
    wxString q3_gamedir; ///< "C:\games\quake3"
#ifndef WIN32
    wxString q3_homedirname; ///< NOARCHIVE ".q3a"
#endif
    wxString q3_pakfiles;
    wxString q3_background;
    // q4max
    wxString q4_gamedir; ///< "C:\games\quake4"

    // -- mis
    wxString q3_hudspecs; ///< NOARCHIVE

    // -- startup
    bool startup_gameselection; // show game selection on startup always (even if there is already a `game' set

    // -- saving
    bool save_writedisabled; ///< whether we write the disabled elements (commented out ofc)
    bool save_backup; ///< whether to create backup files upon saving
    

  private:
    wxString  aspectratio; ///< human readable '4:3', '16:9'. this will be read/written
    double    aspectratiod; ///< use this for calculations, will be updated after read
    */

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

#endif // __PREFS_H__
