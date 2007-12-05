
#include "superhudeditor.h"
#include "mainframe.h"

#include <wx/stdpaths.h>


IMPLEMENT_APP(SHEApp)


bool SHEApp::OnInit()
{
  SetAppName(APP_NAME);
#ifndef WIN32
  #ifdef PACKAGE_PREFIX
    ((wxStandardPaths&)wxStandardPaths::Get()).SetInstallPrefix( wxT( PACKAGE_PREFIX ) );
  #endif
#endif

  wxFrame *frame = new MainFrame(0, wxID_ANY, APP_NAME, wxDefaultPosition, wxSize(800,600));
  SetTopWindow(frame);
  frame->Show();

  return true;
}


