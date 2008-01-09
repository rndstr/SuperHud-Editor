#include "elementbase.h"
#include "superhudeditor.h"
#include "hudfilebase.h"
#include "displayctrlbase.h"

#include <wx/tokenzr.h>


bool ElementBase::parse_property( const wxString& cmd, wxString args )
{
  if (cmd.CmpNoCase(wxT("rect")) == 0)
  {
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.x );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.y );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.width );
    sscanf( targ.GetNextToken().mb_str(), "%i", &m_rect.height );
    m_has |= E_HAS_RECT;
  }
  else
    return false;

  return true;
}
void ElementBase::copy_from( const ElementBase * const el )
{ 
  // NOTE we do not copy visibility (position, size, enabled/disabled)
  wxLogDebug(wxT("COPY FROM %s TO %s"), el->name().c_str(), name().c_str()); 
  int prevhas = m_has;
  m_has = el->has();
  add_has( E_HAS_RECT, (prevhas & E_HAS_RECT) != 0 ); 

  //m_enabled = el->is_enabled();
  // we do not copy position... would just confuse
  //m_rect.width = el->rect().width;
  //m_rect.height = el->rect().height;
}
void ElementBase::reset()
{ 
  // NOTE we do not reset visibility
  wxLogDebug(wxT("RESET %s"), name().c_str()); 
  m_has = (m_has & E_HAS_RECT); // keep the rectangle if we have it already
  //m_rect.width = E_RECT_DEFAULT.width;
  //m_rect.height = E_RECT_DEFAULT.height;
}


void ElementBase::write_properties( wxTextOutputStream& stream ) const
{
  if( m_has & E_HAS_RECT ) 
  {
    if( m_flags & E_SHORT )
      stream << wxString::Format(wxT("rect %i %i %i %i; "), m_rect.x, m_rect.y, m_rect.width, m_rect.height );
    else
      stream << wxString::Format(wxT("\n  rect %i %i %i %i"), m_rect.x, m_rect.y, m_rect.width, m_rect.height );
  }
}


wxRect ElementBase::iget_rect() const
{
  wxRect r = m_rect;
  if( !(m_has & E_HAS_RECT) )
  {
    const ElementBase *parent = wxGetApp().hudfile()->get_parent( this, E_HAS_RECT );
    if( parent == 0 ) r = E_RECT_DEFAULT;
    else r = parent->iget_rect();
  }
  return r; 
}


bool ElementBase::is_rendered() const
{
  if( m_flags & E_DRAWNEVER )
    return false;
  if( m_flags & E_DRAWALWAYS )
    return true;
  return is_enabled();
}

#include "elementsctrlbase.h"
bool ElementBase::is_selected() const
{
  return wxGetApp().elementsctrl()->is_selected(this);
}


void ElementBase::convert( int fromarx, int fromary, int arx, int ary, bool size, bool keepwindow, bool fontsize)
{
  double ratio = (fromarx/(double)fromary)/(arx/(double)ary);
  int width = wxGetApp().mainframe()->displayctrl()->width();
  if( m_has & E_HAS_RECT )
  {
    wxRect r = m_rect;
    if( size )
      r.width *= ratio;
    if( keepwindow )
    {
      r.x *= ratio;
      int neww = width/ratio;
      r.x += (neww - width)/2.0;
    }
    else
    { // stretch positions to correct place
      // if completely on left half of screen we make sure left side will be as before (already is)
      // if completely on right half of screen we move it to the right the amount we cut off the right side
      // if on left _and_ right side we restore the width (TODO maybe also restore width/height ratio?)
      if( m_rect.x > width/2.0 )
        r.x += m_rect.width - r.width;
      else if( m_rect.x + m_rect.width > width/2.0 )
      { // on both sides
        r.width = m_rect.width;
      }
    }
    m_rect = r;
  }
}
