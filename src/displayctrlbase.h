#ifndef __DISPLAYCTRLBASE_H__
#define __DISPLAYCTRLBASE_H__

#include "glcanvasbase.h"
#include "texture.h"

class DisplayCtrlBase : public wxGLCanvas
{

  public:
    DisplayCtrlBase( wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~DisplayCtrlBase();
        

    

    virtual void init() {}
    virtual void cleanup() {}

    static void draw_rect( const wxRect& r, bool texcoords =false );
    

    // events
    
  protected:
    bool      m_initialized;
    Texture   *m_background;
    wxRect    m_hudrect;

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

