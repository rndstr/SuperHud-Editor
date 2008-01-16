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
