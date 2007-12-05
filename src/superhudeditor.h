#ifndef __SUPERHUDEDITOR_H__
#define __SUPERHUDEDITOR_H__

#include <wx/app.h>

const wxString APP_NAME = wxT("superhudeditor");
const wxString APP_CAPTION = APP_NAME;

enum
{
	IDM_GAME_CPMA = wxID_HIGHEST+1,
	IDM_GAME_Q4MAX
};

class SHEApp : public wxApp
{

  public:
    bool OnInit();
};
DECLARE_APP(SHEApp)


#endif // __SUPERHUDEDITOR_H__

