// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "displayctrl.h"

#include "../common.h"
#include "../pakmanager.h"
#include "../model.h"
#include "../hudfilebase.h"
#include "../prefs.h"
#include "../elementsctrlbase.h"
#include "../font.h"
#include "../texture.h"
#include "../model.h"

#include <algorithm>

CPMADisplayCtrl::CPMADisplayCtrl( wxWindow *parent ) :
  DisplayCtrlBase(parent)
{
  
}

CPMADisplayCtrl::~CPMADisplayCtrl()
{
  cleanup();
}

void CPMADisplayCtrl::init()
{
  DisplayCtrlBase::init();

  // set up fonts
  m_fonts[wxT("cpma")] = new CPMAFont(wxT("cpma"));
  m_fonts[wxT("id")] = new CPMAFont(wxT("id"));
  m_fonts[wxT("threewave")] = new CPMAFont(wxT("threewave"));
  m_fonts[wxT("idblock")] = new CPMAFont(wxT("idblock"));
  m_fonts[wxT("sansman")] = new CPMAFont(wxT("sansman"));

  // we could enumerate during startup but initialization order is not working out
  /*
  pakbrowser_dirs_type dirs;
  pakbrowser_files_type files;
  PakManager::get().enumerate_pakdircontents(wxT("/fonts/"), &dirs, &files);
  for( pakbrowser_files_type::const_iterator it = files.begin(); it != files.end(); ++it )
  {
    if( it->first.Matches(wxT("*.tga")) )
    {
      wxString name = it->first.SubString(0, it->first.length()-5);
      if( m_fonts.find(name) == m_fonts.end() )
        m_fonts[name] = new CPMAFont(name);
    }
  }
  */

  // load fonts
  for( fonts_type::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it )
    if( !it->second->load() )
      wxLogError( _("Cannot find font: ") + it->first + wxT("\n\n") + 
      _("Either the game directory is wrong or you have an outdated CPMA version installed,\nmake sure you got at least 1.44") );

  PakManager::get().debug();
}

void CPMADisplayCtrl::load_background()
{
  if( m_background )
    wxDELETE(m_background);
  wxString sbg = Prefs::get().var(wxT("q3_background"));
  if( sbg.empty() )
    sbg = wxT("cpma/texture/background.jpg");
  m_background = new Texture(sbg, PM_SEARCH_APPFILE, true);
}

void CPMADisplayCtrl::OnIdle( wxIdleEvent& )
{
}



// return true if left should be higher than right (ASC)
bool render_sort( ElementBase *a, ElementBase *b )
{
  if( (a->flags() & E_DRAWFRONT) && !(b->flags() & E_DRAWFRONT) )
    return true;
  if( !(a->flags() & E_DRAWBACK) && (b->flags() & E_DRAWBACK) )
    return true;
  //if( wxGetApp().elementsctrl()->is_selected(a) && !wxGetApp().elementsctrl()->is_selected(b) )
  //  return true;
  return false;
}

void CPMADisplayCtrl::render()
{
  if( !IsShown() ) return;
  // that this is not yet ready only happens on wxGTK
  if( !wxGetApp().hudfile() ) return; 
  
  //wxLogDebug(wxT("CPMADisplayCtrl::render"));

  if( m_fish )
  {
    DisplayCtrlBase::render();
    return;
  }
 
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // FIXME what if load_background fails... will it attempt to load every frame? ;) amg
  //if( !m_background )
//      load_background();

  glEnable(GL_TEXTURE_2D);
  m_background->glBind();
  // background
  glColor4f(1.f, 1.f, 1.f, 1.f);
  she::draw_rect(wxRect(0, 0, WIDTH, HEIGHT), true);
  glDisable(GL_TEXTURE_2D);
  /*
  glBegin(GL_QUADS);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glTexCoord2f(0.f, 0.f);
    glVertex2i(0, 0);
    glTexCoord2f(1.f, 0.f);
    glVertex2i(WIDTH, 0);
    glTexCoord2f(1.f, 1.f);
    glVertex2i(WIDTH, HEIGHT);
    glTexCoord2f(0.f, 1.f);
    glVertex2i(0, HEIGHT);
  glEnd();
  */
  
  if( Prefs::get().var(wxT("view_grid")) )
  {
    // grid
    Prefs::get().var(wxT("view_gridcolor")).cval().glBind();
    glBegin(GL_POINTS);
    for( int x=0; x < WIDTH; x += Prefs::get().var(wxT("view_gridX")).ival() )
      for( int y=0; y < HEIGHT; y += Prefs::get().var(wxT("view_gridY")).ival() )
        glVertex2i(x, y);
    glEnd();
  }
  // draw itemz0r
  elements_type els = wxGetApp().hudfile()->elements();
  // sort by: PreDecorate>Other>PostDecorate, Selected>NotSelected
  std::sort(els.begin(), els.end(), render_sort);
  for( cit_elements cit  = els.begin(); cit != els.end(); ++cit )
  {
    if( !(*cit)->is_rendered() )
      continue;
    (*cit)->prerender();
    (*cit)->render();
  }
  // draw nonselected
  for( cit_elements cit  = els.begin(); cit != els.end(); ++cit )
  {
    if( !(*cit)->is_rendered() || (*cit)->is_selected() )
      continue;
    render_helper( (*cit)->iget_hudrect(), false );
  }
  // draw selected
  for( cit_elements cit  = els.begin(); cit != els.end(); ++cit )
  {
    if( !(*cit)->is_rendered() || !(*cit)->is_selected() )
      continue;
    render_helper( (*cit)->iget_hudrect(), true );
  }
}

#include <iostream>
void CPMADisplayCtrl::cleanup()
{
  DisplayCtrlBase::cleanup();

  wxLogDebug(wxT("CPMADisplayCtrl::cleanup"));
  if( m_background )
    wxDELETE(m_background);
  for( fonts_type::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it )
    delete it->second;
  m_fonts.clear();
}

