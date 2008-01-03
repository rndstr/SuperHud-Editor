

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



#include <wx/stdpaths.h>
#include <wx/fs_arc.h>
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


  m_mainframe = new MainFrame(0, wxID_ANY, APP_NAME, wxDefaultPosition, wxSize(800,600));
  SetTopWindow(m_mainframe);
  m_mainframe->Show();

  m_hudfile = m_factory->create_hudfile();
  PakManager::get().init();
  m_mainframe->elementsctrl()->OnSelectionChanged();

  return true;
}


int SHEApp::OnRun()
{
  wxLogDebug(wxT("SHEApp::OnRun"));
  m_hudfile->OnNew();
  m_mainframe->update_title();
  if( Prefs::get().var(wxT("startup_load")) && !Prefs::get().var(wxT("startup_loadfile")).stringval().empty() )
    m_hudfile->OnOpen( Prefs::get().var(wxT("startup_loadfile")) );

  PakManager::get().debug();
  m_ready = true;
  /*
  if( m_firststart )
  {
    wxCommandEvent evt;
    m_frame->OnMenuHelp( evt );
  }
  */
  /*
  if( Prefs::get()->startup_checkforupdates )
    m_frame->check_for_updates();
    */
  

  return wxApp::OnRun();
}

int SHEApp::OnExit()
{
  m_ready = false;
  wxLogDebug(wxT("SHEApp::OnExit"));
  Prefs::get().save();
  Prefs::get().shutdown();

  m_factory->shutdown();
  wxDELETE(m_factory);
  wxDELETE(m_hudfile);
  PakManager::get().shutdown();

  return 0;
}



bool SHEApp::is_cpma() const
{
  return (Prefs::get().var(wxT("game")).stringval().CmpNoCase(wxT("cpma")) == 0);
}

bool SHEApp::is_q4max() const
{
  return (Prefs::get().var(wxT("game")).stringval().CmpNoCase(wxT("q4max")) == 0);
}
