#include "hudfilebase.h"
#include "mainframe.h"
#include "elementsctrlbase.h"
#include <algorithm>
using namespace std;

DECLARE_APP(SHEApp);

HudFileBase::HudFileBase() :
  m_modified(false)
{
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

void HudFileBase::write_element( wxTextOutputStream& stream, const ElementBase& el )
{
  if( !el.is_enabled() )
  {
    stream << wxT("# ") << el.name() << wxT(" { }\n");
    return;
  }
  stream << el.name();
  if( el.flags() & E_SHORT )
    stream << wxT(" { ");
  else
    stream << wxT("\n{");
  el.write_properties(stream);
  if( el.flags() & E_SHORT )
    stream << wxT("}\n");
  else
    stream << wxT("\n}\n\n");
}
