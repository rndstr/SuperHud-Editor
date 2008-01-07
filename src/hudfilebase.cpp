#include "hudfilebase.h"
#include "mainframe.h"
#include "elementsctrlbase.h"
#include "displayctrlbase.h"
#include "prefs.h"

#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>

#include <algorithm>
using namespace std;

DECLARE_APP(SHEApp);

HudFileBase::HudFileBase() :
  m_modified(false)
{
}




void HudFileBase::set_modified( bool modified /*= true*/ )
{ 
  if( m_modified != modified )
  {
    m_modified = modified;
    wxGetApp().mainframe()->update_title();
    if( modified ) wxGetApp().mainframe()->displayctrl()->Refresh();
  }
}
int HudFileBase::OnOpen( const wxString& filename /*=wxT("")*/ )
{
  wxString f = filename;
  int ret = wxID_OK;
  if( f.empty() )
  {
    wxFileDialog dlg(
        wxGetApp().mainframe(),
        _("Open..."),
        wxT(""),
        wxT(""),
        wxT("Hud Files (*.cfg)|*.cfg|All Files (*.*)|*.*"),
  #if wxCHECK_VERSION(2,7,0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
  #else
        wxOPEN | wxFILE_MUST_EXIST
  #endif
        );
    if( wxID_OK != (ret = dlg.ShowModal()) )
      return ret;
    f = dlg.GetPath();
  }

  wxASSERT( !f.empty() );

  wxGetApp().mainframe()->statusbar()->PushStatusText(wxString::Format(_("Loading hud: %s"), f.c_str()));
  wxBusyCursor wait0r;
  if( !load( f ) )
  {
    wxLogError( _("Failed reading Hud from file `%s'"), f.c_str() );
    wxGetApp().mainframe()->statusbar()->PopStatusText();
  }
  wxGetApp().mainframe()->statusbar()->PopStatusText();
  return ret;
}

int HudFileBase::OnSave( )
{
  int ret = wxID_OK;
  if( m_filename.empty() )
  {
    wxFileDialog dlg(
        wxGetApp().mainframe(),
        _("Save..."),
        wxT(""),
        wxT(""),
        wxT("Hud Files (*.cfg)|*.cfg|All Files (*.*)|*.*"),
  #if wxCHECK_VERSION(2,7,0)
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
  #else // 2.6
        wxSAVE|wxOVERWRITE_PROMPT
  #endif
        );
    if( wxID_OK == (ret = dlg.ShowModal()) )
      m_filename = dlg.GetPath();
    else
      return wxID_CANCEL;
  }
  if( m_filename.empty() )
    return wxID_CANCEL; // this shouldn't reach here.. but make sure anyway

  if( Prefs::get().var(wxT("save_backup")) && wxFile::Exists(m_filename)  )
  {
    wxString target = m_filename + wxT(".bak");
    wxGetApp().mainframe()->statusbar()->PushStatusText(wxString::Format(_("Creating backup hud: %s"), target.c_str()));
    if( !wxCopyFile( m_filename, target, true ) )
      wxLogError( _("Failed creating backup hud: %s"), target.c_str() );
    wxGetApp().mainframe()->statusbar()->PopStatusText();
  }
  wxGetApp().mainframe()->statusbar()->PushStatusText(wxString::Format(_("Saving hud: %s"), m_filename.c_str()));
  wxBusyCursor wait;
  if( !save( m_filename ) )
  {
    wxLogError( _("Failed writing hud to `%s'"), m_filename.c_str() );
    wxGetApp().mainframe()->statusbar()->PopStatusText();
    return wxID_CANCEL;
  }
  wxGetApp().mainframe()->statusbar()->PopStatusText();
  return wxID_OK;

}

void HudFileBase::clear()
{
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
    delete (*cit);
  m_els.clear();
}


void HudFileBase::OnNew()
{
  clear();
  load_default_elements();
  wxGetApp().elementsctrl()->list_refresh(m_els);
}

void HudFileBase::append( ElementBase *el )
{
  set_modified();
  m_els.push_back(el);
}

void HudFileBase::insert( size_t pos, ElementBase *el )
{
  set_modified();
  m_els.insert( m_els.begin() + pos, el );
}
void HudFileBase::insert( const ElementBase * const after, ElementBase *el )
{
  set_modified();
  for( it_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( *it == after )
    {
      m_els.insert( ++it, el );
      return;
    }
  }
  // just append if we can't find it
  wxLogDebug(wxT("HudFileBase::insert - Couldn't find element to insert after"));
  append(el);
}

bool HudFileBase::remove( ElementBase *el )
{
  for( it_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( *it == el )
    {
      set_modified();
      m_els.erase(it);
      return true;
    }
  }
  return false;
}

ElementBase* HudFileBase::find_element( const wxString& name )
{
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
    if( (*cit)->name().CmpNoCase(name) == 0 )
      return (*cit);
  return 0;
}

bool HudFileBase::move_element_after( ElementBase *el, ElementBase *after )
{
  if( el == after )
    return true;
  it_elements el_it = find( m_els.begin(), m_els.end(), el );
  if( el_it == m_els.end() )
    return false; // element to move not found
 
  if( 0 == after )
  { // add at beginning
    m_els.erase( el_it );
    m_els.insert( m_els.begin(), el );
  }
  else
  {
    if( find( m_els.begin(), m_els.end(), after ) == m_els.end() )
      return false; // element to move it after not found
    m_els.erase( el_it );
    it_elements after_it = find( m_els.begin(), m_els.end(), after );
    m_els.insert( ++after_it, el );
  }
  set_modified();
  return true;
}

bool HudFileBase::save( const wxString& filename )
{
  wxFFileOutputStream file( filename.c_str() );
  if( !file.Ok() )
    return false;
  wxTextOutputStream stream( file );
  wxDateTime dt(wxDateTime::Now());
  
  stream << wxT("# written by ") << APP_NAME << wxT(" v") << APP_VERSION << wxT(" on ") << dt.FormatISODate() << wxT(" ") << dt.FormatISOTime() << wxT("\n");
  stream << wxT("# ") << APP_URL << wxT("\n");

  for( cit_elements it = m_els.begin(); it != m_els.end(); ++it )
    write_element( stream, *(*it) );

  m_filename = filename;
  m_modified = false;

  return true;
}

void HudFileBase::write_element( wxTextOutputStream& stream, const ElementBase& el )
{
  if( !el.is_enabled() )
  {
    if( Prefs::get().var(wxT("save_writedisabled")) )
      stream << wxT("# ") << el.name() << wxT(" { }\n");
    return;
  }
  if( el.flags() & E_SHORT )
  {
    stream << el.name() << wxT(" { ");
    el.write_properties(stream);
    stream << wxT("}\n");
  }
  else
  {
    stream << wxT("\n") << el.name() << wxT("\n{");
    el.write_properties(stream);
    stream << wxT("\n}\n\n");
  }
}

const ElementBase* HudFileBase::get_parent( const ElementBase * const from, int specifies /*= E_HAS_NONE*/ ) const
{
  if( from->flags() & E_NOINHERIT )
    return 0; // the item (of which we would like to retrieve the parent) doesn't inherit at all.

  const ElementBase *d = 0;
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
  {
    if( *cit == from )
      break;
    // is he a parent and does specify what we are looking for?
    if( ((*cit)->flags() & E_PARENT) && ((*cit)->has() & specifies) )
      d = *cit;
    // since we start from top and we want the most near parent, we continue here :o
  }
  return d;
}

