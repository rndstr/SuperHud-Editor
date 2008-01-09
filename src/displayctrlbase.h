#ifndef DISPLAYCTRLBASE_H
#define DISPLAYCTRLBASE_H

#include "font.h"
#include <wx/gdicmn.h>
#include <map>

typedef enum {
  DRAG_NONE = 0,
  DRAG_START = 1,
  DRAG_DRAGGING = 2
} eDragMode;

typedef std::map<wxString, IFont*> fonts_type;
typedef fonts_type::iterator it_fonts;
typedef fonts_type::const_iterator cit_fonts;

typedef wxPoint Vec2;

class Texture;
class ElementBase;

class DisplayCtrlBase : public wxGLCanvas
{

  public:
    DisplayCtrlBase( wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~DisplayCtrlBase() {}
        
    /// this shouldn't be called too early. preferrably in the OnPaint() function
    /// at first call
    virtual void init();
    virtual void cleanup();


    const fonts_type& fonts() const { return m_fonts; }
    IFont*    font( const wxString& name );

    const Texture* const texture_default() const { return m_texdefault; }
    const Texture* const texture_model() const { return m_texmodel; }


    void render_helper( const wxRect& rect, bool selected = false ) const;

    void reset_projection_mode();
    /// width of internal coordinate system
    virtual int width() const = 0;
    /// height of internal coordinate system
    virtual int height() const = 0;

    /// finds the item(s) at that point
    /// @arg p The point where we are looking for items
    /// @arg toggle If true and we click on a point where several items underlay then we switch to next one, otherwise we keep it.
    ElementBase*    element_hittest( const wxPoint& p, bool toggle = true );

  protected:
    bool          m_initialized;
    Texture       *m_background;
    wxRect        m_hudrect;

    ElementBase   *m_drag_el;
    eDragMode     m_drag_mode;
    wxPoint       m_drag_start; ///< mouse position when we started dragging, hudcoords

    fonts_type    m_fonts;

    Texture       *m_texdefault;
    Texture       *m_texmodel; ///< what we display if user selected model (we can't render it (yet) ourselves : ((( 

    virtual void load_background() = 0;
    virtual void OnIdle( wxIdleEvent& );
    void OnSize( wxSizeEvent& );
    void OnEraseBackground( wxEraseEvent& );
    void OnMouse( wxMouseEvent& );
    void OnPaint( wxPaintEvent& );

    /// smallest distance (max view_snapthreshold) to other elements
    /// @returns The 2d vector we have to move the selection to snap or wxPoint(0,0) if it's disabled by prefs 
    Vec2 snap_to_elements() const;

    Vec2 snap_to_grid() const;

    virtual void render() {}
    

    

    

    /// sets up the opengl view to use 2D
    void prepare2d();
    /// sets up the opengl view to use 3D
    void prepare3d();
    
    wxPoint panel_to_hud( const wxPoint& p ) const;

    DECLARE_EVENT_TABLE()

};


#endif // DISPLAYCTRLBASE_H

