#ifndef __SUPERHUDEDITOR_H__
#define __SUPERHUDEDITOR_H__

#include <wx/app.h>

const wxString APP_NAME = wxT("superhudeditor");
const wxString APP_VENDOR = wxEmptyString;
const wxString APP_CAPTION = APP_NAME;
const wxString APP_CONFIG = wxT("superhudeditor.conf");

enum
{
	ID_BTN_Q4MAX = wxID_HIGHEST+1,
	ID_BTN_CPMA,
  IDM_GAMESELECTION,
  IDM_PREFERENCES
};

class FactoryBase;
class HudFileBase;

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


    FactoryBase *factory() { return m_factory; }
    HudFileBase *hudfile() { return m_hudfile; }

  protected:
    void load_config();

    bool m_firststart;
    FactoryBase *m_factory;
    HudFileBase *m_hudfile;
};

DECLARE_APP(SHEApp)


#endif // __SUPERHUDEDITOR_H__

