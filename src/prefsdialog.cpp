// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// panel file is part of SuperHud Editor.
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
#include "prefsdialog.h"

#include "artprovider.h"
#include "prefs.h"

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/spinctrl.h>
#include <wx/bookctrl.h>


BEGIN_EVENT_TABLE(PrefsDialog, wxPropertySheetDialog)
  EVT_BUTTON(wxID_OK, PrefsDialog::OnOk)
  EVT_BUTTON(wxID_DEFAULT, PrefsDialog::OnAdvancedDefault) // advanced settings default
#if ENABLE_CPMA
  EVT_CHECKBOX(ID_CHECK_Q3_BACKGROUND_USEDEFAULT, PrefsDialog::OnCheckQ3BackgroundUseDefault)
#endif
  EVT_CHOICE(ID_CHOICE_ASPECTRATIO, PrefsDialog::OnChoiceAspectratio)
END_EVENT_TABLE()

PrefsDialog::PrefsDialog(wxWindow *parent)
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  const wxSize imgsize(16, 16);
  m_imglist = new wxImageList(imgsize.GetWidth(), imgsize.GetHeight());
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_DISPLAY, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_CPMA, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_NETWORK, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_MISC, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_ADVANCED, wxART_OTHER, imgsize));

  //SetSheetStyle(wxPROPSHEET_TOOLBOOK);
  SetSheetStyle(wxPROPSHEET_LISTBOOK);
  //SetSheetInnerBorder(0);
  //SetSheetOuterBorder(0);


  Create(parent, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
  CreateButtons( wxOK | wxCANCEL );

  wxBookCtrlBase *ctrl = GetBookCtrl();
  ctrl->SetImageList(m_imglist);

  wxPanel* display = create_display(ctrl);
#if ENABLE_CPMA
  wxPanel* cpma = create_cpma(ctrl);
#endif
  wxPanel* network = create_network(ctrl);
  wxPanel* misc = create_misc(ctrl);
  wxPanel* advanced = create_advanced(ctrl);

  ctrl->AddPage(display, _("Display"), true, 0);
#if ENABLE_CPMA
  ctrl->AddPage(cpma, _("CPMA"), false, 1);
#endif
  ctrl->AddPage(network, _("Network"), false, 2);
  ctrl->AddPage(misc, _("Misc"), false, 3);
  ctrl->AddPage(advanced, _("Advanced"), false, ADVANCED_IDX);
  
  fill_defaults();

  LayoutDialog();
}

PrefsDialog::~PrefsDialog()
{
  delete m_imglist;
}

#if ENABLE_CPMA
void PrefsDialog::OnCheckQ3BackgroundUseDefault( wxCommandEvent& ev )
{
  m_q3_background->Enable( !ev.IsChecked() );
}
#endif

void PrefsDialog::OnChoiceAspectratio( wxCommandEvent& )
{
  m_view_aspectratio_custom->Enable( m_view_aspectratio->GetSelection() == 2 );
}

void PrefsDialog::OnOk( wxCommandEvent& ev )
{
  // --- display
  switch( m_view_aspectratio->GetSelection() )
  {
  case 1:
    Prefs::get().set(wxT("view_aspectratio"), wxT("16:10"));
    break;
  case 2:
    Prefs::get().set(wxT("view_aspectratio"), m_view_aspectratio_custom->GetValue());
    break;
  case 0: 
  default:
    Prefs::get().set(wxT("view_aspectratio"), wxT("4:3"));
    break;
  }
  Prefs::get().seti(wxT("view_gridX"), m_view_gridX->GetValue());
  Prefs::get().seti(wxT("view_gridY"), m_view_gridY->GetValue());

  // --- cpma
#if ENABLE_CPMA
  Prefs::get().set( wxT("q3_gamedir"), m_q3_gamedir->GetValue() );
  if( m_q3_background_usedefault->GetValue() )
    Prefs::get().set( wxT("q3_background"), wxT("") );
  else
    Prefs::get().set( wxT("q3_background"), m_q3_background->GetValue() );
#endif

  // --- network
  Prefs::get().setb(wxT("startup_checkforupdate"), m_startup_checkforupdate->GetValue() );
  Prefs::get().set(wxT("net_proxy"), m_net_proxy->GetValue());

  // --- misc
  Prefs::get().setb(wxT("save_backup"), m_save_backup->GetValue());
  Prefs::get().setb(wxT("startup_load"), m_startup_load->GetValue());
  Prefs::get().setb(wxT("elements_collections"), m_elements_collections->GetValue());

  // --- advanced
  Prefs::get().seti(wxT("view_dragthreshold"), m_pg->GetPropertyValueAsInt(wxT("view-dragthreshold")));
  Prefs::get().seti(wxT("view_snapthreshold"), m_pg->GetPropertyValueAsInt(wxT("view-snapthreshold")));
  Prefs::get().seti(wxT("view_movestep"), m_pg->GetPropertyValueAsInt(wxT("view-movestep")));
  
  
  wxColour col;

  col << m_vgc->GetValue();
  Prefs::get().setwxc(wxT("view_gridcolor"), col, m_vgca->GetValue().GetInteger());

  col << m_vhfs->GetValue();
  Prefs::get().setwxc(wxT("view_helper_fill_selected"), col, m_vhfsa->GetValue().GetInteger());

  col << m_vhbs->GetValue();
  Prefs::get().setwxc(wxT("view_helper_border_selected"), col, m_vhbsa->GetValue().GetInteger());

  col << m_vhf->GetValue();
  Prefs::get().setwxc(wxT("view_helper_fill"), col, m_vhfa->GetValue().GetInteger());

  col << m_vhb->GetValue();
  Prefs::get().setwxc(wxT("view_helper_border"), col, m_vhba->GetValue().GetInteger());
  

  col << m_pcol->GetValue();
  Prefs::get().setwxc(wxT("props_color"), col);

  col << m_pbgcol->GetValue();
  Prefs::get().setwxc(wxT("props_bgcolor"), col);

  col << m_picol->GetValue();
  Prefs::get().setwxc(wxT("props_inheritcolor"), col);

  col << m_pibgcol->GetValue();
  Prefs::get().setwxc(wxT("props_inheritbgcolor"), col);

  Prefs::get().seti(wxT("elements_collnamecount"), m_ec->GetValue().GetInteger());
  Prefs::get().setb(wxT("props_neverdisable"), m_pnd->GetValue().GetBool());
  Prefs::get().setb(wxT("save_writedisabled"), m_swd->GetValue().GetBool());

  ev.Skip();
}



void PrefsDialog::fill_defaults()
{
  // --- display
  wxString ar = Prefs::get().var(wxT("view_aspectratio")).sval();
  m_view_aspectratio_custom->Disable();
  if( wxT("4:3") == ar )
    m_view_aspectratio->SetSelection(0);
  else if( wxT("16:10") == ar )
    m_view_aspectratio->SetSelection(1);
  else
  {
    m_view_aspectratio->SetSelection(2);
    m_view_aspectratio_custom->Enable();
    m_view_aspectratio_custom->SetValue(ar);
  }
  m_view_gridX->SetValue(Prefs::get().var(wxT("view_gridX")).ival());
  m_view_gridY->SetValue(Prefs::get().var(wxT("view_gridY")).ival());

#if ENABLE_CPMA
  // --- cpma
  m_q3_gamedir->SetValue( Prefs::get().var(wxT("q3_gamedir")).sval() );
  m_q3_background_usedefault->SetValue( Prefs::get().var(wxT("q3_background")).sval().empty() );
  m_q3_background->SetValue( Prefs::get().var(wxT("q3_background")).sval() );
  m_q3_background->Enable( !Prefs::get().var(wxT("q3_background")).sval().empty() );
#endif

  // --- network
  m_startup_checkforupdate->SetValue( Prefs::get().var(wxT("startup_checkforupdate")).bval() );
  m_net_proxy->SetValue( Prefs::get().var(wxT("net_proxy")).sval() );

  // --- misc
  m_save_backup->SetValue( Prefs::get().var(wxT("save_backup")).bval() );
  m_startup_load->SetValue( Prefs::get().var(wxT("startup_load")).bval() );
  m_elements_collections->SetValue( Prefs::get().var(wxT("elements_collections")).bval() );
  

  // --- advanced
  // is done in create_advanced

}


wxPanel* PrefsDialog::create_display(wxWindow *parent)
{
      wxPanel* panel = new wxPanel(parent, wxID_ANY);
    // begin wxGlade: display_prefs::display_prefs
    grid_styler_staticbox = new wxStaticBox(panel, -1, wxT("Grid"));
    ar_styler_staticbox = new wxStaticBox(panel, -1, wxT("Aspect ratio"));
    display_title = new wxStaticText(panel, wxID_ANY, wxT("Display"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT("Proportion of your ingame resolution."));
    const wxString m_view_aspectratio_choices[] = {
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)"),
        wxT("Custom")
    };
    m_view_aspectratio = new wxChoice(panel, ID_CHOICE_ASPECTRATIO, wxDefaultPosition, wxDefaultSize, 3, m_view_aspectratio_choices, 0);
    label_1 = new wxStaticText(panel, wxID_ANY, wxT("Custom:"));
    m_view_aspectratio_custom = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT("width:height"));
    label_7 = new wxStaticText(panel, wxID_ANY, wxT("NOTE: If you want to convert the current HUD to panel new ratio as well\nyou shouldn't change it here but convert the hud in Tools->Convert HUD..."));
    label_4 = new wxStaticText(panel, wxID_ANY, wxT("Distance X:"));
    m_view_gridX = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_5 = new wxStaticText(panel, wxID_ANY, wxT("Distance Y:"));
    m_view_gridY = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    // end wxGlade
    // begin wxGlade: display_prefs::set_properties
    display_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_3->SetForegroundColour(wxColour(0, 0, 85));
    m_view_aspectratio->SetSelection(0);
    m_view_aspectratio_custom->SetMinSize(wxSize(70, -1));
    label_2->SetForegroundColour(wxColour(0, 0, 85));
    label_7->SetForegroundColour(wxColour(0, 0, 85));
    m_view_gridX->SetMinSize(wxSize(70, -1));
    m_view_gridY->SetMinSize(wxSize(70, -1));
    // end wxGlade
    // begin wxGlade: display_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* grid_styler = new wxStaticBoxSizer(grid_styler_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_el_sizer = new wxFlexGridSizer(2, 4, 0, 0);
    wxStaticBoxSizer* ar_styler = new wxStaticBoxSizer(ar_styler_staticbox, wxVERTICAL);
    wxBoxSizer* ar_el_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxVERTICAL);
    sizer_8->Add(display_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    container->Add(sizer_8, 0, wxEXPAND, 0);
    ar_styler->Add(label_3, 0, wxALL|wxEXPAND, 3);
    ar_el_sizer->Add(m_view_aspectratio, 0, wxALL, 3);
    ar_el_sizer->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_view_aspectratio_custom, 0, wxALL, 3);
    ar_el_sizer->Add(label_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_styler->Add(ar_el_sizer, 1, wxALL|wxEXPAND, 0);
    ar_styler->Add(label_7, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    container->Add(ar_styler, 0, wxALL|wxEXPAND, 3);
    grid_el_sizer->Add(label_4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridX, 0, wxALL, 3);
    grid_el_sizer->Add(label_5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridY, 0, wxALL, 3);
    grid_styler->Add(grid_el_sizer, 1, wxEXPAND, 0);
    container->Add(grid_styler, 0, wxALL|wxEXPAND, 3);
    // end wxGlade
    
    


    top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
    //top_sizer->AddSpacer(5);

    panel->SetSizer(top_sizer);
    top_sizer->Fit(panel);

    return panel;
}

#if ENABLE_CPMA
wxPanel* PrefsDialog::create_cpma(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);
  
    // begin wxGlade: cpma_prefs::cpma_prefs
    sizer_5_staticbox = new wxStaticBox(panel, -1, wxT("Background image"));
    sizer_3_staticbox = new wxStaticBox(panel, -1, wxT("Quake3 path"));
    cpma_title = new wxStaticText(panel, wxID_ANY, wxT("CPMA"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_13 = new wxStaticText(panel, wxID_ANY, wxT("Where you installed Quake3 at."));
    m_q3_gamedir = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    button_2 = new wxButton(panel, ID_BTN_Q3_GAMEDIR_BROWSE, wxT("..."));
    label_12 = new wxStaticText(panel, wxID_ANY, wxT("Preferrably of width and height as power of two."));
    m_q3_background = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    button_2_copy = new wxButton(panel, ID_BTN_Q3_BACKGROUND_BROWSE, wxT("..."));
    m_q3_background_usedefault = new wxCheckBox(panel, ID_CHECK_Q3_BACKGROUND_USEDEFAULT, wxT("Use default"));
    // end wxGlade
    // begin wxGlade: cpma_prefs::set_properties
    SetTitle(wxT("cpma_prefs"));
    cpma_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_13->SetForegroundColour(wxColour(0, 0, 85));
    button_2->SetMinSize(wxSize(40, -1));
    label_12->SetForegroundColour(wxColour(0, 0, 85));
    button_2_copy->SetMinSize(wxSize(40, -1));
    // end wxGlade
    // begin wxGlade: cpma_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_10 = new wxBoxSizer(wxVERTICAL);
    sizer_10->Add(cpma_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_10, 0, wxEXPAND, 0);
    sizer_3->Add(label_13, 0, wxALL, 3);
    sizer_4->Add(m_q3_gamedir, 1, wxALL|wxEXPAND, 3);
    sizer_4->Add(button_2, 0, wxALL, 3);
    sizer_3->Add(sizer_4, 1, wxEXPAND, 0);
    container->Add(sizer_3, 0, wxALL|wxEXPAND, 3);
    sizer_5->Add(label_12, 0, wxALL, 3);
    sizer_6->Add(m_q3_background, 1, wxALL|wxEXPAND, 3);
    sizer_6->Add(button_2_copy, 0, wxALL, 3);
    sizer_5->Add(sizer_6, 1, wxEXPAND, 0);
    sizer_5->Add(m_q3_background_usedefault, 0, wxALL, 3);
    container->Add(sizer_5, 0, wxALL|wxEXPAND, 3);
    // end wxGlade

    

  top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
  //top_sizer->AddSpacer(5);

  panel->SetSizer(top_sizer);
  top_sizer->Fit(panel);

  return panel;
}
#endif

wxPanel* PrefsDialog::create_network(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);
  
    // begin wxGlade: network_prefs::network_prefs
    sizer_13_staticbox = new wxStaticBox(panel, -1, wxT("Proxy"));
    sizer_12_staticbox = new wxStaticBox(panel, -1, wxT("Startup"));
    network_title = new wxStaticText(panel, wxID_ANY, wxT("Network"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    m_startup_checkforupdate = new wxCheckBox(panel, wxID_ANY, wxT("Check for new version"));
    m_net_proxy = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_8 = new wxStaticText(panel, wxID_ANY, wxT("host:port"));
    // end wxGlade
    // begin wxGlade: network_prefs::set_properties
    SetTitle(wxT("network_prefs"));
    network_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_8->SetForegroundColour(wxColour(0, 0, 85));
    // end wxGlade
    // begin wxGlade: network_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_13 = new wxStaticBoxSizer(sizer_13_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_12 = new wxStaticBoxSizer(sizer_12_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_11 = new wxBoxSizer(wxVERTICAL);
    sizer_11->Add(network_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_11, 0, wxEXPAND, 0);
    sizer_12->Add(m_startup_checkforupdate, 0, wxALL, 3);
    container->Add(sizer_12, 0, wxALL|wxEXPAND, 3);
    sizer_13->Add(m_net_proxy, 0, wxALL|wxEXPAND, 3);
    sizer_13->Add(label_8, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);
    container->Add(sizer_13, 0, wxALL|wxEXPAND, 3);
    // end wxGlade


  top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
  //top_sizer->AddSpacer(5);

  panel->SetSizer(top_sizer);
  top_sizer->Fit(panel);

  return panel;
}

wxPanel* PrefsDialog::create_misc(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);

    // begin wxGlade: misc_prefs::misc_prefs
    sizer_2_staticbox = new wxStaticBox(panel, -1, wxT("Message dialogs"));
    sizer_7_staticbox = new wxStaticBox(panel, -1, wxT("Elements list"));
    sizer_1_staticbox = new wxStaticBox(panel, -1, wxT("Load / Save"));
    misc_title = new wxStaticText(panel, wxID_ANY, wxT("Misc"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    m_save_backup = new wxCheckBox(panel, wxID_ANY, wxT("Create backup before saving"));
    m_startup_load = new wxCheckBox(panel, wxID_ANY, wxT("Reload previously open HUD on startup"));
    label_14 = new wxStaticText(panel, wxID_ANY, wxT("On several occassions you are prompted a message dialog that\nyou can disable to pop up. If you want to enable them all\nagain, click the button below."));
    button_1 = new wxButton(panel, ID_BTN_RESETDIALOGS, wxT("Reset dialogs"));
    m_elements_collections = new wxCheckBox(panel, wxID_ANY, wxT("Use collections"));
    label_6 = new wxStaticText(panel, wxID_ANY, wxT("Collections are a mean to gather similar named elements together."));
    // end wxGlade
    // begin wxGlade: misc_prefs::set_properties
    SetTitle(wxT("misc_prefs"));
    misc_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_14->SetForegroundColour(wxColour(0, 0, 85));
    label_6->SetForegroundColour(wxColour(0, 0, 85));
    // end wxGlade
    // begin wxGlade: misc_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_7 = new wxStaticBoxSizer(sizer_7_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_1 = new wxStaticBoxSizer(sizer_1_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_9 = new wxBoxSizer(wxVERTICAL);
    sizer_9->Add(misc_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_9, 0, wxEXPAND, 0);
    sizer_1->Add(m_save_backup, 0, wxALL, 3);
    sizer_1->Add(m_startup_load, 0, wxALL, 3);
    container->Add(sizer_1, 0, wxALL|wxEXPAND, 3);
    sizer_2->Add(label_14, 0, wxALL, 3);
    sizer_2->Add(button_1, 0, wxALL, 3);
    container->Add(sizer_2, 0, wxALL|wxEXPAND, 3);
    sizer_7->Add(m_elements_collections, 0, wxALL, 3);
    sizer_7->Add(label_6, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    container->Add(sizer_7, 0, wxALL|wxEXPAND, 3);
    // end wxGlade


  top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
  //top_sizer->AddSpacer(5);

  panel->SetSizer(top_sizer);
  top_sizer->Fit(panel);

  return panel;
}

wxPanel* PrefsDialog::create_advanced(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);
  wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);

  wxStaticText *adv_title = new wxStaticText(panel, wxID_ANY, wxT("Advanced"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
  wxBoxSizer* sizer_title = new wxBoxSizer(wxVERTICAL);
  sizer_title->Add(adv_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
  container->Add(sizer_title, 0, wxEXPAND, 0);

  adv_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));

    m_pg = new wxPropertyGrid(
            panel, // parent
            wxID_ANY, // id
            wxDefaultPosition, // position
            wxDefaultSize, // size
            // Some specific window styles - for all additional styles,
            // see Modules->PropertyGrid Window Styles
            wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
            // Default style
            wxPG_DEFAULT_STYLE );
   m_pg->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS );


   m_pg->Append( new wxPropertyCategory(_("Mouse/key behaviour")) );
   m_pg->Append( new wxIntProperty(_("Drag threshold"), wxT("view-dragthreshold"), Prefs::get().var(wxT("view_dragthreshold")).ival()) );
   m_pg->SetPropertyHelpString( wxT("view-dragthreshold"), _("How many pixel it takes till drag&drop begins") );
   m_pg->Append( new wxIntProperty(_("Snap threshold"), wxT("view-snapthreshold"), Prefs::get().var(wxT("view_snapthreshold")).ival()) );
   m_pg->SetPropertyHelpString( wxT("view-snapthreshold"), _("Says how close an item has to be for the element to snap at it") );
   m_pg->Append( new wxIntProperty(_("Move step"), wxT("view-movestep"), Prefs::get().var(wxT("view_movestep")).ival()) );
   m_pg->SetPropertyHelpString( wxT("view-movestep"), _("How many pixels to move upon pressing arrow keys") );

   m_pg->Append( new wxPropertyCategory(_("Grid color")) );
   m_vgc = m_pg->Append( new wxColourProperty(_("Color"), wxT("view-gridcolor"), Prefs::get().var(wxT("view_gridcolor")).wxcval()) );
   m_vgca = m_pg->Append( new wxIntProperty(_("Opacity"), wxT("view-gridcolor-alpha"), Prefs::get().var(wxT("view_gridcolor")).cval().a100()) );
   
   m_pg->Append( new wxPropertyCategory(_("Helpers color")) );
   wxPGId pid = m_pg->Append( new wxParentProperty(_("Selected elements")) );
   m_vhfs = m_pg->AppendIn( pid, new wxColourProperty(_("Fill"), wxT("view-helper-fill-selected"), Prefs::get().var(wxT("view_helper_fill_selected")).wxcval()) );
   m_vhfsa = m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-fill-selected-alpha"), Prefs::get().var(wxT("view_helper_fill_selected")).cval().a100()) );
   m_vhbs = m_pg->AppendIn( pid, new wxColourProperty(_("Border"), wxT("view-helper-border-selected"), Prefs::get().var(wxT("view_helper_border_selected")).wxcval()) );
   m_vhbsa = m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-border-selected-alpha"), Prefs::get().var(wxT("view_helper_border_selected")).cval().a100()) );
   pid = m_pg->Append( new wxParentProperty(_("Deselected elements")) );
   m_vhf = m_pg->AppendIn( pid, new wxColourProperty(_("Fill"), wxT("view-helper-fill"), Prefs::get().var(wxT("view_helper_fill")).wxcval()) );
   m_vhfa = m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-fill-alpha"), Prefs::get().var(wxT("view_helper_fill")).cval().a100()) );
   m_vhb = m_pg->AppendIn( pid, new wxColourProperty(_("Border"), wxT("view-helper-border"), Prefs::get().var(wxT("view_helper_border")).wxcval()) );
   m_vhba = m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-border-alpha"), Prefs::get().var(wxT("view_helper_border")).cval().a100()) );

   m_pg->Append( new wxPropertyCategory(_("Properties pane cell color")) );
   pid = m_pg->Append( new wxParentProperty(_("Default")) );
   m_pcol = m_pg->AppendIn( pid, new wxColourProperty(_("Foreground"), wxT("props-color"), Prefs::get().var(wxT("props_color")).wxcval()) );
   m_pbgcol = m_pg->AppendIn( pid, new wxColourProperty(_("Background"), wxT("props-bgcolor"), Prefs::get().var(wxT("props_bgcolor")).wxcval()) );
   pid = m_pg->Append( new wxParentProperty(_("Inherited")) );
   m_picol = m_pg->AppendIn( pid, new wxColourProperty(_("Foreground"), wxT("props-inheritcolor"), Prefs::get().var(wxT("props_inheritcolor")).wxcval()) );
   m_pibgcol = m_pg->AppendIn( pid, new wxColourProperty(_("Background"), wxT("props-inheritbgcolor"), Prefs::get().var(wxT("props_inheritbgcolor")).wxcval()) );


   m_pg->Append( new wxPropertyCategory(_("Expert")) );

   pid = m_pg->Append( new wxParentProperty(_("Elements list")) );
   m_ec = m_pg->AppendIn( pid, new wxIntProperty(_("Collection chars match"), wxT("elements-collnamecount"), Prefs::get().var(wxT("elements_collnamecount")).ival()) );
   m_ec->SetHelpString( _("How many characters of consecutive element names have to match for a collection to be created (Default: 3)") );
   
   pid = m_pg->Append( new wxParentProperty(_("Properties pane")) );
   m_pnd = m_pg->AppendIn( pid, new wxBoolProperty(_("Never disable properties"), wxT("props-neverdisable"), Prefs::get().var(wxT("props_neverdisable")).bval()) );
   m_pnd->SetHelpString( _("Never disable any propertes even if they would be ignored ingame (Default: NO)") );

   pid = m_pg->Append( new wxParentProperty(_("Saving")) );
   m_swd = m_pg->AppendIn( pid, new wxBoolProperty(_("Write hidden elements"), wxT("save-writedisabled"), Prefs::get().var(wxT("save_writedisabled")).bval()) );
   m_swd->SetHelpString( _("Not used elements are written to file but commented out (Default: YES)") );

   wxPGVIterator it;
   for( it = m_pg->GetVIterator(wxPG_ITERATE_ALL); !it.AtEnd(); it.Next() )
   {
     wxPGPropertyWithChildren* pwc = (wxPGPropertyWithChildren*) it.GetProperty();
     if( pwc->GetParentingType() != 0 ) /*PT_NONE*/
       pwc->SetExpanded( true );
   }
   m_pg->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

   container->Add(m_pg, 1, wxALL|wxEXPAND, 3);
   container->Add( new wxButton(panel, wxID_DEFAULT, _("&Reset to Defaults")), 0, wxALL|wxEXPAND, 3 );


    top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
  //top_sizer->AddSpacer(5);

  panel->SetSizer(top_sizer);
  top_sizer->Fit(panel);

  return panel;
}

void PrefsDialog::OnAdvancedDefault( wxCommandEvent& )
{
  if( wxYES == wxMessageBox(_("Are you sure to reset all Advanced settings?"), _("Reset Advanced settings?"), wxYES_NO|wxICON_QUESTION) )
  {
    wxString s[] = { wxT("view_dragthreshold"), wxT("view_snapthreshold"), wxT("view_movestep"), wxT("view_gridcolor"), 
      wxT("view_helper_fill_selected"), wxT("view_helper_border_selected"), wxT("view_helper_fill"), wxT("view_helper_border"),
      wxT("props_color"), wxT("props_bgcolor"), wxT("props_inheritcolor"), wxT("props_inheritbgcolor"),
      wxT("elements_collnamecount"), wxT("props_neverdisable"), wxT("save_writedisabled"),
      wxEmptyString };

    wxString *ps = s;
    while( !ps->empty() )
    {
      Prefs::get().set_default(*ps);
      ++ps;
    }


    // recreate page :x (woohooh lazy)
    wxBookCtrlBase *ctrl = GetBookCtrl();
    ctrl->SetSelection(0);
    ctrl->DeletePage(ADVANCED_IDX);
    wxPanel* advanced = create_advanced(ctrl);
    ctrl->AddPage(advanced, _("Advanced"), false, 4);
    ctrl->SetSelection(ADVANCED_IDX);

  }
}
