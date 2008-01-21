// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "superhudeditor.h"

#include "elementsctrlbase.h"
#include "displayctrlbase.h"
#include "setupwizard.h"

#include "factorybase.h"
#include "hudfilebase.h"
#include "pakmanager.h"
#include "GameSelectionDialog.h"
#include "artprovider.h"

#if ENABLE_CPMA
  #include "cpma_factory.h"
  #include "cpma_hudfile.h"
#endif
#if ENABLE_Q4MAX
  #include "q4max_factory.h"
  #include "q4max_hudfile.h"
#endif


#include "hudspecs.h"

#include <wx/stdpaths.h>
#include <wx/fs_arc.h>
#include <wx/cmdproc.h>
#include <wx/socket.h>


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
  

  if( !wxApp::OnInit() )
    return false;

  // set up config file
  m_firststart = Prefs::get().init();
  Prefs::get().load();

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

  wxLogDebug(APP_CAPTION);
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

#if ENABLE_CPMA
  if( is_cpma() )
    m_factory = new CPMAFactory;
#endif
#if ENABLE_Q4MAX
  if( is_q4max() )
    m_factory = new Q4MAXFactory;
#endif

  wxASSERT_MSG(m_factory, wxT("no factory created!"));

  PakManager::get().init_app();
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
      wxGetApp().factory()->set_dir_game(wizard.gamedirpage()->gamedir());
      Prefs::get().set(wxT("view_aspectratio"), wizard.miscpage()->aspectratio());
      Prefs::get().set(wxT("net_proxy"), wizard.miscpage()->proxy());
      Prefs::get().setb(wxT("startup_checkforupdate"), wizard.miscpage()->checkforupdate());
      Prefs::get().save(); // just save to make sure.. if we crash at least this we don't have to go through again
    }
  }
  else if( m_factory->dir_game().empty() )
  {
    SetupWizard wizard(0);
    if( wizard.RunWizard(wizard.GetFirstPage()) )
      wxGetApp().factory()->set_dir_game(wizard.gamedirpage()->gamedir());
  }

  PakManager::get().init_game();

  m_mainframe = new MainFrame(0, wxID_ANY, APP_NAME, wxDefaultPosition, wxSize(800,600));
  SetTopWindow(m_mainframe);
  m_mainframe->Show();

  m_hudfile = m_factory->create_hudfile();

#if HAS_UNDO
  m_commands = new wxCommandProcessor();
  m_commands->SetEditMenu(m_mainframe->edit_menu());
  m_commands->Initialize();
#endif

  m_mainframe->elementsctrl()->OnSelectionChanged();
  wxSocketBase::Initialize();
  return true;
}


int SHEApp::OnRun()
{
  wxLogDebug(wxT("SHEApp::OnRun"));
  m_mainframe->update_title();
  if( m_cmdline_file.empty() )
  {
    wxString loadfile = wxGetApp().factory()->startup_loadfile();
    if( Prefs::get().var(wxT("startup_load")) && !loadfile.empty() )
      m_hudfile->OnOpen( loadfile );
    else
      m_hudfile->OnNew();
  }
  else
    m_hudfile->OnOpen( m_cmdline_file );


  m_mainframe->update_title();
  m_ready = true;

  if( Prefs::get().var(wxT("startup_tips")).bval() )
  {
    wxCommandEvent plaarfz;
    m_mainframe->OnMenuHelpTip(plaarfz);
  }
  
  if( Prefs::get().var(wxT("startup_checkforupdate")).bval() )
    m_mainframe->check_for_updates();

  return wxApp::OnRun();
}

int SHEApp::OnExit()
{
  m_ready = false;
  wxLogDebug(wxT("SHEApp::OnExit"));
  Prefs::get().save();
  Prefs::get().cleanup();

#if HAS_UNDO
  m_commands->SetEditMenu(0);
  wxDELETE(m_commands);
#endif

  m_factory->shutdown();
  wxDELETE(m_factory);
  wxDELETE(m_hudfile); // this is segfaulting if nothing is being done
  PakManager::get().shutdown();

  if( !m_exec.empty() )
    wxExecute( m_exec, wxEXEC_ASYNC );

  return 0;
}

void SHEApp::OnInitCmdLine( wxCmdLineParser& parser )
{
  parser.SetDesc( g_cmdLineDesc );
  parser.SetSwitchChars (wxT("-"));
}
bool SHEApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  if( !parser.GetParamCount() )
    return true;

  if( parser.GetParamCount() > 1 )
  {
    wxLogWarning(_("Too many parameters, only one allowed (a single HUD to open)"));
    return false;
  }
  m_cmdline_file = parser.GetParam(0);
  return true;
}

#if HAS_CMDPROC
wxCommandProcessor* SHEApp::cmds()
{ 
  return m_commands; 
}
#endif


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

