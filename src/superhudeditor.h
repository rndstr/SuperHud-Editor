#ifndef __SUPERHUDEDITOR_H__
#define __SUPERHUDEDITOR_H__

#include <wx/app.h>

const wxString APP_NAME = wxT("superhudeditor");
const wxString APP_VERSION = wxT("0.3.0");
const wxString APP_VENDOR = wxEmptyString;
const wxString APP_CAPTION = wxT("SuperHud Editor v") + APP_VERSION;
const wxString APP_CONFIG = wxT("superhudeditor.conf");
const wxString APP_URL = wxT("http://plrf.org/superhudeditor");



class FactoryBase;
class HudFileBase;
class ElementsCtrlBase;
class MainFrame;
class PakManagerBase;

class SHEApp : public wxApp
{

  public:
    SHEApp();

    bool OnInit();
    int OnRun();
    int OnExit();

    void store_config();
    bool is_cpma() const;
    bool is_q4max() const;


    MainFrame*    mainframe() { return m_mainframe; }
    FactoryBase* factory() { return m_factory; }
    HudFileBase* hudfile() { return m_hudfile; }
    ElementsCtrlBase* elementsctrl();
    PakManagerBase* pakmanager() { return m_pakmanager; }

  protected:
    void load_config();

    bool m_firststart;

    MainFrame   *m_mainframe;
    FactoryBase *m_factory;
    HudFileBase *m_hudfile;
    PakManagerBase *m_pakmanager;
};

DECLARE_APP(SHEApp)


#endif // __SUPERHUDEDITOR_H__

