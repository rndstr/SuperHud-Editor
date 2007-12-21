#include "setupwizard.h"
#include "common.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/file.h>


SetupWizard::SetupWizard( wxFrame *frame ) :
  wxWizard(frame,wxID_ANY,_("Setup wizard"),
                   wxArtProvider::GetBitmap(ART_BMP_SETUPWIZARD, wxART_OTHER),wxDefaultPosition,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{

  m_one = new wxWizardPageSimple(this);
  new wxStaticText(m_one, wxID_ANY,
             _("This wizard guides you through some essential settings.\n"),
             wxPoint(5,5)
        );
  m_two= new GameDirPage(this);
  wxWizardPageSimple::Chain(m_one, m_two);
}

BEGIN_EVENT_TABLE(GameDirPage, wxWizardPageSimple)
  EVT_BUTTON(ID_BTN_SELECTGAMEDIR, GameDirPage::OnBtnSelectGameDir)
END_EVENT_TABLE()

GameDirPage::GameDirPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
  // create items

  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  mainsizer->Add(
    new wxStaticText(this, wxID_ANY,
    wxString::Format(_("Select the directory where your %s resides:\n"), wxGetApp().factory()->gamename().c_str())), 
    0, wxALL, 5
    );

  wxBoxSizer *dirsizer = new wxBoxSizer(wxHORIZONTAL);
  m_gamedir = new wxTextCtrl(this, wxID_ANY);
  dirsizer->Add( m_gamedir, 3, wxEXPAND|wxALL, 5 );
  dirsizer->Add( new wxButton(this, ID_BTN_SELECTGAMEDIR, _("Choose")), 0, wxALL, 5 );
  mainsizer->Add(dirsizer);
  SetSizer(mainsizer);
  mainsizer->Fit(this);
}
void GameDirPage::OnBtnSelectGameDir( wxCommandEvent& )
{
  wxString dir = wxDirSelector();
  m_gamedir->SetValue(dir);
}

bool GameDirPage::TransferDataFromWindow()
{
  if( !wxFile::Exists(m_gamedir->GetValue() + PATH_SEP + wxGetApp().factory()->filename_gamebin()) )
  {
    if( wxYES != wxMessageBox(wxString::Format(_("I cannot find %s in the given directory.\nDo you want to continue nevertheless?"), wxGetApp().factory()->filename_gamebin().c_str()), 
      _("Invalid game directory"),
      wxICON_QUESTION | wxYES_NO, this) )
      return false;
  }
  return true; // everything fine
}
  