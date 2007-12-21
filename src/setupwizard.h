#ifndef __SETUPWIZARD_H__
#define __SETUPWIZARD_H__

#include <wx/wizard.h>

#include <wx/textctrl.h>
class GameDirPage : public wxWizardPageSimple
{
public:
    GameDirPage(wxWizard *parent);

    virtual bool TransferDataFromWindow();

    wxString  gamedir() const { return m_gamedir->GetValue(); }
   
protected:
  void OnBtnSelectGameDir( wxCommandEvent& );

private:
  wxTextCtrl *m_gamedir;
  DECLARE_EVENT_TABLE()
};

class SetupWizard : public wxWizard
{
  public:
    SetupWizard( wxFrame *frame );

    wxWizardPage *GetFirstPage() const { return m_two; }

    wxString    gamedir() const { return m_two->gamedir(); }

  private:
    wxWizardPageSimple  *m_one;
    GameDirPage         *m_two;
};



#endif // __SETUPWIZARD_H__
