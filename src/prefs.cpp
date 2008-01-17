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


Variable::Variable( const wxString& name, const wxString& def, int type, int flags /*= PVF_NONE*/ ) :
  m_name(name),
  m_value(wxT("")),
  m_def(def),
  m_type(type),
  m_flags(flags),
  m_isset(false),
  m_uptodate(false)
{
}

void Variable::read()
{
  wxConfigBase *c = wxConfigBase::Get();
  wxString val;
  m_isset = c->Read(m_name, &val, m_def);
  set(val, m_isset);
}

void Variable::write()
{
  wxConfigBase *c = wxConfigBase::Get();
  if( m_flags & PVF_NOARCHIVE ) 
    return;
  if( !(m_flags & PVF_ARCHIVEALWAYS) )
  {
    if( !m_isset ) 
      return;
    if( m_value == m_def )
    {
      c->DeleteEntry(m_name);
      return;
    }
  }
  c->Write(m_name, m_value);
}
void Variable::set_default()
{
  set(m_def);
}

void Variable::set( const wxString& str, bool isset /*=true*/ )
{
  m_isset = isset;
  m_value = str;
  if( m_type == PVT_INT )
  {
    long val;
    m_value.ToLong(&val);
    intval = static_cast<int>(val);
  }
  else if( m_type == PVT_DOUBLE )
  {
    if( !she::ratio_string2double(m_value, &doubleval) )
      wxLogError(wxT("Invalid value for aspectratio"));
  }
  else if( m_type == PVT_BOOL )
  {
    boolval = (m_value == wxT("true") || m_value == wxT("yes") || m_value == wxT("1"));
  }
  else if( m_type == PVT_COLOR )
  {
    colorval.set(m_value);
  }
}

const Variable& Prefs::var( const wxString& name ) const
{
  variables_type::const_iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable: ") + name );
  return var->second;
}

void Prefs::set( const wxString& name, const wxString& val )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  var->second.set(val);
}

void Prefs::set_default( const wxString& name )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  var->second.set_default();
}

void Prefs::setb( const wxString& name, bool bval )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  var->second.set( bval ? wxT("true") : wxT("false"));
}

void Prefs::seti( const wxString& name, int ival )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  var->second.set( wxString::Format(wxT("%i"), ival) );
}

void Prefs::setv( const wxString& name, const wxVariant& variant )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  var->second.set( variant.MakeString() );
}

void Prefs::setwxc( const wxString& name, const wxColour& wxcol, int alpha /*= -1*/ )
{
  variables_type::iterator var = m_vars.find(name);
  wxASSERT_MSG( var != m_vars.end(), wxT("Cannot find variable ") + name );
  if( var->second.cval().set(wxcol) )
  { // ok looks like the color changed.. so let's update
    Color4 tmp;
    tmp.set(wxcol);
    if( -1 != alpha )
      tmp.set_a100(alpha);
    var->second.set( tmp.to_string() );
  }
}

bool Prefs::init()
{
  if( !wxDir::Exists( wxStandardPaths::Get().GetUserDataDir() ) )
    wxMkdir( wxStandardPaths::Get().GetUserDataDir() );
  wxLogDebug(wxT("Prefs::init - Loading config file: ") + wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );

  bool confexists = wxFile::Exists( wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );
  wxConfigBase::Set( new wxFileConfig( APP_NAME_UNIX, APP_VENDOR, wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG) );
  //wxConfigBase::Get()->SetRecordDefaults();

  load();

  return !confexists;
}



void Prefs::addvar( const wxString& name, const wxString& def /*= wxT("")*/, int type /*= PVT_ANY*/, int flags /*= PVF_NONE*/ )
{
  m_vars.insert( std::make_pair(name, Variable(name, def, type, flags)) );
}


void Prefs::load()
{
  
  addvar(wxT("app_version"), APP_VERSION, PVT_STRING, PVF_ARCHIVEALWAYS);

  // -- display
  addvar(wxT("game"), wxT(""), PVT_STRING);
  addvar(wxT("app_maximized"), wxT("false"), PVT_BOOL);
  addvar(wxT("app_width"), wxT("-1"), PVT_INT);
  addvar(wxT("app_height"), wxT("-1"), PVT_INT);
  addvar(wxT("view_aspectratio"), wxT("4:3"), PVT_DOUBLE, PVF_NOARCHIVE);
  addvar(wxT("view_grid"), wxT("true"), PVT_BOOL);
  addvar(wxT("view_gridX"), wxT("12"), PVT_INT);
  addvar(wxT("view_gridY"), wxT("12"), PVT_INT);
  addvar(wxT("view_gridcolor"), wxT("1 1 1 0.3"), PVT_COLOR);
  addvar(wxT("view_helper"), wxT("true"), PVT_BOOL);
  addvar(wxT("view_helper_border"), wxT("1 1 1 0.8"), PVT_COLOR);
  addvar(wxT("view_helper_fill"), wxT("1 1 1 0.1"), PVT_COLOR);
  addvar(wxT("view_helper_border_selected"), wxT("1 0 0 0.8"), PVT_COLOR);
  addvar(wxT("view_helper_fill_selected"), wxT("1 0 0 0.1"), PVT_COLOR);
  addvar(wxT("elements_collections"), wxT("true"), PVT_BOOL);
  addvar(wxT("elements_collnamecount"), wxT("3"), PVT_INT); ///< minimum matching starting chars for a collection to be created
  addvar(wxT("app_perspective"), wxT(""), PVT_STRING);
  addvar(wxT("props_color"), PROPS_COLOR.to_string(), PVT_COLOR);
  addvar(wxT("props_bgcolor"), PROPS_BGCOLOR.to_string(), PVT_COLOR);
  addvar(wxT("props_inheritcolor"), PROPS_INHERITCOLOR.to_string(), PVT_COLOR);
  addvar(wxT("props_inheritbgcolor"), PROPS_INHERITBGCOLOR.to_string(), PVT_COLOR);
  
  addvar(wxT("view_dragthreshold"), wxT("1"), PVT_INT); ///< how many pixels till we start dragging
  addvar(wxT("view_snapthreshold"), wxT("3"), PVT_INT); ///< how many pixels we snap to snappable items
  addvar(wxT("view_snapelements"), wxT("true"), PVT_BOOL); ///< whether to snap to other elements
  addvar(wxT("view_snapgrid"), wxT("true"), PVT_BOOL); ///< whether to snap to grid
  addvar(wxT("view_movestep"), wxT("12"), PVT_INT); ///< while moving/resizing with keys, how much to skip (by default same as grid)
  addvar(wxT("props_neverdisable"), wxT("false"), PVT_BOOL); ///< only if something is wrong, this quick hack can be used to not disable stuff at all in properties pane
  addvar(wxT("view_updatewhiledragging"), wxT("true"), PVT_BOOL); ///< should give smoother drag&drop if disable


    // -- game specific
  // cpma
  addvar(wxT("q3_gamedir"), wxT(""), PVT_STRING);
#ifndef WIN32
  addvar(wxT("q3_homedirname"), PREFS_Q3_HOMEDIRNAME_DEFAULT, PVT_STRING, PVF_NOARCHIVE);
#endif
  
  wxString prefs_q3_pakfiles_default = PREFS_Q3_PAKFILES_DEFAULT;
  prefs_q3_pakfiles_default.Replace(wxT("/"), PATH_SEP);
  addvar(wxT("q3_pakfiles"), prefs_q3_pakfiles_default, PVT_STRING);
  addvar(wxT("q3_hudspecs"), wxT("cpma/hudspecs.dat"), PVT_STRING);
  addvar(wxT("q3_background"), wxT(""), PVT_STRING);
  addvar(wxT("q3_filedialog_path"), wxT(""), PVT_STRING);

  // q4max
  addvar(wxT("q4_gamedir"), wxT(""), PVT_STRING);

  // -- optional message dialogs
  addvar(wxT("dlg_reset"), wxT("-1"), PVT_INT);
  addvar(wxT("dlg_delete"), wxT("-1"), PVT_INT);
  addvar(wxT("dlg_convertonload"), wxT("-1"), PVT_INT);

  
  // -- misc

  // -- startup
  addvar(wxT("startup_gameselection"), wxT("true"), PVT_BOOL);
  addvar(wxT("startup_load"), wxT("true"), PVT_BOOL);
  addvar(wxT("startup_loadfile"), wxT(""), PVT_STRING);
  addvar(wxT("startup_tips"), wxT("true"), PVT_BOOL);
  addvar(wxT("startup_tipidx"), wxT("0"), PVT_INT);
  addvar(wxT("net_proxy"), wxT(""), PVT_STRING);
  addvar(wxT("startup_checkforupdate"), wxT("true"), PVT_BOOL);

  // -- saving
  addvar(wxT("save_writedisabled"), wxT("true"), PVT_BOOL);
  addvar(wxT("save_backup"), wxT("true"), PVT_BOOL);
  addvar(wxT("load_autoconvert"), wxT("false"), PVT_BOOL); ///< whether to auto convert huds that have a different aspectratio than we would like

  for( variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    it->second.read();

  if( APP_VERSION != var(wxT("app_version")) )
  { // different app version has written this config file.. we might need to do some conversion work
  }
  set(wxT("app_version"), APP_VERSION);
}

void Prefs::save( bool from_prefs_dialog /*= false*/ )
{
  for( variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    it->second.write();

  wxConfigBase::Get()->Flush();
}

void Prefs::shutdown()
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
