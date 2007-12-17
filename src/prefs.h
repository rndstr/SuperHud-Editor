#ifndef __PREFS_H__
#define __PREFS_H__

#include <wx/string.h>

const wxString PREFS_Q3_PAKFILES_DEFAULT = wxT("baseq3/pak*.pk3;baseq3/map_cpm*.pk3;cpma/z-cpma-pak*.pk3");

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

  // settings
  public:

    // -- display
    wxString perspective;
    /// possible values 'q4max' or 'cpma'
    wxString game;

    // -- game specific
    // cpma
    wxString q3_gamedir; ///< "C:\games\quake3"
    wxString q3_pakfiles;
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

  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
};

#endif // __PREFS_H__
