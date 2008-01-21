// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "prefs.h"

#include "misc.h"
#include "factorybase.h"
#include "propertiesnotebookbase.h"

#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/fileconf.h>

/// NOTE that this contain "wrong" path separators, will be replaced with PATH_SEP further down
const wxString PREFS_Q3_PAKFILES_DEFAULT = wxT("baseq3/pak*.pk3;baseq3/map_cpm*.pk3;cpma/z-cpma-pak*.pk3");
#ifndef WIN32
const wxString PREFS_Q3_HOMEDIRNAME_DEFAULT = wxT(".q3a");
#endif
const wxString PREFS_Q4_PAKFILES_DEFAULT = wxT("q4base/pak*.pk4;q4max/zz-q4max-*.pk4");
#ifndef WIN32
const wxString PREFS_Q4_HOMEDIRNAME_DEFAULT = wxT(".quake4");
#endif




bool Prefs::init()
{
  if( !wxDir::Exists( wxStandardPaths::Get().GetUserDataDir() ) )
    wxMkdir( wxStandardPaths::Get().GetUserDataDir() );
  wxLogDebug(wxT("Prefs::init - Loading config file: ") + wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );

  bool confexists = wxFile::Exists( wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );
  wxConfigBase::Set( new wxFileConfig( APP_NAME_UNIX, APP_VENDOR, wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG) );

  addvar(wxT("app_version"), APP_VERSION, VART_STRING, VARF_ARCHIVEALWAYS);

  // -- display
  addvar(wxT("game"), wxT(""), VART_STRING);
  addvar(wxT("app_maximized"), wxT("false"), VART_BOOL);
  addvar(wxT("app_width"), wxT("-1"), VART_INT);
  addvar(wxT("app_height"), wxT("-1"), VART_INT);
  addvar(wxT("view_aspectratio"), wxT("4:3"), VART_DOUBLE, VARF_NOARCHIVE);
  addvar(wxT("view_grid"), wxT("true"), VART_BOOL);
  addvar(wxT("view_gridX"), wxT("12"), VART_INT);
  addvar(wxT("view_gridY"), wxT("12"), VART_INT);
  addvar(wxT("view_gridcolor"), wxT("1 1 1 0.3"), VART_COLOR);
  addvar(wxT("view_helper"), wxT("true"), VART_BOOL);
  addvar(wxT("view_helper_border"), wxT("1 1 1 0.8"), VART_COLOR);
  addvar(wxT("view_helper_fill"), wxT("1 1 1 0.1"), VART_COLOR);
  addvar(wxT("view_helper_border_selected"), wxT("1 0 0 0.8"), VART_COLOR);
  addvar(wxT("view_helper_fill_selected"), wxT("1 0 0 0.1"), VART_COLOR);
  addvar(wxT("elements_collections"), wxT("true"), VART_BOOL);
  addvar(wxT("elements_collnamecount"), wxT("3"), VART_INT); ///< minimum matching starting chars for a collection to be created
  addvar(wxT("app_perspective"), wxT(""), VART_STRING);
  addvar(wxT("props_color"), PROPS_COLOR.to_string(), VART_COLOR);
  addvar(wxT("props_bgcolor"), PROPS_BGCOLOR.to_string(), VART_COLOR);
  addvar(wxT("props_inheritcolor"), PROPS_INHERITCOLOR.to_string(), VART_COLOR);
  addvar(wxT("props_inheritbgcolor"), PROPS_INHERITBGCOLOR.to_string(), VART_COLOR);
  addvar(wxT("view_suppresshelpergrid"), wxT("false"), VART_BOOL, VARF_NOARCHIVE); ///< we only keep this while running...
  
  addvar(wxT("view_dragthreshold"), wxT("1"), VART_INT); ///< how many pixels till we start dragging
  addvar(wxT("view_snapthreshold"), wxT("3"), VART_INT); ///< how many pixels we snap to snappable items
  addvar(wxT("view_snapelements"), wxT("true"), VART_BOOL); ///< whether to snap to other elements
  addvar(wxT("view_snapgrid"), wxT("true"), VART_BOOL); ///< whether to snap to grid
  addvar(wxT("view_movestep"), wxT("12"), VART_INT); ///< while moving/resizing with keys, how much to skip (by default same as grid)
  addvar(wxT("props_neverdisable"), wxT("false"), VART_BOOL); ///< only if something is wrong, this quick hack can be used to not disable stuff at all in properties pane
  addvar(wxT("view_updatewhiledragging"), wxT("true"), VART_BOOL); ///< should give smoother drag&drop if disable


    // -- game specific
  // cpma
  addvar(wxT("q3_gamedir"), wxT(""), VART_STRING);
#ifndef WIN32
  addvar(wxT("q3_homedirname"), PREFS_Q3_HOMEDIRNAME_DEFAULT, VART_STRING, VARF_NOARCHIVE);
#endif
  
  wxString prefs_q3_pakfiles_default = PREFS_Q3_PAKFILES_DEFAULT;
  prefs_q3_pakfiles_default.Replace(wxT("/"), PATH_SEP);
  addvar(wxT("q3_pakfiles"), prefs_q3_pakfiles_default, VART_STRING);
  addvar(wxT("q3_hudspecs"), wxT("cpma/hudspecs.dat"), VART_STRING);
  addvar(wxT("q3_background"), wxT(""), VART_STRING);
  addvar(wxT("q3_filedialog_path"), wxT(""), VART_STRING);
  addvar(wxT("q3_startup_loadfile"), wxT(""), VART_STRING);
  

  // q4max
  addvar(wxT("q4_gamedir"), wxT(""), VART_STRING);
#ifndef WIN32
  addvar(wxT("q4_homedirname"), PREFS_Q4_HOMEDIRNAME_DEFAULT, VART_STRING, VARF_NOARCHIVE);
#endif
  wxString prefs_q4_pakfiles_default = PREFS_Q4_PAKFILES_DEFAULT;
  prefs_q4_pakfiles_default.Replace(wxT("/"), PATH_SEP);
  addvar(wxT("q4_pakfiles"), prefs_q4_pakfiles_default, VART_STRING);
  addvar(wxT("q4_hudspecs"), wxT("q4max/hudspecs.dat"), VART_STRING);
  addvar(wxT("q4_background"), wxT(""), VART_STRING);
  addvar(wxT("q4_filedialog_path"), wxT(""), VART_STRING);
  addvar(wxT("q4_startup_loadfile"), wxT(""), VART_STRING);

  // -- optional message dialogs
  addvar(wxT("dlg_reset"), wxT("-1"), VART_INT);
  addvar(wxT("dlg_delete"), wxT("-1"), VART_INT);
  addvar(wxT("dlg_convertonload"), wxT("-1"), VART_INT);

  
  // -- misc

  // -- startup
  addvar(wxT("startup_gameselection"), wxT("true"), VART_BOOL);
  addvar(wxT("startup_load"), wxT("true"), VART_BOOL);
  addvar(wxT("startup_tips"), wxT("true"), VART_BOOL);
  addvar(wxT("startup_tipidx"), wxT("0"), VART_INT);
  addvar(wxT("net_proxy"), wxT(""), VART_STRING);
  addvar(wxT("startup_checkforupdate"), wxT("true"), VART_BOOL);

  // -- saving
  addvar(wxT("save_writedisabled"), wxT("true"), VART_BOOL);
  addvar(wxT("save_backup"), wxT("true"), VART_BOOL);
  addvar(wxT("load_autoconvert"), wxT("false"), VART_BOOL); ///< whether to auto convert huds that have a different aspectratio than we would like

  return !confexists;
}

void Prefs::load()
{
  VarContainer<Var>::load();
  
  if( APP_VERSION != var(wxT("app_version")).sval() )
  { // different app version has written this config file.. we might need to do some conversion work
  }
  set(wxT("app_version"), APP_VERSION);
}


void Prefs::read_var( var_type& var )
{
  wxConfigBase *c = wxConfigBase::Get();
  wxString val;
  var.m_isset = c->Read( var.m_name, &val, var.m_def );
  var.set(val, var.m_isset);
}

void Prefs::write_var( const var_type& var ) const
{
  wxConfigBase *c = wxConfigBase::Get();
  if( var.m_flags & VARF_NOARCHIVE )
    return;
  if( !(var.m_flags & VARF_ARCHIVEALWAYS) )
  {
    if( !var.m_isset )
      return;
    if( var.m_value == var.m_def )
    {
      c->DeleteEntry( var.m_name );
      return;
    }
  }
  c->Write( var.m_name, var.m_value );
}

void Prefs::save()
{
  VarContainer<Var>::save();
  wxConfigBase::Get()->Flush();
}

void Prefs::cleanup()
{
  delete wxConfigBase::Set((wxConfigBase *)0);
}

wxString Prefs::filedialog_path() const
{
  wxString path = wxGetApp().factory()->filedialog_path();
  const wxString dir_game = wxGetApp().factory()->dir_game();
  if( path.empty() && !dir_game.empty() )
    path = dir_game + PATH_SEP + wxGetApp().factory()->dirname_moddata() + PATH_SEP + wxT("hud");
  return path;
}


Prefs& Prefs::get() 
{
  static Prefs pinstance;
  return pinstance;
}
