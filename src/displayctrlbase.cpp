#include "displayctrlbase.h"
#include "common.h"
#include "prefs.h"
#include "hudfilebase.h"
#include "elementsctrlbase.h"

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

ElementBase* DisplayCtrlBase::element_hittest( const wxPoint& p, bool toggle /*=true*/ )
{
  const elements_type els = wxGetApp().hudfile()->elements();
  elements_type inside;
  for( cit_elements cit = els.begin(); cit != els.end(); ++cit )
  {
#if wxCHECK_VERSION(2,8,0)
    if( (*cit)->is_rendered() && (*cit)->rect().Contains( p.x, p.y ) )
#else // 2.7 2.6
    if( (*cit)->is_rendered() && (*cit)->rect().Inside( p.x, p.y ) )
#endif
      inside.push_back( *cit );
  }

  if( !inside.size() )
    return 0;
  if( inside.size() == 1)
    return inside[0]; // there is only one hence no toggling anyway

  // check if one of them is already selected.
  ElementBase *selected = 0;
  for( cit_elements it = inside.begin(); it != inside.end(); ++it )
  {
    if( (*it)->is_selected() )
    {
      selected = *it;
      if( !toggle )
        return selected;
    }
    else
    {
      if( selected != 0 )
      { // there was one previously selected, next (=current) is an der reihe
        return *it;
      }
    }
  }
  return inside[0];
}

void DisplayCtrlBase::OnMouse( wxMouseEvent& ev )
{
  // upon LeftDown, whether the element was already selected previously!
  // important for LeftUp to know if we have to toggle or not (otherwise 
  // LeftDown selects one and LeftUp toggles even if there wasn't a selection before)
  static bool selected_on_ldown = false; 

  wxPoint clientpos = DisplayCtrlBase::panel_to_hud(ev.GetPosition());
  wxGetApp().mainframe()->statusbar()->SetStatusText(wxString::Format(wxT("(%i,%i)"), clientpos.x, clientpos.y), SB_MOUSEPOS);

  // capture focus (to enable moving with keys)
  if( ev.ButtonDown() )
    SetFocus();

  if( ev.ControlDown() )
  {
    ElementBase *el = element_hittest(clientpos, false);
    if( el ) // toggle selection
      wxGetApp().elementsctrl()->select_item(el, !el->is_selected());
  }
  else // !ControlDown
  {
    if( ev.LeftDown() )
    {
      // if user clicks on an element he already has selected:
      //  - if there are several items on this point, hi == selected item.
      //  - 
      // if not 0 this will be:
      //  a) if no item at clientpos which has already been selected: the first item (from the item vector in Hud) will be returned
      //  b) if an item has already been selected: exactly that one.
      ElementBase *el = element_hittest(clientpos, false);
      if( el )
      {
        
        selected_on_ldown = el->is_selected();
        // only change anything if not yet selected
        if( !selected_on_ldown )
        {
          wxGetApp().elementsctrl()->deselect_all();
          wxGetApp().elementsctrl()->select_item(el);
        }
        // prepare for dragging
        m_drag_el = el; // note that there might be other elements.. we just only store this one as we do the snapping with it
        m_drag_start = clientpos;
        m_drag_elpos = el->iget_rect().GetPosition();
        m_drag_mode = DRAG_START;
        
        /*
        m_drag_itempt = hi->get_rect().GetPosition();
        */
      }
      else
      {
        wxGetApp().elementsctrl()->deselect_all();
        m_drag_el = 0;
        m_drag_mode = DRAG_NONE;
      }
    }
    else if( ev.Dragging() && ev.LeftIsDown() && m_drag_el != 0 )
    {
      if( m_drag_mode == DRAG_START )
      { // initialize
        m_drag_mode = DRAG_DRAGGING;
        // fix m_drag_start to point lefttop
        if( ~m_drag_el->has() & E_HAS_RECT )
        {
          // if we drag it around then we modify the inherited value...
          // hence we copy the inherited rect and set it as overwriting.
          m_drag_el->set_rect(m_drag_el->iget_rect());
          m_drag_el->add_has(E_HAS_RECT);
          // update properties
          // FIXME that isn't entirely true.. it's rather OnElementPropertiesChanged but it does the trick for now :x
          wxGetApp().mainframe()->OnElementSelectionChanged();
        }
      }
      else if( m_drag_mode = DRAG_DRAGGING )
      {
        wxRect r = m_drag_el->rect(); // wxASSERT(same as iget_rect())
        r.SetPosition(clientpos - (m_drag_start - m_drag_elpos));
        m_drag_el->set_rect(r);
        wxGetApp().mainframe()->OnElementSelectionChanged();
      }
    }
    else if( ev.LeftUp() )
    {
      int i=2;
      if( m_drag_mode != DRAG_DRAGGING )
      {
        // only toggle if it was already selected previously
        ElementBase *el = element_hittest( clientpos, selected_on_ldown );
        if( el != 0 && !el->is_selected() )
        {
          wxGetApp().elementsctrl()->deselect_all();
          wxGetApp().elementsctrl()->select_item(el);
        }
      }
      else if( m_drag_mode == DRAG_DRAGGING )
      {
        // move it back to starting position and then issue the whole operation.
        /*
        m_drag_el->m_rect.SetPosition( m_drag_itempt );
        wxGetApp().cmds()->Submit( new MoveToCommand( m_drag_hi, m_drag_realpt ) );

        wxRect r = m_drag_hi->get_client_rect();
        wxGetApp().get_frame()->get_statusbar()->SetStatusText( 
          m_drag_hi->get_name() + wxString::Format(wxT(" @ (%i,%i)-(%i,%i)"), r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom()),
          SB_DRAGDROP);
          */
      }
      m_drag_mode = DRAG_NONE;
      m_drag_el = 0;
    }
  } // if ControlDown
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
