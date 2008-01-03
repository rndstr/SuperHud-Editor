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

void Variable::set( const wxString& str, bool isset /*=true*/ )
{
  m_isset = isset;
  m_value = str;
  if( m_type == PVT_INT )
  {
    long val;
    m_value.ToLong(&val);
    ival = static_cast<int>(val);
  }
  else if( m_type == PVT_FLOAT )
  {
    double val;
    if( m_value.ToDouble(&val) )
      fval = static_cast<float>(val);
    else
    { // maybe it's 4:3 ?
      size_t pos = m_value.Find(wxT(":"));
      if( pos != wxString::npos )
      {
        long lhs, rhs;
        if( !m_value.Left(pos).ToLong(&lhs) || !m_value.Right(m_value.length() - pos - 1).ToLong(&rhs) )
        {
          wxLogError(wxT("Invalid value for aspectratio"));
        }
        else
          fval = static_cast<float>(lhs)/static_cast<float>(rhs);
      }
    }
  }
  else if( m_type == PVT_BOOL )
  {
    bval = (m_value == wxT("true") || m_value == wxT("yes") || m_value == wxT("1"));
  }
  else if( m_type == PVT_COLOR )
  {
    cval.set(m_value);
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
  
  

  // -- display
  addvar(wxT("game"), wxT(""), PVT_STRING);
  addvar(wxT("app_maximized"), wxT("false"), PVT_BOOL);
  addvar(wxT("app_width"), wxT("-1"), PVT_INT);
  addvar(wxT("app_height"), wxT("-1"), PVT_INT);
  addvar(wxT("view_aspectratio"), wxT("4:3"), PVT_FLOAT);
  addvar(wxT("view_grid"), wxT("true"), PVT_BOOL);
  addvar(wxT("view_gridX"), wxT("16"), PVT_INT);
  addvar(wxT("view_gridY"), wxT("16"), PVT_INT);
  addvar(wxT("grid_color"), wxT("1 1 1 0.3"), PVT_COLOR);
  addvar(wxT("view_helper"), wxT("true"), PVT_BOOL);
  addvar(wxT("view_helper_border"), wxT("1 1 1 0.8"), PVT_COLOR);
  addvar(wxT("view_helper_fill"), wxT("1 1 1 0.1"), PVT_COLOR);
  addvar(wxT("view_helper_border_selected"), wxT("1 0 0 0.8"), PVT_COLOR);
  addvar(wxT("view_helper_fill_selected"), wxT("1 0 0 0.1"), PVT_COLOR);
  addvar(wxT("elements_collections"), wxT("true"), PVT_BOOL);
  addvar(wxT("app_perspective"), wxT(""), PVT_STRING);


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
  addvar(wxT("q3_background"), wxT("cpma/texture/background.jpg"), PVT_STRING);

  // q4max
  addvar(wxT("q4_gamedir"), wxT(""), PVT_STRING);
  // -- misc

  // -- startup
  addvar(wxT("startup_gameselection"), wxT("true"), PVT_BOOL);
  addvar(wxT("startup_load"), wxT("true"), PVT_BOOL);
  addvar(wxT("startup_loadfile"), wxT(""), PVT_STRING);

  // -- saving
  addvar(wxT("save_writedisabled"), wxT("true"), PVT_BOOL);
  addvar(wxT("save_backup"), wxT("true"), PVT_BOOL);

  for( variables_type::iterator it = m_vars.begin(); it != m_vars.end(); ++it )
    it->second.read();
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


Prefs& Prefs::get() 
{
  static Prefs pinstance;
  return pinstance;
}
