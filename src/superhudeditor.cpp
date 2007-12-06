

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "superhudeditor.h"
#include "mainframe.h"
#include "common.h"


#include <wx/stdpaths.h>
#include "factorybase.h"
#include "hudfilebase.h"

#ifndef DISABLE_CPMA
  #include "cpma/factory.h"
  #include "cpma/hudspecs.h"
  #include "cpma/hudfile.h"
#endif


IMPLEMENT_APP(SHEApp)

SHEApp::SHEApp() :
  m_firststart(false)
{

}

bool SHEApp::OnInit()
{
  SetAppName(APP_NAME);
#ifndef WIN32
  #ifdef PACKAGE_PREFIX
    ((wxStandardPaths&)wxStandardPaths::Get()).SetInstallPrefix( wxT( PACKAGE_PREFIX ) );
  #endif
#endif

  wxArtProvider::Push( new ArtProvider );

  // set up config file
  
  m_firststart = Prefs::get().init();

  if( (!is_cpma() && !is_q4max()) || Prefs::get().startup_gameselection )
  {
    wxCommandEvent ev;
    static_cast<MainFrame*>(GetTopWindow())->OnMenuGameSelection(ev);
    if( !is_cpma() && !is_q4max() )
    {
      wxLogError(_("Invalid game in settings"));
      // FIXME bail out!
    }
  }

  if( is_cpma() )
  {
    m_factory = new CPMAFactory;
  }
  else if( is_q4max() )
  {

  }
  m_factory->init();

  m_hudfile = m_factory->create_hudfile();

  wxFrame *frame = new MainFrame(0, wxID_ANY, APP_NAME, wxDefaultPosition, wxSize(800,600));
  SetTopWindow(frame);
  frame->Show();

  return true;
}

int SHEApp::OnRun()
{
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
  Prefs::get().save();
  Prefs::get().shutdown();

  m_factory->shutdown();
  wxDELETE(m_factory);
  wxDELETE(m_hudfile);

  return 0;
}


bool SHEApp::is_cpma() const
{
  return (Prefs::get().game.CmpNoCase(wxT("cpma")) == 0);
}

bool SHEApp::is_q4max() const
{
  return (Prefs::get().game.CmpNoCase(wxT("q4max")) == 0);
}