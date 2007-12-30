#include "displayctrlbase.h"
#include "common.h"
#include "prefs.h"

BEGIN_EVENT_TABLE(DisplayCtrlBase, wxGLCanvas)
  EVT_IDLE(DisplayCtrlBase::OnIdle)
  EVT_SIZE(DisplayCtrlBase::OnSize)
  EVT_ERASE_BACKGROUND(DisplayCtrlBase::OnEraseBackground)
  EVT_MOUSE_EVENTS(DisplayCtrlBase::OnMouse)
  EVT_PAINT(DisplayCtrlBase::OnPaint)
END_EVENT_TABLE()


DisplayCtrlBase::DisplayCtrlBase( wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
    wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE),
    m_background(0),
    m_initialized(false)
{
}

DisplayCtrlBase::~DisplayCtrlBase()
{
}

void DisplayCtrlBase::OnPaint( wxPaintEvent& )
{
  //wxLogDebug(wxT("DisplayCtrlBase::OnPaint"));
  // must always be here
  wxPaintDC dc(this);

#ifndef __WXMOTIF__
  if (!GetContext()) return;
#endif

  SetCurrent();
  if( !m_initialized )
  {
    reset_projection_mode();
    m_initialized = true;
  }

  render();

  glFlush();
  SwapBuffers();
}

void DisplayCtrlBase::OnEraseBackground( wxEraseEvent& )
{
  // do nothing to avoid flashing on MSW
}

void DisplayCtrlBase::OnSize( wxSizeEvent& ev )
{
  // this is also necessary to update the context on some platforms
  wxGLCanvas::OnSize(ev);
  // Reset the OpenGL view aspect
  reset_projection_mode();
}

void DisplayCtrlBase::OnIdle( wxIdleEvent& )
{

}

void DisplayCtrlBase::OnMouse( wxMouseEvent& ev )
{
  wxPoint clientpos = DisplayCtrlBase::panel_to_hud(ev.GetPosition());
  wxGetApp().mainframe()->statusbar()->SetStatusText(wxString::Format(wxT("(%i,%i)"), clientpos.x, clientpos.y), SB_MOUSEPOS);

}

void DisplayCtrlBase::reset_projection_mode()
{
  if( wxGetApp().mainframe() &&  wxGetApp().mainframe()->model() )
    prepare3d();
  else
    prepare2d();

}

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
  double aspectcorrect = (w/h)/Prefs::get().var(wxT("view_aspectratio")).floatval();
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

void DisplayCtrlBase::render_helper( const wxRect& rect, bool selected /*= false*/ ) const
{
  if( !Prefs::get().var(wxT("view_helper")) )
    return;
  glDisable(GL_TEXTURE_2D);
  // -- draw helper outline
  if( selected )
  {
    Prefs::get().var(wxT("view_helper_fill_selected")).colorval().glBind();
    draw_rect(rect);

    Prefs::get().var(wxT("view_helper_border_selected")).colorval().glBind();
  glBegin( GL_LINE_LOOP );
    glVertex2i( rect.GetLeft(), rect.GetBottom()+1 );
    glVertex2i( rect.GetRight(), rect.GetBottom()+1 );
    glVertex2i( rect.GetRight(), rect.GetTop()+1 );
    glVertex2i( rect.GetLeft(), rect.GetTop()+1 );
  glEnd();
  }
  else
  {
    Prefs::get().var(wxT("view_helper_fill")).colorval().glBind();
    draw_rect(rect);

    Prefs::get().var(wxT("view_helper_border")).colorval().glBind();
  glBegin( GL_LINE_LOOP );
    glVertex2i( rect.GetLeft(), rect.GetBottom()+1 );
    glVertex2i( rect.GetRight(), rect.GetBottom()+1 );
    glVertex2i( rect.GetRight(), rect.GetTop()+1 );
    glVertex2i( rect.GetLeft(), rect.GetTop()+1 );
  glEnd();
  }
  glEnable(GL_TEXTURE_2D);
}
