#ifndef __PREFS_H__
#define __PREFS_H__

#include <wx/string.h>
#include "color4.h"

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

    void set_aspectratio( const wxString& ar );
    double get_aspectratio() const { return aspectratiod; }

  // settings
  // NOARCHIVE = we don't write it.. only read
  public:

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
    wxString hudspecs; ///< NOARCHIVE

    // -- sepcific games

    // -- startup
    bool startup_gameselection; // show game selection on startup always (even if there is already a `game' set

    // -- saving
    bool save_writedisabled; ///< whether we write the disabled elements (commented out ofc)
    bool save_backup; ///< whether to create backup files upon saving

  private:
    wxString  aspectratio; ///< human readable '4:3', '16:9'. this will be read/written
    double    aspectratiod; ///< use this for calculations, will be updated after read

  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
};

#endif // __PREFS_H__
