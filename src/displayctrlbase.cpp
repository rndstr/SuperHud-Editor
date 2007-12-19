#include "displayctrlbase.h"
#include "common.h"

BEGIN_EVENT_TABLE(DisplayCtrlBase, GLCanvasBase)
  EVT_MOTION(DisplayCtrlBase::OnMotion)
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
  double aspect = 4.0/3.0;
  double w = (double)GetSize().x;
  double h = (double)GetSize().y;
  GLdouble hudw = width(), hudh = height();
  double aspectcorrect = (w/h)/aspect;
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
