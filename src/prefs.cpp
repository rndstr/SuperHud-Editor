#include "prefs.h"
#include "common.h"

#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/fileconf.h>


bool Prefs::init()
{
  if( !wxDir::Exists( wxStandardPaths::Get().GetUserDataDir() ) )
    wxMkdir( wxStandardPaths::Get().GetUserDataDir() );
  bool confexists = wxFile::Exists( wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );
  wxConfigBase::Set( new wxFileConfig( APP_NAME, APP_VENDOR, wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG) );
  wxConfigBase::Get()->SetRecordDefaults();

  load();

  return !confexists;
}

void Prefs::load()
{
  wxConfigBase *c = wxConfigBase::Get();

  // -- display
  c->Read(wxT("game"), &game, wxT(""));
  c->Read(wxT("perspective"), &perspective, wxT(""));

  // -- misc
  c->Read(wxT("hudspecs"), &hudspecs, wxT(""));

  // -- startup/saving
  c->Read(wxT("startup_gameselection"), &startup_gameselection, true);
}

void Prefs::save( bool from_prefs_dialog /*= false*/ )
{
  wxConfigBase *c = wxConfigBase::Get();
  // -- display
  c->Write(wxT("game"), game);
  c->Write(wxT("perspective"), perspective);

  // -- startup/saving
  c->Write(wxT("startup_gameselection"), startup_gameselection);
  c->Flush();
}

void Prefs::shutdown()
{
  delete wxConfigBase::Set((wxConfigBase *)0);
}


Prefs& Prefs::get() 
{
  static Prefs pinstance;
  return pinstance;
}
