#include "elementbase.h"
#include "superhudeditor.h"
#include "hudfilebase.h"
#include "displayctrlbase.h"

#include <wx/tokenzr.h>


wxString ElementBase::type2string( int type )
{
  switch(type)
  {
  case E_T_UNKNOWN: return wxT("UNKNOWN");
  case E_T_USERTEXT: return wxT("USERTEXT");
  case E_T_TEXT: return wxT("TEXT");
  case E_T_ICON: return wxT("ICON");
  case E_T_USERICON: return wxT("USERICON");
  case E_T_BAR: return wxT("BAR");
  case E_T_WEAPONLIST: return wxT("WEAPONLIST");
  default:
    break;
  }
  return wxT("WTF?!");
}

bool ElementBase::parse_property( const wxString& cmd, wxString args )
{
  if (cmd.CmpNoCase(wxT("rect")) == 0)
  {
    long i;
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.x = (int)i;
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.y = (int)i;
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.width = (int)i;
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.height = (int)i;
    m_has |= E_HAS_RECT;
  }
  else if (cmd.CmpNoCase(wxT("position")) == 0)
  {
    long i;
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.x = (int)i;
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.y = (int)i;
    m_has |= E_HAS_POS;
  }
  else if (cmd.CmpNoCase(wxT("dimensions")) == 0)
  {
    long i;
    wxStringTokenizer targ( args, HF_PROPERTY_ARG_DELIM );
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.width = (int)i;
    if( targ.GetNextToken().ToLong(&i) )
      m_rect.height = (int)i;
    m_has |= E_HAS_DIM;
  }
  else
    return false;

  return true;
}
void ElementBase::copy_from( const ElementBase * const el )
{ 
  // NOTE we do not copy rectangle and whether enabled/disabled
  wxLogDebug(wxT("COPY FROM %s TO %s"), el->name().c_str(), name().c_str()); 
  int prevhas = m_has;
  m_has = el->has();
//  add_has( E_HAS_RECT, (prevhas & E_HAS_RECT) != 0 ); 
  add_has( E_HAS_POS, (prevhas & E_HAS_POS) != 0 ); 
  add_has( E_HAS_DIM, (prevhas & E_HAS_DIM) != 0 ); 

  //m_enabled = el->is_enabled();
  // we do not copy rectangle... would just confuse
  //m_rect.width = el->rect().width;
  //m_rect.height = el->rect().height;
}
void ElementBase::reset()
{ 
  // NOTE we do not reset rectangle and whether enabled/disabled
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

Vec2 ElementBase::iget_pos() const
{
  Vec2 p;
  p.x = m_rect.x;
  p.y = m_rect.y;
  if( !(m_has & E_HAS_POS) )
  {
    const ElementBase *parent = wxGetApp().hudfile()->get_parent( this, E_HAS_POS );
    if( parent == 0 ) { p.x = E_RECT_DEFAULT.x; p.y = E_RECT_DEFAULT.y; }
    else p = parent->iget_pos();
  }
  return p; 
}
Vec2 ElementBase::iget_dim() const
{
  Vec2 d;
  d.x = m_rect.width;
  d.y = m_rect.height;
  if( !(m_has & E_HAS_DIM) )
  {
    const ElementBase *parent = wxGetApp().hudfile()->get_parent( this, E_HAS_DIM );
    if( parent == 0 ) { d.x = E_RECT_DEFAULT.width; d.y = E_RECT_DEFAULT.height; }
    else d = parent->iget_dim();
  }
  return d; 
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


void ElementBase::convert( double from, double to, bool size, bool stretchposition, bool fontsize)
{
  double ratio = from/to;
  int width = wxGetApp().mainframe()->displayctrl()->width();
  if( m_has & E_HAS_RECT )
  {
    wxRect r = m_rect;
    if( size )
      r.width = (int)(r.width*ratio);
    if( stretchposition )
    { // as we resized the elements, those that leaned against right border/other elements
      // won't anymore. so we will move them to the right the amount we resized (as we did not change position while resizing)

      // if completely on left half of screen we make sure left side will be as before (already is)
      // if completely on right half of screen we move it to the right the amount we cut off the right side
      // if on left _and_ right side we restore the width (TODO maybe also restore width/height ratio?)

      // this is an invertible transformation
      if( m_rect.x > width/2.0 ) // element is on right hand side
        r.x += m_rect.width - r.width;
      else if( m_rect.x + m_rect.width > width/2.0 ) // element crosses middleline
        r.width = m_rect.width;
    }
    else
    { // the position wasn't touched so far and the user chose not to stretch the boundaries to 
      // fill the new area but to move it to the center.
      // i.e. if we go from 4:3 to 16:10 there will be empty space left&right after this

      // 4:3 to 16:10 = scale * translate
      // 16:10 to 4:3 = translate * scale
      // -> not invertible

      // move to center
      if( ratio < 1.0 )
      {
        // scale
        r.x = (int)(r.x*ratio);
        // transform
        int neww = (int)(width/ratio);
        r.x += (int)((neww - width)/2.0);
      }
      else
      {
        // transform
        int neww = (int)(width*ratio);
        r.x -= (int)((neww - width)/2.0);
        // scale
        r.x = (int)(r.x*ratio);
      }
    }
    m_rect = r;
  }
}

