#ifndef __SUPERHUDEDITOR_H__
#define __SUPERHUDEDITOR_H__

#include <wx/app.h>

const wxString APP_NAME = wxT("superhudeditor");
const wxString APP_VERSION = wxT("0.3.0");
const wxString APP_VENDOR = wxEmptyString;
const wxString APP_CAPTION = APP_NAME + wxT(" v") + APP_VERSION;
const wxString APP_CONFIG = wxT("superhudeditor.conf");
const wxString APP_URL = wxT("http://plrf.org/superhudeditor");
const wxString BUG_MSG = wxT("Bug detected, please contact the author");

enum
{
	ID_BTN_Q4MAX = wxID_HIGHEST+1,
	ID_BTN_CPMA,

  ID_BTN_INSERTDEFAULT,
  ID_BTN_INSERTPREDECORATE,
  ID_BTN_INSERTPOSTDECORATE,
  ID_BTN_COPY,
  ID_BTN_PASTE,

  ID_MENU_TOOLS_GAMESELECTION,
  ID_MENU_TOOLS_PREFERENCES,

  ID_MENU_VIEW_DEFAULTPERSPECTIVE,
  ID_MENU_VIEW_CONFIGPREVIEW,


  ID_LISTCTRL_ELEMENTS,
  ID_TEXTCTRL_CONFIGPREVIEW
};

class FactoryBase;
class HudFileBase;
class ElementsCtrlBase;
class MainFrame;

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

  protected:
    void load_config();

    bool m_firststart;

    MainFrame   *m_mainframe;
    FactoryBase *m_factory;
    HudFileBase *m_hudfile;
};

DECLARE_APP(SHEApp)


#endif // __SUPERHUDEDITOR_H__

