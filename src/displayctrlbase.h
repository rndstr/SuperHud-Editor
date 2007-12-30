#ifndef __DISPLAYCTRLBASE_H__
#define __DISPLAYCTRLBASE_H__

#include "glcanvasbase.h"
#include "texture.h"
#include <wx/gdicmn.h>

typedef enum {
  DRAG_NONE = 0,
  DRAG_START = 1,
  DRAG_DRAGGING = 2
} eDragMode;

class ElementBase;

class DisplayCtrlBase : public wxGLCanvas
{

  public:
    DisplayCtrlBase( wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~DisplayCtrlBase();
        
    virtual void init() {}
    virtual void cleanup() {}

    void render_helper( const wxRect& rect, bool selected = false ) const;

    /// finds the item(s) at that point
    /// @arg p The point where we are looking for items
    /// @arg toggle If true and we click on a point where several items underlay then we switch to next one, otherwise we keep it.
    ElementBase*    element_hittest( const wxPoint& p, bool toggle = true );

  protected:
    bool      m_initialized;
    Texture   *m_background;
    wxRect    m_hudrect;

    ElementBase *m_drag_el;
    eDragMode m_drag_mode;
    wxPoint   m_drag_start; ///< mouse position when we started dragging
    wxPoint   m_drag_elpos; //?, element position when we started dragging

    virtual void OnIdle( wxIdleEvent& );
    void OnSize( wxSizeEvent& );
    void OnEraseBackground( wxEraseEvent& );
    void OnMouse( wxMouseEvent& );
    void OnPaint( wxPaintEvent& );

    virtual void render() {}
    

    /// width of internal coordinate system
    virtual int width() const = 0;
    /// height of internal coordinate system
    virtual int height() const = 0;

    void reset_projection_mode();

    /// sets up the opengl view to use 2D
    void prepare2d();
    /// sets up the opengl view to use 3D
    void prepare3d();
    
    wxPoint panel_to_hud( const wxPoint& p ) const;

    DECLARE_EVENT_TABLE()

};


#endif // __DISPLAYCTRLBASE_H__

