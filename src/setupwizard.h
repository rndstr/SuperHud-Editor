#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include "common.h"

#include <wx/wizard.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

class GameDirPage : public wxWizardPageSimple
{
public:
    GameDirPage(wxWizard *parent);

    virtual bool TransferDataFromWindow();

    wxString  gamedir() const { return m_gamedir->GetValue(); }
   
protected:
  void OnBtnSelectGameDir( wxCommandEvent& );

private:
  wxTextCtrl    *m_gamedir;
  DECLARE_EVENT_TABLE()
};

class DisplayPage : public wxWizardPageSimple
{
  public:
    DisplayPage( wxWizard *parent );

    wxString aspectratio() const;
  private:
    wxComboBox  *m_aspectratio;
};

class SetupWizard : public wxWizard
{
  public:
    SetupWizard( wxFrame *frame );

    wxWizardPage *GetFirstPage() const { return m_two; }

    wxString    gamedir() const { return m_two->gamedir(); }
    wxString    aspectratio() const { return m_three->aspectratio(); }

  private:
    wxWizardPageSimple  *m_one;
    GameDirPage         *m_two;
    DisplayPage         *m_three;
};



#endif // SETUPWIZARD_H
