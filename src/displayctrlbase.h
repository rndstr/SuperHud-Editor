#ifndef __DISPLAYCTRLBASE_H__
#define __DISPLAYCTRLBASE_H__

#include <wx/textctrl.h>

class DisplayCtrlBase : public wxTextCtrl
{
  public:
    DisplayCtrlBase( wxWindow *parent ) :
      wxTextCtrl( parent, wxID_ANY, wxT(":k"), wxPoint(0,0), wxSize(100,100), wxNO_BORDER | wxTE_MULTILINE )
    {
    }

};


#endif // __DISPLAYCTRLBASE_H__

