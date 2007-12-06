#ifndef __PREFS_H__
#define __PREFS_H__

#include <wx/string.h>

class Prefs
{
  public:
    /// initializes the config object
    /// @returns bool True if the configfile didn't yet exist aka "first start" otherwise false
    bool init();
    void shutdown();

    void load();
    /// stores all preferences in config file
    /// @arg from_prefs_dialog Whether we are coming from the preferences dialog
    void save( bool from_prefs_dialog = false );

  // settings
  public:

    // -- display
    wxString perspective;
    wxString game;

    // -- mis
    wxString hudspecs; ///< NOARCHIVE

    // -- sepcific games

    // -- startup/saving
    bool startup_gameselection; // show game selection on startup always (even if there is already a `game' set

  // singleton
  public:
    static Prefs& get();
  protected:
    Prefs() {}
    Prefs(const Prefs&);
    Prefs& operator= (const Prefs&);
};

#endif // __PREFS_H__
