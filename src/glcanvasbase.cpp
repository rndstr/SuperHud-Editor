#include "glcanvasbase.h"

#include "common.h"


BEGIN_EVENT_TABLE(GLCanvasBase, wxGLCanvas)
    EVT_PAINT    (GLCanvasBase::OnPaint)
END_EVENT_TABLE()
 
GLCanvasBase::GLCanvasBase(wxWindow *parent) :
  wxGLCanvas(parent, wxID_ANY,  0),//wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas")){
  m_initialized(false)  
{
}
 
 
void GLCanvasBase::OnPaint( wxPaintEvent& )
{
  if( !wxGetApp().is_ready() )
    return;

  render();
}



 
 