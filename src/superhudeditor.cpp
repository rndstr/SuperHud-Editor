// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "superhudeditor.h"
#include "mainframe.h"
#include "common.h"
#include "elementsctrlbase.h"
#include "displayctrlbase.h"
#include "setupwizard.h"

#include <wx/stdpaths.h>
#include <wx/fs_arc.h>
#include <wx/cmdproc.h>

#include "factorybase.h"
#include "hudfilebase.h"
#include "pakmanager.h"
#include "GameSelectionDialog.h"

#ifndef DISABLE_CPMA
  #include "cpma/factory.h"
  #include "cpma/hudspecs.h"
  #include "cpma/hudfile.h"
#endif


IMPLEMENT_APP(SHEApp)

SHEApp::SHEApp() :
  m_firststart(false),
  m_mainframe(0),
  m_factory(0),
  m_hudfile(0),
  m_ready(false)
{

}
// FIXME remove this Oo
ElementsCtrlBase* SHEApp::elementsctrl() { return m_mainframe->elementsctrl(); }

bool SHEApp::OnInit()
{
  wxLogDebug(wxT("SHEApp::OnInit"));
  SetAppName(APP_NAME_UNIX);
#ifndef WIN32
  #ifdef PACKAGE_PREFIX
    ((wxStandardPaths&)wxStandardPaths::Get()).SetInstallPrefix( wxT( PACKAGE_PREFIX ) );
  #endif
#endif

  wxArtProvider::Push( new ArtProvider );
  wxFileSystem::AddHandler(new wxArchiveFSHandler);
  wxInitAllImageHandlers();

  // set up config file
  m_firststart = Prefs::get().init();

#ifndef NDEBUG
  // also log to file in debug mode
  wxString logfilename = wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_NAME_UNIX + wxT(".log");
  wxLogDebug(wxT("Logging to file: ") + logfilename );
  FILE *fp = fopen(logfilename.mb_str(), "w");
  if( fp )
    wxLogChain *ruelps = new wxLogChain(new wxLogStderr(fp));
  else
    wxLogWarning(wxT("Cannot open logfile:\n\n") + logfilename);
#endif

  /*
  wxLocale::AddCatalogLookupPathPrefix(wxT("./locale/"));
  wxLocale::AddCatalogLookupPathPrefix(wxT("../locale/"));

  if( !m_locale.Init(wxLANGUAGE_GERMAN) )
    wxLogError(wxT("Couldn't initialize language"));
    */

#if HAS_MULTIPLE_GAMES
  wxLogDebug(wxT("Multiple Games"));
  if( (!is_cpma() && !is_q4max()) || Prefs::get().var(wxT("startup_gameselection")) )
  {
    GameSelectionDialog dlg(0);
    int ret = dlg.ShowModal();
    if( ID_BTN_CPMA == ret )
      Prefs::get().set(wxT("game"), wxT("cpma"));
    else if( ID_BTN_Q4MAX == ret )
      Prefs::get().set(wxT("game"), wxT("q4max"));
    Prefs::get().setb(wxT("startup_gameselection"), dlg.startup_gameselection());

    if( !is_cpma() && !is_q4max() )
    {
      wxLogFatalError(_("Invalid game in settings, loaded CPMA"));
      Prefs::get().set(wxT("game"), wxT("cpma"));
    }
  }
#elif ENABLE_CPMA
  wxLogDebug(wxT("Single Game: CPMA"));
#elif ENABLE_Q4MAX
  wxLogDebug(wxT("Single Game: Q4MAX"));
#elif
  wxLogDebug(wxT("ERROR - No game"));
#endif

  if( is_cpma() )
  {
    m_factory = new CPMAFactory;
  }
  else if( is_q4max() )
  {

  }

  if( !m_factory->init() )
  {
    m_factory->shutdown();
    wxDELETE(m_factory);
    return false;
  }


  if( wxGetApp().is_firststart() )
  {
    SetupWizard wizard(0);
    if( wizard.RunWizard(wizard.GetFirstPage()) )
    {
      wxGetApp().factory()->set_dir_game(wizard.gamedir());
      Prefs::get().set(wxT("view_aspectratio"), wizard.aspectratio());
      Prefs::get().save(); // just save to make sure.. if we crash at least this we don't have to go through again
    }
  }
  else if( Prefs::get().var(wxT("q3_gamedir")).sval().empty() )
  {
    SetupWizard wizard(0);
    if( wizard.RunWizard(wizard.GetFirstPage()) )
      wxGetApp().factory()->set_dir_game(wizard.gamedir());
  }

  PakManager::get().init();

  m_mainframe = new MainFrame(0, wxID_ANY, APP_NAME, wxDefaultPosition, wxSize(800,600));
  SetTopWindow(m_mainframe);
  m_mainframe->Show();

  m_hudfile = m_factory->create_hudfile();

  m_commands = new wxCommandProcessor();
  m_commands->SetEditMenu(m_mainframe->edit_menu());
  m_commands->Initialize();

  m_mainframe->elementsctrl()->OnSelectionChanged();

  return true;
}


int SHEApp::OnRun()
{
  wxLogDebug(wxT("SHEApp::OnRun"));
  m_mainframe->update_title();
  if( Prefs::get().var(wxT("startup_load")) && !Prefs::get().var(wxT("startup_loadfile")).sval().empty() )
    m_hudfile->OnOpen( Prefs::get().var(wxT("startup_loadfile")) );
  else
    m_hudfile->OnNew();
  m_mainframe->update_title();

  m_ready = true;

  if( Prefs::get().var(wxT("startup_tips")).bval() )
  {
    wxCommandEvent plaarfz;
    m_mainframe->OnMenuHelpTip(plaarfz);
  }

  return wxApp::OnRun();
}

int SHEApp::OnExit()
{
  m_ready = false;
  wxLogDebug(wxT("SHEApp::OnExit"));
  Prefs::get().save();
  Prefs::get().shutdown();

  m_commands->SetEditMenu(0);
  wxDELETE(m_commands);

  m_factory->shutdown();
  wxDELETE(m_factory);
  wxDELETE(m_hudfile);
  PakManager::get().shutdown();

  if( !m_exec.empty() )
  {
    
    wxExecute( m_exec, wxEXEC_ASYNC );
  }

  return 0;
}

wxCommandProcessor* SHEApp::cmds()
{ 
  return m_commands; 
}


bool SHEApp::is_cpma() const
{
#if HAS_MULTIPLE_GAMES
  return (Prefs::get().var(wxT("game")).sval().CmpNoCase(wxT("cpma")) == 0);
#elif ENABLE_CPMA
  return true; // cpma only available, so this is always cpma
#else
  return false; // other game only available
#endif

}

bool SHEApp::is_q4max() const
{
#if HAS_MULTIPLE_GAMES
  return (Prefs::get().var(wxT("game")).sval().CmpNoCase(wxT("q4max")) == 0);
#elif ENABLE_Q4MAX
  return true;
#else
  return false;
#endif
}
