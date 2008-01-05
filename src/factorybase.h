#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <wx/string.h>

class ElementsCtrlBase;
class PropertiesNotebookBase;
class DisplayCtrlBase;
class HudFileBase;
class wxWindow;

class FactoryBase
{
  public:

    /// this can be used to initialize game specific stuff
    virtual bool init() { return true; }
    /// this can be used to shut down game specific stuff
    virtual void shutdown() {  }

    // view panels
    virtual ElementsCtrlBase*   create_elementsctrl( wxWindow *parent ) const = 0;
    virtual PropertiesNotebookBase* create_propertiesnotebook( wxWindow *parent ) const = 0;
    virtual DisplayCtrlBase*    create_displayctrl( wxWindow *parent ) const = 0;
    virtual HudFileBase*        create_hudfile() const = 0;

    // game specific options
    virtual wxString gamename() const = 0;
    virtual wxString modname() const = 0;

    /// e.g. 'baseq3'
    virtual wxString dirname_gamedata() const = 0;
    /// e.g. 'cpma'
    virtual wxString dirname_moddata() const = 0;
    /// e.g. 'c:\\games\quake3'
    virtual wxString dir_game() const = 0;
    virtual void set_dir_game( const wxString& dir ) const = 0;
    /// semicolon separated, relative to gamedir, wildcards (?*) allowed
    virtual wxString pakfiles() const = 0;

    /// e.g. 'quake3.exe' or 'quake3.x86'
    /// is used to determine if we have a valid game directory
    virtual wxString filename_gamebin() const = 0;

    /// e.g. '.q3a' only used on unix systems
#ifndef WIN32
    virtual wxString unixdirname_userdata() const = 0;
#endif

};

#endif // FACTORYBASE_H

