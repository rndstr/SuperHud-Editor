#include "displayctrlbase.h"
#include "common.h"
#include "prefs.h"
#include "hudfilebase.h"
#include "elementsctrlbase.h"
#include "texture.h"
#include "pakmanager.h"

#include <cmath>

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
    m_initialized(false),
    m_texdefault(0),
    m_texmodel(0)
{
}


void DisplayCtrlBase::cleanup()
{
  if( m_texdefault )
    wxDELETE(m_texdefault);
  if( m_texmodel )
    wxDELETE(m_texmodel);
}


void DisplayCtrlBase::OnPaint( wxPaintEvent& )
{
  if( !wxGetApp().is_ready() )
    return;

  wxPaintDC dc(this);
  PrepareDC(dc);

#ifndef __WXMOTIF__
  if (!GetContext()) return;
#endif

  SetCurrent();
  if( !m_initialized )
    init();
    
  render();

  glFlush();
  SwapBuffers();
}

void DisplayCtrlBase::init()
{
  wxLogDebug(wxT("DisplayCtrlBase::init"));

  reset_projection_mode();
  load_background();

  // load default texture
  m_texdefault = new Texture( wxT("texture/default.tga"), PM_SEARCH_APPFILE );
  m_texmodel = new Texture( wxT("texture/model.tga"), PM_SEARCH_APPFILE );

  m_initialized = true;
}

IFont* DisplayCtrlBase::font( const wxString& name )
{
  it_fonts f = m_fonts.find(name);
  if( f == m_fonts.end() )
    return 0;
  return f->second;
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
    if( (*cit)->is_rendered() && (*cit)->iget_rect().Contains( p.x, p.y ) )
#else // 2.7 2.6
    if( (*cit)->is_rendered() && (*cit)->iget_rect().Inside( p.x, p.y ) )
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
  /// how far we moved the elements so far
  static Vec2 moved(0,0);

  wxPoint clientpos = DisplayCtrlBase::panel_to_hud(ev.GetPosition());
  wxGetApp().mainframe()->statusbar()->SetStatusText(wxString::Format(wxT("(%i,%i)"), clientpos.x, clientpos.y), SB_MOUSEPOS);

  // capture focus (to enable moving with keys)
  if( ev.ButtonDown() )
    SetFocus();

  if( ev.ControlDown() )
  {
    if( ev.LeftDown() )
    {
      ElementBase *el = element_hittest(clientpos, false);
      if( el ) // toggle selection
        wxGetApp().elementsctrl()->select_item(el, !el->is_selected());
    }
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
        if( abs(m_drag_start.x - clientpos.x) <= Prefs::get().var(wxT("view_dragthreshold")).intval() &&
            abs(m_drag_start.y - clientpos.y) <= Prefs::get().var(wxT("view_dragthreshold")).intval() )
          return;
        m_drag_mode = DRAG_DRAGGING;
        // fix m_drag_start to point lefttop
        if( ~m_drag_el->has() & E_HAS_RECT )
        {
          // if we drag it around then we modify the inherited value...
          // hence we copy the inherited rect and set it as overwriting.
          elements_type& els = wxGetApp().elementsctrl()->selected_elements();
          for( it_elements it = els.begin(); it != els.end(); ++it )
          {
            (*it)->set_rect((*it)->iget_rect());
            (*it)->add_has(E_HAS_RECT);
          }
          //m_drag_el->set_rect(m_drag_el->iget_rect());
          //m_drag_el->add_has(E_HAS_RECT);
          // update properties
          // FIXME that isn't entirely true.. it's rather OnElementPropertiesChanged but it does the trick for now :x
          wxGetApp().mainframe()->OnElementSelectionChanged();
        }
      }
      else if( m_drag_mode = DRAG_DRAGGING )
      {
        //m_drag_el->move_to( m_dragelpos + (clientpos - m_drag_start) );
        // how much moved so far = clientpos - m_drag_start;
        elements_type& els = wxGetApp().elementsctrl()->selected_elements();
        Vec2 move(clientpos - m_drag_start);
        for( it_elements it = els.begin(); it != els.end(); ++it )
        {
          // first move back to initial
          (*it)->move(-moved);
          // move to new position
          (*it)->move(move);
        }
        // now check if we have some snapping
        Vec2 snapels = snap_to_elements();
        Vec2 snapgrid = snap_to_grid();

        // hmm, we pick the one that is not 0 and closer
        Vec2 snap;
        if( snapels == Vec2(0,0) )
          snap = snapgrid;
        else if( snapgrid == Vec2(0,0) )
          snap = snapels;
        else
        { // pick closer
          double lgrid = snapgrid.x*snapgrid.x + snapgrid.y*snapgrid.y;
          double lels = snapels.x*snapels.x + snapels.y*snapels.y;
          snap = (lgrid < lels ? snapgrid : snapels);
        }
        if( snap != Vec2(0,0) )
        {
          for( it_elements it = els.begin(); it != els.end(); ++it )
            (*it)->move(snap);
          move += snap;
        }
        moved = move;
        wxGetApp().mainframe()->OnElementSelectionChanged();
      }
    }
    else if( ev.LeftUp() )
    {
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
      { // drop!
        /*
        // NOTE that this (probably?) isn't necessary as we will have received a OnMouse move event previously
        elements_type& els = wxGetApp().elementsctrl()->selected_elements();
        wxPoint move(clientpos - m_drag_start);
        for( it_elements it = els.begin(); it != els.end(); ++it )
        {
          // first move back to initial
          (*it)->move(-moved);
          // move to new position
          (*it)->move(move);
        }
        wxGetApp().mainframe()->OnElementSelectionChanged();
        */
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
      moved = Vec2(0,0);
    }
  } // if ControlDown
}

Vec2 DisplayCtrlBase::snap_to_grid() const
{
  if( !Prefs::get().var(wxT("view_snapgrid")).boolval() )
    return Vec2(0,0);

  wxASSERT_MSG( m_drag_el != 0, wxT("need a valid element") );

  const int gx = Prefs::get().var(wxT("view_gridX"));
  const int gy = Prefs::get().var(wxT("view_gridY"));
  const int threshold = Prefs::get().var(wxT("view_snapthreshold"));

  const wxRect elr = m_drag_el->iget_rect();
  Vec2 p;

  if( (elr.x % gx) < threshold )
    p.x = (int)std::floor(elr.x/(float)gx)*gx - elr.x;
  else if( (elr.x % gx) > gx-threshold )
    p.x = (int)std::ceil(elr.x/(float)gx)*gx - elr.x;

  if( (elr.y % gy) < threshold )
    p.y = (int)std::floor(elr.y/(float)gy)*gy - elr.y;
  else if( (elr.y % gy) > gy-threshold )
    p.y = (int)std::ceil(elr.y/(float)gy)*gy - elr.y;

  return p;
}

Vec2 DisplayCtrlBase::snap_to_elements() const
{
  if( !Prefs::get().var(wxT("view_snapelements")).boolval() )
    return Vec2(0,0);

  wxASSERT_MSG( m_drag_el != 0, wxT("need a valid element") );

  // this element is already moved to real position
  const wxRect elr = m_drag_el->iget_rect();
  // rectangle we snap
  wxRect r = elr;

  // assert( r now has the real position where it is currently )
  const int threshold = Prefs::get().var(wxT("view_snapthreshold"));
  const elements_type& els = wxGetApp().hudfile()->elements();
  const elements_type& sels = wxGetApp().elementsctrl()->selected_elements();
  wxRect itr; // rectangle to be checked against
  for( cit_elements cit = els.begin(); cit != els.end(); ++cit )
  {
    if( m_drag_el == *cit )
    { // self exclusion for screen boundary
      itr.x = 0;
      itr.y = 0;
      itr.width = width();
      itr.height = height();
    }
    else
    {
      // skip other selected elements
      if( std::find(sels.begin(), sels.end(), *cit) != sels.end() )
      {
        wxLogDebug(wxT("ISSEL ") + (*cit)->name());
        continue;
      }
      if( !(*cit)->is_rendered() )
        continue;
      itr = (*cit)->iget_rect();
    }

 /*
         top  
       _|_ _|_
  left  |   |  right
       _|_ _|_
        |   |
        bottom
    */
    // has an X value which is on both rectangles
    bool xx = (r.GetBottom() > itr.GetTop() && r.GetTop() < itr.GetBottom());
    // has an Y value which is on both rectangles
    bool yy = (r.GetRight() > itr.GetLeft() && r.GetLeft() < itr.GetRight());
    

    if( !xx && !yy )
      continue;
    
    if( xx )
    {
      enum { LL = 0, LR = 1, RR = 2, RL = 3};
      // first char is the huditem to snap, second is the foreign huditem.
      int d[4] = {
        abs(r.GetLeft() - itr.GetLeft()),
        abs(r.GetLeft() - itr.GetRight()),
        abs(r.GetRight() - itr.GetRight()),
        abs(r.GetRight() - itr.GetLeft())
      };
      int smallest_idx = -1;
      int smallest = threshold+1;
      for( int i=0; i<4; ++i )
        if( d[i] < smallest )
        {
          smallest_idx = i;
          smallest = d[i];
        }
      switch( smallest_idx )
      {
      case LL: r.x = itr.GetLeft(); break;
      case LR: r.x = itr.GetRight() + 1; break;
      case RR: r.x = itr.GetRight() - r.GetWidth() + 1; break;
      case RL: r.x = itr.GetLeft() - r.GetWidth(); break;
      }
    }
    if( yy )
    {
      enum { TT = 0, TB = 1, BB = 2, BT = 3};
      // first char is the huditem to snap, second is the foreign huditem.
      int d[4] = {
        abs(r.GetTop() - itr.GetTop()),
        abs(r.GetTop() - itr.GetBottom()),
        abs(r.GetBottom() - itr.GetBottom()),
        abs(r.GetBottom() - itr.GetTop())
      };
      int smallest_idx = -1;
      int smallest = threshold+1;
      for( int i=0; i<4; ++i )
        if( d[i] < smallest )
        {
          smallest_idx = i;
          smallest = d[i];
        }
      switch( smallest_idx )
      {
      case TT: r.y = itr.GetTop(); break;
      case TB: r.y = itr.GetBottom() + 1; break;
      case BB: r.y = itr.GetBottom() - r.GetHeight()+1; break;
      case BT: r.y = itr.GetTop() - r.GetHeight(); break;
      }
    }
  }
  /*
  if( ev.ControlDown() )
  { // lock to x/y axis of m_drag_itempt
    if( abs( r.x - m_drag_itempt.x ) < abs( r.y - m_drag_itempt.y ) )
      r.x = m_drag_itempt.x;
    else
      r.y = m_drag_itempt.y;
  }
  */
  return r.GetPosition() - elr.GetPosition();
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
  int hudw = width(), hudh = height();
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
    m_hudrect.SetLeft((int)-empty);
    m_hudrect.SetRight(hudw + (int)empty);
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
    m_hudrect.SetTop((int)-empty);
    m_hudrect.SetBottom(hudh+(int)empty);
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
    she::draw_rect(rect);

    Prefs::get().var(wxT("view_helper_border_selected")).colorval().glBind();
  glBegin( GL_LINE_LOOP );
    glVertex2i( rect.GetLeft(), rect.GetBottom() );
    glVertex2i( rect.GetRight(), rect.GetBottom() );
    glVertex2i( rect.GetRight(), rect.GetTop() );
    glVertex2i( rect.GetLeft(), rect.GetTop() );
  glEnd();
  }
  else
  {
    Prefs::get().var(wxT("view_helper_fill")).colorval().glBind();
    she::draw_rect(rect);

    Prefs::get().var(wxT("view_helper_border")).colorval().glBind();
  glBegin( GL_LINE_LOOP );
    glVertex2i( rect.GetLeft(), rect.GetBottom() );
    glVertex2i( rect.GetRight(), rect.GetBottom() );
    glVertex2i( rect.GetRight(), rect.GetTop() );
    glVertex2i( rect.GetLeft(), rect.GetTop() );
  glEnd();
  }
  glEnable(GL_TEXTURE_2D);
}
