#ifndef __DISPLAYCTRLBASE_H__
#define __DISPLAYCTRLBASE_H__

#include "glcanvasbase.h"
#include "texture.h"

class DisplayCtrlBase : public GLCanvasBase
{

  public:
    DisplayCtrlBase( wxWindow *parent ) :
        GLCanvasBase(parent),
          m_background(0)
      //wxTextCtrl( parent, wxID_ANY, wxT(":k"), wxPoint(0,0), wxSize(100,100), wxNO_BORDER | wxTE_MULTILINE )
    {
    }

    

    virtual void init() {}
    virtual void cleanup() {}
    

    // events
    virtual void OnMotion( wxMouseEvent& );
    virtual void OnIdle( wxIdleEvent& ) {}
  protected:
    Texture   *m_background;
    wxRect    m_hudrect;

    /// width of internal coordinate system
    virtual int width() const = 0;
    /// height of internal coordinate system
    virtual int height() const = 0;
    /// sets up the opengl view to use 2D
    virtual void prepare2d();
    virtual void prepare3d();
    wxPoint panel_to_hud( const wxPoint& p ) const;

    DECLARE_EVENT_TABLE()

};


#endif // __DISPLAYCTRLBASE_H__

