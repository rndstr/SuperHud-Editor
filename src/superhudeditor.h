#ifndef __SUPERHUDEDITOR_H__
#define __SUPERHUDEDITOR_H__

#include <wx/app.h>

const wxString APP_NAME = wxT("superhudeditor");
const wxString APP_CAPTION = APP_NAME;

class SHEApp : public wxApp
{

  public:
    bool OnInit();
};
DECLARE_APP(SHEApp)


#endif // __SUPERHUDEDITOR_H__

