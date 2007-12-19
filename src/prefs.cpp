#include "prefs.h"
#include "common.h"

#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/fileconf.h>

/// NOTE that this contain "wrong" path separators, will be replaced with PATH_SEP further down
const wxString PREFS_Q3_PAKFILES_DEFAULT = wxT("baseq3/pak*.pk3;baseq3/map_cpm*.pk3;cpma/z-cpma-pak*.pk3");
#ifndef WIN32
  const wxString PREFS_Q3_HOMEDIRNAME_DEFAULT = wxT(".q3a");
#endif


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
  wxString str;
  wxConfigBase *c = wxConfigBase::Get();

  // -- display
  c->Read(wxT("game"), &game, wxT(""));
  c->Read(wxT("perspective"), &perspective, wxT(""));
  c->Read(wxT("grid"), &grid, true);
  c->Read(wxT("grid_x"), &grid_x, 10);
  c->Read(wxT("grid_y"), &grid_y, 10);
  c->Read(wxT("grid_color"), &str, wxT("1 1 1 0.3"));
  grid_color.from_string(str);



  // -- game specific
  // cpma
  c->Read(wxT("q3_gamedir"), &q3_gamedir, wxT(""));
#ifndef WIN32
  c->Read(wxT("q3_homedirname"), &q3_homedirname, PREFS_Q3_HOMEDIRNAME_DEFAULT);
#endif
  
  wxString prefs_q3_pakfiles_default = PREFS_Q3_PAKFILES_DEFAULT;
  prefs_q3_pakfiles_default.Replace(wxT("/"), PATH_SEP);
  c->Read(wxT("q3_pakfiles"), &q3_pakfiles, prefs_q3_pakfiles_default);
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
