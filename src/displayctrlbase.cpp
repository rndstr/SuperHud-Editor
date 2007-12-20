#include "displayctrlbase.h"
#include "common.h"

BEGIN_EVENT_TABLE(DisplayCtrlBase, GLCanvasBase)
  EVT_MOTION(DisplayCtrlBase::OnMotion)
  EVT_IDLE(DisplayCtrlBase::OnIdle)
END_EVENT_TABLE()

void DisplayCtrlBase::prepare2d()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable( GL_CULL_FACE );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  //
  double w = (double)GetSize().x;
  double h = (double)GetSize().y;
  GLdouble hudw = width(), hudh = height();
  double aspectcorrect = (w/h)/Prefs::get().aspectratiod;
  double empty; // how much space left/right or bottom/top has to be empty

  // topleft is 0,0
  if( aspectcorrect > 1.0 )
  { // height will fill the available space (= e.g. 16:9)
    // ___________
    // |   | |   |
    // -----------
    m_hudrect.SetTop(0);
    m_hudrect.SetBottom(hudh);
    empty = hudw*(aspectcorrect-1)/2.0; 
    m_hudrect.SetLeft(-empty);
    m_hudrect.SetRight(hudw + empty);
  }
  else
  { // width will fill the available space (= e.g. 5:4)
    //
    //  --
    // |  |
    // |--|
    // |  |
    // |--|
    // |  |
    //  --
    m_hudrect.SetLeft(0);
    m_hudrect.SetRight(hudw);
    empty = (hudh/aspectcorrect - hudh)/2.0;
    m_hudrect.SetTop(-empty);
    m_hudrect.SetBottom(hudh+empty);
  }

  glViewport(0, 0, (GLint)w, (GLint)h);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  // topleft is (0,0)
  gluOrtho2D( m_hudrect.GetLeft(), m_hudrect.GetRight(), m_hudrect.GetBottom(), m_hudrect.GetTop() );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void DisplayCtrlBase::prepare3d()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
  glClearDepth(1.0f);	// Depth Buffer Setup
  glEnable(GL_DEPTH_TEST); // Enables Depth Testing
  glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glDisable( GL_CULL_FACE );

  glEnable( GL_LIGHTING );
  GLfloat global_ambient[] = { .2f, .2f, .2f, 1.f };
  GLfloat ambient[] = { .1f, .1f, .1f, 1.f };
  GLfloat specular[] = { 1.f, 1.f, 1.f, 1.f, 1.f };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, global_ambient );
  glShadeModel(GL_SMOOTH);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
  glEnable( GL_LIGHT0 );
  GLfloat lightpos[] = {0.f, 0.f, 50.f, 1.f };
  glLightfv( GL_LIGHT0, GL_POSITION, lightpos );

  //glEnable(GL_COLOR_MATERIAL);

  wxRect hudrect(0, 0, GetSize().GetWidth(), GetSize().GetHeight());

  //glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
  glViewport(hudrect.GetLeft(), hudrect.GetTop(), hudrect.GetWidth(), hudrect.GetHeight());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float ratio_w_h = (float)hudrect.GetWidth()/(float)hudrect.GetHeight();
  gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

wxPoint DisplayCtrlBase::panel_to_hud( const wxPoint& p ) const
{
  wxSize s = GetSize();

  wxPoint hud;
  hud.x = static_cast<int>( m_hudrect.GetLeft() + p.x/(double)s.x * m_hudrect.GetWidth() );
  hud.y = static_cast<int>( m_hudrect.GetTop() + p.y/(double)s.y * m_hudrect.GetHeight() );
  return hud;


}

void DisplayCtrlBase::OnMotion( wxMouseEvent& ev )
{
  wxPoint clientpos = DisplayCtrlBase::panel_to_hud(ev.GetPosition());
  //wxPoint clientpos = ev.GetPosition();
  wxGetApp().mainframe()->statusbar()->SetStatusText(wxString::Format(wxT("(%i,%i)"), clientpos.x, clientpos.y), SB_MOUSEPOS);
}
