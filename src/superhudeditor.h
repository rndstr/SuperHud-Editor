#ifndef SUPERHUDEDITOR_H
#define SUPERHUDEDITOR_H

#include "common.h"

#include <wx/app.h>
#include <wx/intl.h>
#include <wx/cmdline.h>


const wxString APP_NAME = wxT("SuperHud Editor");
const wxString APP_NAME_UNIX = wxT("superhudeditor");
const wxString APP_VERSION = wxT("0.4.0");
const int APP_MAJOR_VERSION = 0;
const int APP_MINOR_VERSION = 4;
const int APP_RELEASE_NUMBER = 0;
const wxString APP_RELEASE_TYPE = wxT("");
const wxString APP_VENDOR = wxEmptyString;
#ifndef NDEBUG
  const wxString APP_CAPTION = APP_NAME + wxT(" v") + APP_VERSION + wxT(" DEBUG BUILD");
#else
  const wxString APP_CAPTION = APP_NAME + wxT(" v") + APP_VERSION;
#endif
const wxString APP_CONFIG = wxT("superhudeditor.conf");
const wxString APP_URL = wxT("http://plrf.org/superhudeditor");
const wxString APP_URL_VERSIONCHECK = APP_URL + wxT("/versioncheck.php");
const wxString APP_BUILDTIME = wxString(wxT(__DATE__)) + wxT(" ") + wxT(__TIME__);

class wxCommandProcessor;
class FactoryBase;
class HudFileBase;
class ElementsCtrlBase;
class MainFrame;

class SHEApp : public wxApp
{

  public:
    SHEApp();

    virtual bool OnInit();
    virtual int OnRun();
    virtual int OnExit();
    virtual void OnInitCmdLine( wxCmdLineParser& parser );
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    void store_config();
    bool is_cpma() const;
    bool is_q4max() const;

    bool is_firststart() const { return m_firststart; }


    MainFrame*    mainframe() { return m_mainframe; }
    FactoryBase* factory() { return m_factory; }
    HudFileBase* hudfile() { return m_hudfile; }
    ElementsCtrlBase* elementsctrl();

    bool is_ready() const { return m_ready; }

    void set_exec( const wxString& exec ) { m_exec = exec; }
    wxString exec() const { return m_exec; }

#if HAS_CMDPROC
    wxCommandProcessor* cmds();
#endif

  protected:
    void load_config();

    bool        m_firststart;
    MainFrame   *m_mainframe;
    FactoryBase *m_factory;
    HudFileBase *m_hudfile;
    bool        m_ready;
    wxString    m_exec; ///< what command to execute after shutdown
    wxLocale    m_locale;
#if HAS_CMDPROC
    wxCommandProcessor *m_commands;
#endif
    wxString    m_cmdline_file;
};

static const wxCmdLineEntryDesc g_cmdLineDesc[] = 
{
  { wxCMD_LINE_PARAM, 0, 0, _("HUD file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_NONE }
};

DECLARE_APP(SHEApp)


#endif

