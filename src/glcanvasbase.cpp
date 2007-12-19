#include "glcanvasbase.h"

#include "common.h"


BEGIN_EVENT_TABLE(GLCanvasBase, wxGLCanvas)
    EVT_PAINT    (GLCanvasBase::OnPaint)
END_EVENT_TABLE()
 
GLCanvasBase::GLCanvasBase(wxWindow *parent)
:wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas")){
    int argc = 0;
    char** argv = NULL;
}
 
 
void GLCanvasBase::OnPaint(wxPaintEvent& WXUNUSED(event)){
    render();
}



 
 