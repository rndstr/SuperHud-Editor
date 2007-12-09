#include "hudfilebase.h"
#include "mainframe.h"
#include "elementsctrlbase.h"

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
  }
}

void HudFileBase::clear()
{
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
    delete (*cit);
  m_els.clear();
}


void HudFileBase::on_new()
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

ElementBase* HudFileBase::find_element( const wxString& name )
{
  for( cit_elements cit = m_els.begin(); cit != m_els.end(); ++cit )
    if( (*cit)->name().CmpNoCase(name) == 0 )
      return (*cit);
  return 0;
}

bool HudFileBase::move_element_after( ElementBase *el, ElementBase *after )
{
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
    if( Prefs::get().save_writedisabled )
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
