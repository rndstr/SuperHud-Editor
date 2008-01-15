#include "setupwizard.h"

#include "factorybase.h"

#include <wx/file.h>



SetupWizard::SetupWizard( wxFrame *frame ) :
  wxWizard(frame,wxID_ANY,_("Setup wizard"),
                   wxArtProvider::GetBitmap(ART_BMP_SETUPWIZARD, wxART_OTHER),wxDefaultPosition,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{

  /*
  m_one = new wxWizardPageSimple(this);
  new wxStaticText(m_one, wxID_ANY,
             _("This wizard guides you through some essential settings.\n"),
             wxPoint(5,5)
        );
        */
  m_two = new GameDirPage(this);
  // only display further dialogs for the first start
  // becausewe also call this setup wizard when the 
  // gamedir is missing
  if( wxGetApp().is_firststart() )
  {
    m_three = new DisplayPage(this);
//  wxWizardPageSimple::Chain(m_one, m_two);
    wxWizardPageSimple::Chain(m_two, m_three);
  }
}

DisplayPage::DisplayPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *title = new wxStaticText(this, wxID_ANY, _("Display"));
  wxStaticText *saspectratio = new wxStaticText(this, wxID_ANY, _("Select your ingame aspect ratio"));
  wxArrayString aspectratio_choices;
  aspectratio_choices.Add(_("Standard (4:3)"));
  aspectratio_choices.Add(_("Widescreen (16:10)"));
  //aspectratio_choices.Add(_("Custom"));
  m_aspectratio = new wxComboBox(this, wxID_ANY, _("Standard (4:3)"), wxDefaultPosition, wxDefaultSize, aspectratio_choices, wxCB_DROPDOWN|wxCB_READONLY);

  mainsizer->Add(title, 0, wxALL, 5);
  mainsizer->Add(saspectratio, 0, wxALL, 5);
  mainsizer->Add(m_aspectratio, 0, wxALL, 5);

  // design
  wxFont ftitle( 18, wxDEFAULT, wxFONTSTYLE_NORMAL, wxBOLD );
  title->SetFont( ftitle );
  title->SetForegroundColour(wxColour(100, 100, 100));


  SetSizer(mainsizer);
  mainsizer->Fit(this);
}

wxString DisplayPage::aspectratio() const
{
  switch( m_aspectratio->GetCurrentSelection() )
  {
    case 1: return wxT("16:10");
    case 0: 
    default:
      break;
  }
  return wxT("4:3");
}
 
BEGIN_EVENT_TABLE(GameDirPage, wxWizardPageSimple)
  EVT_BUTTON(ID_BTN_SELECTGAMEDIR, GameDirPage::OnBtnSelectGameDir)
END_EVENT_TABLE()
//#include <wx/font.h>

GameDirPage::GameDirPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
  // create items
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *dirsizer = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText *title = new wxStaticText(this, wxID_ANY, _("Directory"));
  wxButton *button = new wxButton(this, ID_BTN_SELECTGAMEDIR, _("Choose"));
  m_gamedir = new wxTextCtrl(this, wxID_ANY);


  
  // layout
  dirsizer->Add( m_gamedir, 1, wxALL, 5 );
  dirsizer->Add( button, 0, wxALL, 5 );

  mainsizer->Add(title, 0, wxALL, 5);
  mainsizer->Add(
    new wxStaticText(this, wxID_ANY,
    wxString::Format(_("Select your %s directory\n"), wxGetApp().factory()->gamename().c_str())), 
    0, wxALL, 5
    );
  mainsizer->Add(dirsizer, 1, wxEXPAND);

  // design
  wxFont ftitle( 18, wxDEFAULT, wxFONTSTYLE_NORMAL, wxBOLD );
  title->SetFont( ftitle );
  title->SetForegroundColour(wxColour(100, 100, 100));


  SetSizer(mainsizer);
  mainsizer->Fit(this);
  Layout();
}

void GameDirPage::OnBtnSelectGameDir( wxCommandEvent& )
{
  wxString dir = wxDirSelector( wxString::Format(_("Select your %s directory:"), wxGetApp().factory()->gamename().c_str()), m_gamedir->GetValue() );
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
  
