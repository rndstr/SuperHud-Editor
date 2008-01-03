#include "displayctrl.h"

#include "../common.h"
#include "../pakmanager.h"
#include "../model.h"
#include "../hudfilebase.h"
#include "../prefs.h"
#include "../elementsctrlbase.h"
#include "../font.h"
#include "../texture.h"

#include <algorithm>

void CPMADisplayCtrl::init()
{
  DisplayCtrlBase::init();
  // load fonts : D
  /*
  m_fonts[wxT("cpma")] = new CPMAFont(wxT("cpma"));
  m_fonts[wxT("id")] = new CPMAFont(wxT("id"));
  m_fonts[wxT("threewave")] = new CPMAFont(wxT("threewave"));
  m_fonts[wxT("sansman")] = new CPMAFont(wxT("sansman"));
  m_fonts[wxT("idblock")] = new CPMAFont(wxT("idblock"));
  */

  // load fonts
  for( fonts_type::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it )
    if( !it->second->load() )
      wxLogError( wxT("Failed loading font ") + it->first );
}

void CPMADisplayCtrl::load_background()
{
  if( m_background )
    wxDELETE(m_background);
  m_background = new Texture(Prefs::get().var(wxT("q3_background")), PM_SEARCH_APPFILE, true);
}

void CPMADisplayCtrl::OnIdle( wxIdleEvent& )
{
  if( !wxGetApp().mainframe() || !wxGetApp().mainframe()->model() )
    return;

  static wxLongLong last = 0;
  if( wxGetLocalTimeMillis() - last > 100 )
  {
    Refresh();
    last = wxGetLocalTimeMillis();
  }
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
  static int count = 0;
  if(!IsShown()) return;
  wxLogDebug(wxT("CPMADisplayCtrl::render"));

 
  if( wxGetApp().mainframe()->model() )
  {
    count += 10;
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -40.f);
    glRotatef(30.f+count, 0.f, 1.f, 0.f);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glDisable(GL_TEXTURE_2D);
    wxGetApp().mainframe()->model()->render();
    glEnable(GL_TEXTURE_2D);

    /*
    glBegin(GL_QUADS);
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
  }
  else
  {

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIXME what if load_background fails... will it attempt to load every frame? ;) amg
    //if( !m_background )
//      load_background();

    m_background->glBind();
    // background
    glColor4f(1.f, 1.f, 1.f, 1.f);
    draw_rect(wxRect(0, 0, WIDTH, HEIGHT), true);
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
      glDisable(GL_TEXTURE_2D);
      // grid
      Prefs::get().var(wxT("grid_color")).colorval().glBind();
      glBegin(GL_POINTS);
      for( int x=0; x < WIDTH; x += Prefs::get().var(wxT("view_gridX")).intval() )
        for( int y=0; y < HEIGHT; y += Prefs::get().var(wxT("view_gridY")).intval() )
          glVertex2i(x, y);
      glEnd();
      glEnable(GL_TEXTURE_2D);
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
      render_helper( (*cit)->iget_rect(), false );
    }
    // draw selected
    for( cit_elements cit  = els.begin(); cit != els.end(); ++cit )
    {
      if( !(*cit)->is_rendered() || !(*cit)->is_selected() )
        continue;
      render_helper( (*cit)->iget_rect(), true );
    }

  }
}

void CPMADisplayCtrl::cleanup()
{
  DisplayCtrlBase::cleanup();

  if( m_background )
    wxDELETE(m_background);
  for( fonts_type::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it )
    delete it->second;
  m_fonts.clear();
}
