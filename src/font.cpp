// Copyright (C) 2006 Roland Schilter <rolansch@student.ethz.ch>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include <wx/log.h>
#include <wx/image.h>
#include <wx/dc.h>
#include <cmath>

#include "font.h"

#include "texture.h"
#include "pakmanager.h"


IFont::IFont( const wxString& name ) :
  m_name( name )
{
}

bool IFont::load()
{
  char *cwbuf = 0;
  size_t cwsize = 0;
  if( !PakManager::get().load( &cwbuf, wxT("fonts/") + m_name + wxT(".cw"), PM_SEARCH_HUDFILE, &cwsize ) )
  { // just fill in default values
    for( int i=0; i < F_COUNT_X * F_COUNT_Y; ++i )
      m_widths[i] = F_CHAR_WIDTH;
  }
  else
  {
    if( cwsize != F_COUNT_X * F_COUNT_Y )
    {
      wxLogWarning( _("Fontwidth file %s.cw has less entries than expected: %i < %i"), m_name.c_str(), cwsize, F_COUNT_X * F_COUNT_Y );
    }
    if( cwsize > F_COUNT_X * F_COUNT_Y )
    {
      wxLogError( _("Fontwidth file %s.cw has more entries than expected: %i < %i"), m_name.c_str(), cwsize, F_COUNT_X * F_COUNT_Y );
      return false;
    }
    memcpy( m_widths, cwbuf, cwsize );
    PakManager::get().cleanup_lastloaded();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////



CPMAFont::CPMAFont( const wxString& name ) :
  IFont( name ),
  m_tex(0)
{
}



void CPMAFont::cleanup()
{
  if( m_tex )
    wxDELETE(m_tex);
}

bool CPMAFont::load()
{
  wxLogDebug(wxT("Loading font: ") + m_name);

  cleanup();
  if( !IFont::load() )
    return false;

  m_tex = new Texture( wxT("fonts/") + m_name + wxT(".tga"), PM_SEARCH_HUDFILE );
  if( !m_tex->is_ok() )
    return false;
  return true;
}

void CPMAFont::print( const wxRect& r, int sizept, const wxString& msg, bool monospace /*=false*/, char textalign /*='L'*/ ) const
{
  print( r, sizept, sizept, msg, monospace, textalign );
}

int CPMAFont::get_width( int sizex, const wxString& msg, bool monospace ) const
{
  if( monospace )
    return static_cast<int>(msg.length()) * sizex;

  int width = 0;
  for( wxString::const_iterator cit = msg.begin(); cit != msg.end(); ++cit )
    width += m_widths[*cit];

  return static_cast<int>(width * sizex/(float)F_CHAR_WIDTH);
}

void CPMAFont::print( const wxRect& r, int sizex, int sizey, const wxString& msg, bool monospace /*=false*/, char textalign /*='L'*/ ) const
{
  if( !m_tex || !m_tex->is_ok() )
  {
    wxLogDebug(wxT("CPMAFont::print - ") + m_name + wxT(" not_ok"));
    return;
  }
  glPushMatrix();
  glLoadIdentity();

  switch( textalign )
  {
  case 'R':
    glTranslated( r.x + r.width - get_width( sizex, msg, monospace ), r.y, 0 );
    break;
  case 'C':
    glTranslated( r.x + r.width/2 - get_width( sizex, msg, monospace )/2, r.y, 0 );
    break;
  case 'L':
  default:
    glTranslated( r.x, r.y, 0 );
    break;
  }
  
  m_tex->glBind();
  
  
  float width_ratio = sizex/(float)F_CHAR_WIDTH;
  float cx, cy;
  

  //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  // looks much better, needs alpha channel
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  float one_div_x = 1/(float)F_COUNT_X;
  float one_div_y = 1/(float)F_COUNT_Y;
  
  float th = one_div_y;
  float tw = one_div_x; // changes if not monospace.
  int height = sizey;
  int width = sizex; // changes if not monospace.

  for( wxString::const_iterator cit = msg.begin(); cit != msg.end(); ++cit )
  {
    if( *cit < 0 || *cit >= F_COUNT_X*F_COUNT_Y ) 
      continue;
    
    
    cx=float(*cit%F_COUNT_Y) * one_div_x;
	  cy=float(*cit/F_COUNT_X) * one_div_y;
    if( !monospace )
    {
      width = static_cast<int>(m_widths[*cit] * width_ratio);
      tw = one_div_x * (m_widths[*cit] / (float)F_CHAR_WIDTH);
    }

    
  glBegin( GL_QUADS );  
    //glTexCoord2f(cx,1-cy-0.0625f);
    glTexCoord2f(cx,cy);
    glVertex2i(0, 0);

    //glTexCoord2f(cx+0.0625f,1-cy-0.0625f);
    glTexCoord2f(cx+tw,cy);
    glVertex2i(width, 0);

    //glTexCoord2f(cx+0.0625f,1-cy);
    glTexCoord2f(cx+tw,cy+th);
    glVertex2i(width, height);
    
    //glTexCoord2f(cx,1-cy);
    glTexCoord2f(cx,cy+th);
    glVertex2i(0, height);
  glEnd();

  // DEBUG
  
  /*
    glDisable( GL_TEXTURE_2D );
   glBegin( GL_LINES );
    glVertex2i( 0, 0 );
    glVertex2i( width, 0 );

    glVertex2i( width, 0 );
    glVertex2i( width, height );

    glVertex2i( width, height );
    glVertex2i( 0, height );

    glVertex2i( 0, height );
    glVertex2i( 0, 0 );
  glEnd();
  glEnable( GL_TEXTURE_2D );
  */
  
    
    glTranslated( width, 0, 0 );  
  }

  //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  
  //glDisable( GL_TEXTURE_2D );
  glPopMatrix();
}

