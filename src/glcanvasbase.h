#ifndef __GLCANVASBASE_H__
#define __GLCANVASBASE_H__

#include <wx/glcanvas.h>

class GLCanvasBase : public wxGLCanvas
{
    
  public:
    GLCanvasBase( wxWindow* parent );
    void OnPaint( wxPaintEvent& ev );

    virtual void initgl() {}
    virtual void render() {}

  protected:
    bool    m_initialized;
    

  protected:
    DECLARE_EVENT_TABLE()
};

#endif

