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

class MiscPage : public wxWizardPageSimple
{
  public:
    MiscPage( wxWizard *parent );

    wxString  aspectratio() const;
    bool      checkforupdate() const;
    wxString  proxy() const;
  private:
    wxComboBox  *m_aspectratio;
    wxCheckBox  *m_checkforupdate;
    wxTextCtrl  *m_proxy;
};

class SetupWizard : public wxWizard
{
  public:
    SetupWizard( wxFrame *frame );

    wxWizardPage *GetFirstPage() const { return m_two; }

    const MiscPage*     miscpage() const { return m_three; }
    const GameDirPage*  gamedirpage() const { return m_two; }

  private:
    wxWizardPageSimple  *m_one;
    GameDirPage         *m_two;
    MiscPage            *m_three;
};



#endif // SETUPWIZARD_H
