#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>

class wxPanel;
class wxWindow;
class wxImageList;

class PrefsDialog : public wxPropertySheetDialog
{
  public:
    PrefsDialog( wxWindow *parent );
    ~PrefsDialog();

  protected:
    wxImageList *m_imglist;
    wxPanel* create_display(wxWindow *parent);
};


#endif // PREFSDIALOG_H

