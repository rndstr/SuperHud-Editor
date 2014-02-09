ifndef CPMAFACTORY_H
#define CPMAFACTORY_H

#include "factorybase.h"
#include "prefs.h"

class CPMAFactory : public FactoryBase
{
  public:
    ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const;
    PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const;
    DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const;
    HudFileBase*        create_hudfile() const;

    wxString            gamename() const { return wxT("Quake3"); }
    wxString            modname() const { return wxT("CPMA"); }
    wxString            dirname_gamedata() const { return wxT("baseq3"); }
    wxString            dirname_moddata() const { return wxT("cpma"); }

    // -- access prefs
    wxString            dir_game() const { return Prefs::get().var(wxT("q3_gamedir")).sval(); }
    void                set_dir_game( const wxString& dir ) const { Prefs::get().set(wxT("q3_gamedir"), dir); }
    wxString            pakfiles() const { return Prefs::get().var(wxT("q3_pakfiles")).sval(); }
    wxString            filedialog_path() const { return Prefs::get().var(wxT("q3_filedialog_path")).sval(); }
    void                set_filedialog_path( const wxString& path ) { Prefs::get().set(wxT("q3_filedialog_path"), path); }
    wxString            startup_loadfile() const { return Prefs::get().var(wxT("q3_startup_loadfile")).sval(); }
    void                set_startup_loadfile( const wxString& file ) { Prefs::get().set(wxT("q3_startup_loadfile"), file); }
    wxString            background() const { return Prefs::get().var(wxT("q3_background")).sval(); }
    wxString            hudspecs() const { return Prefs::get().var(wxT("q3_hudspecs")).sval(); }
#ifndef WIN32
    wxString            unixdirname_userdata() const { return Prefs::get().var(wxT("q3_homedirname")).sval(); }
#endif

#ifdef WIN32
    wxString            filename_gamebin() const { return wxT("quake3.exe"); }
#else // unix
    wxString            filename_gamebin() const { return wxT("quake3.x86"); }
#endif

    virtual wxString wikiurl() const { return wxT("http://www.promode.org/wiki/index.php/Custom_HUDs"); }

};

#endif
