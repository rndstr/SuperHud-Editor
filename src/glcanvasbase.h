#ifndef __GLCANVASBASE_H__
#define __GLCANVASBASE_H__

#include <wx/glcanvas.h>

class GLCanvasBase : public wxGLCanvas
{
    
  public:
    GLCanvasBase( wxWindow* parent );
    void OnPaint( wxPaintEvent& ev );

  protected:
    virtual void render() {}

  protected:
    DECLARE_EVENT_TABLE()
};

#endif
