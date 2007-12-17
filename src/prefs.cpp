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

  // -- game specific
  // cpma
  c->Read(wxT("q3_gamedir"), &q3_gamedir, wxT(""));
  c->Read(wxT("q3_pakfiles"), &q3_pakfiles, PREFS_Q3_PAKFILES_DEFAULT);
  // q4max
  c->Read(wxT("q4_gamedir"), &q4_gamedir, wxT(""));

  // -- misc
  c->Read(wxT("hudspecs"), &hudspecs, wxT("")); // this is only read

  // -- startup
  c->Read(wxT("startup_gameselection"), &startup_gameselection, true);

  // -- saving
  c->Read(wxT("save_writedisabled"), &save_writedisabled, true);
  c->Read(wxT("save_backup"), &save_backup, true);

}

void Prefs::save( bool from_prefs_dialog /*= false*/ )
{
  wxConfigBase *c = wxConfigBase::Get();
  // -- display
  c->Write(wxT("game"), game);
  c->Write(wxT("perspective"), perspective);

  // -- game specific
  // cpma
  c->Write(wxT("q3_gamedir"), q3_gamedir);
  //c->Write(wxT("q3_pakfiles"), q3_pakfiles);

  // q4max
  c->Write(wxT("q4_gamedir"), q4_gamedir);

  // -- startup
  c->Write(wxT("startup_gameselection"), startup_gameselection);

  // -- saving
  c->Write(wxT("save_writedisabled"), save_writedisabled);
  c->Write(wxT("save_backup"), save_backup);

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
