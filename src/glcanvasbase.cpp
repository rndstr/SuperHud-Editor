#include "glcanvasbase.h"

#include "wx/wx.h"


BEGIN_EVENT_TABLE(GLCanvasBase, wxGLCanvas)
    EVT_PAINT    (GLCanvasBase::OnPaint)
END_EVENT_TABLE()
 
GLCanvasBase::GLCanvasBase(wxWindow *parent)
:wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas")){
    int argc = 0;
    char** argv = NULL;
}
 
 
void GLCanvasBase::OnPaint(wxPaintEvent& WXUNUSED(event)){
    Render();
}
 
void GLCanvasBase::Render()
{
    SetCurrent();
    wxPaintDC(this);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);
 
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(-0.5, -0.5);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
        glColor3f(0.4, 0.5, 0.4);
        glVertex2f(0.0, -0.8);
    glEnd();
 
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.1, 0.1);
        glVertex2f(-0.1, 0.1);
        glVertex2f(-0.1, -0.1);
        glVertex2f(0.1, -0.1);
    glEnd();
 
    /*
// using a little of glut
    glColor4f(0,0,1,1);
    glutWireTeapot(0.4);
 
    glLoadIdentity();
    glColor4f(2,0,1,1);
    glutWireTeapot(0.6);
// done using glut
*/
 
    glFlush();
    SwapBuffers();
}
 