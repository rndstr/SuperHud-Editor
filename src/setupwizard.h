// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
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



#endif
