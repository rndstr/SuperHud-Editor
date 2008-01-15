#ifndef __CPMA_FACTORY_H__
#define __CPMA_FACTORY_H__

#include "../factorybase.h"
#include "../prefs.h"

class CPMAFactory : public FactoryBase
{
  public:
    bool                init();
    ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const;
    PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const;
    DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const;
    HudFileBase*        create_hudfile() const;

    wxString            gamename() const { return wxT("Quake3"); }
    wxString            modname() const { return wxT("CPMA"); }
    wxString            dirname_gamedata() const { return wxT("baseq3"); }
    wxString            dirname_moddata() const { return wxT("cpma"); }
    wxString            dir_game() const { return Prefs::get().var(wxT("q3_gamedir")); }
    void                set_dir_game( const wxString& dir ) const { Prefs::get().set(wxT("q3_gamedir"), dir); }
    wxString            pakfiles() const { return Prefs::get().var(wxT("q3_pakfiles")); }
    wxString            filedialog_path() const { return Prefs::get().var(wxT("q3_filedialog_path")); }
    void                set_filedialog_path( const wxString& path ) { Prefs::get().set(wxT("q3_filedialog_path"), path); }
#ifndef WIN32
    wxString            unixdirname_userdata() const { return Prefs::get().var(wxT("q3_homedirname")); }
#endif

#ifdef WIN32
    wxString            filename_gamebin() const { return wxT("quake3.exe"); }
#else // unix
    wxString            filename_gamebin() const { return wxT("quake3.x86"); }
#endif

};

#endif // __CPMA_FACTORY_H__
