#include "displayctrl.h"

#include "../common.h"
#include "../pakmanagerbase.h"


void CPMADisplayCtrl::init()
{
  
  
}

void CPMADisplayCtrl::load_background()
{
  if( m_background )
    wxDELETE(m_background);
  wxString bgfpath = Prefs::get().q3_background;
  if( bgfpath.empty() )
    bgfpath = wxT("data/cpma/imgs/background.jpg");
  m_background = new Texture(bgfpath, PM_SEARCH_APPFILE, true);
}

void CPMADisplayCtrl::render()
{
  wxGLCanvas::SetCurrent();
  wxPaintDC(this);

  glClearColor(0.f, 0.f, 0.f, 0.f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  prepare2d();


  if( !m_background )
    load_background();

  m_background->glBind();
  // background
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
  
  if( Prefs::get().grid )
  {
    glDisable(GL_TEXTURE_2D);
    // grid
    Prefs::get().grid_color.glBind();
    glBegin(GL_POINTS);
    for( int x=0; x < WIDTH; x += Prefs::get().grid_x )
      for( int y=0; y < HEIGHT; y += Prefs::get().grid_y )
        glVertex2i(x, y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }

  glFlush();
  SwapBuffers();
}

void CPMADisplayCtrl::cleanup()
{
  wxDELETE(m_background);
}
