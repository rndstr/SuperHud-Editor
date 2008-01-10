#include "prefsdialog.h"

#include "artprovider.h"

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/spinctrl.h>
#include <wx/bookctrl.h>

PrefsDialog::PrefsDialog(wxWindow *parent)
{

  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  const wxSize imgsize(32, 32);
  m_imglist = new wxImageList(imgsize.GetWidth(), imgsize.GetHeight());
  m_imglist->Add(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, imgsize));

  //SetSheetStyle(wxPROPSHEET_SHRINKTOFIT | wxPROPSHEET_TOOLBOOK);
  //SetSheetInnerBorder(0);
  //SetSheetOuterBorder(0);


  Create(parent, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
  CreateButtons( wxOK | wxCANCEL );

  wxBookCtrlBase *ctrl = GetBookCtrl();
  ctrl->SetImageList(m_imglist);

  wxPanel* display = create_display(ctrl);
  wxPanel* sample = create_sample(ctrl);

  ctrl->AddPage(display, _("Display"), true, 0);
  ctrl->AddPage(sample, _("Sample"), true, 1);
  

  LayoutDialog();
}

PrefsDialog::~PrefsDialog()
{
  delete m_imglist;
}

wxPanel* PrefsDialog::create_display(wxWindow *parent)
{
      wxPanel* panel = new wxPanel(parent, wxID_ANY);

    // begin wxGlade: display_prefs::attributes
    wxStaticBox* helper_styler_staticbox;
    wxStaticBox* grid_styler_staticbox;
    wxStaticBox* ar_styler_staticbox;
    wxChoice* m_aspectratio;
    wxStaticText* label_1;
    wxTextCtrl* m_aspectratioX;
    wxStaticText* label_2;
    wxTextCtrl* m_aspectratioY;
    wxStaticText* label_3;
    wxStaticText* label_4;
    wxSpinCtrl* m_gridX;
    wxStaticText* label_5;
    wxSpinCtrl* m_gridY;
    wxStaticText* label_6;
    wxButton* m_gridcolor;
    wxStaticText* label_7;
    wxSpinCtrl* m_gridcolor_alpha;
    wxStaticText* label_8;
    wxStaticText* label_9;
    wxButton* m_fill_selected;
    wxStaticText* labelfa;
    wxSpinCtrl* m_fill_selected_alpha;
    wxStaticText* label_11;
    wxButton* m_border_selected;
    wxStaticText* labeloa;
    wxSpinCtrl* m_border_selected_alpha;
    wxStaticText* label_8_copy;
    wxStaticText* label_9_copy;
    wxButton* m_fill;
    wxStaticText* labelaeijfafe;
    wxSpinCtrl* m_fill_alpha;
    wxStaticText* label_11_copy;
    wxButton* m_border;
    wxStaticText* labelahosehjfija;
    wxSlider* m_border_alpha;
    // end wxGlade
  // begin wxGlade: display_prefs::display_prefs
    grid_styler_staticbox = new wxStaticBox(panel, -1, wxT("Grid"));
    helper_styler_staticbox = new wxStaticBox(panel, -1, wxT("Helper"));
    ar_styler_staticbox = new wxStaticBox(panel, -1, wxT("Aspect ratio"));
    const wxString m_aspectratio_choices[] = {
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)"),
        wxT("Custom")
    };
    m_aspectratio = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_aspectratio_choices, 0);
    label_1 = new wxStaticText(panel, wxID_ANY, wxT("Custom:"));
    m_aspectratioX = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT(":"));
    m_aspectratioY = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT("Proportion of width:height ingame."));
    label_4 = new wxStaticText(panel, wxID_ANY, wxT("Distance X:"));
    m_gridX = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_5 = new wxStaticText(panel, wxID_ANY, wxT("Distance Y:"));
    m_gridY = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_6 = new wxStaticText(panel, wxID_ANY, wxT("Color:"));
    m_gridcolor = new wxButton(panel, wxID_ANY, wxEmptyString);
    label_7 = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_gridcolor_alpha = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_8 = new wxStaticText(panel, wxID_ANY, wxT("Selected Elements"));
    label_9 = new wxStaticText(panel, wxID_ANY, wxT("Background:"));
    m_fill_selected = new wxButton(panel, wxID_ANY, wxEmptyString);
    labelfa = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_fill_selected_alpha = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_11 = new wxStaticText(panel, wxID_ANY, wxT("Outline:"));
    m_border_selected = new wxButton(panel, wxID_ANY, wxEmptyString);
    labeloa = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_border_selected_alpha = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_8_copy = new wxStaticText(panel, wxID_ANY, wxT("Deselected Elements"));
    label_9_copy = new wxStaticText(panel, wxID_ANY, wxT("Background:"));
    m_fill = new wxButton(panel, wxID_ANY, wxEmptyString);
    labelaeijfafe = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_fill_alpha = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_11_copy = new wxStaticText(panel, wxID_ANY, wxT("Outline:"));
    m_border = new wxButton(panel, wxID_ANY, wxEmptyString);
    labelahosehjfija = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_border_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    // end wxGlade
    // begin wxGlade: display_prefs::set_properties
    m_aspectratio->SetSelection(0);
    m_aspectratioX->SetMinSize(wxSize(40, -1));
    m_aspectratioY->SetMinSize(wxSize(40, -1));
    label_3->SetForegroundColour(wxColour(0, 0, 85));
    m_gridX->SetMinSize(wxSize(70, -1));
    m_gridY->SetMinSize(wxSize(70, -1));
    m_gridcolor_alpha->SetMinSize(wxSize(70, -1));
    label_8->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    m_fill_selected_alpha->SetMinSize(wxSize(70, -1));
    m_border_selected_alpha->SetMinSize(wxSize(70, -1));
    label_8_copy->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    m_fill_alpha->SetMinSize(wxSize(70, -1));
    // end wxGlade
    // begin wxGlade: display_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* helper_styler = new wxStaticBoxSizer(helper_styler_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_1_copy = new wxFlexGridSizer(2, 4, 0, 0);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(2, 4, 0, 0);
    wxStaticBoxSizer* grid_styler = new wxStaticBoxSizer(grid_styler_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_el_sizer = new wxFlexGridSizer(2, 4, 0, 0);
    wxStaticBoxSizer* ar_styler = new wxStaticBoxSizer(ar_styler_staticbox, wxVERTICAL);
    wxBoxSizer* ar_el_sizer = new wxBoxSizer(wxHORIZONTAL);
    ar_el_sizer->Add(m_aspectratio, 0, wxALL, 3);
    ar_el_sizer->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_aspectratioX, 0, wxALL, 3);
    ar_el_sizer->Add(label_2, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_aspectratioY, 0, wxALL, 3);
    ar_styler->Add(ar_el_sizer, 1, wxALL|wxEXPAND, 0);
    ar_styler->Add(label_3, 0, wxALL|wxEXPAND, 3);
    container->Add(ar_styler, 0, wxALL|wxEXPAND, 3);
    grid_el_sizer->Add(label_4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_gridX, 0, wxALL, 3);
    grid_el_sizer->Add(label_5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_gridY, 0, wxALL, 3);
    grid_el_sizer->Add(label_6, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_gridcolor, 0, wxALL, 3);
    grid_el_sizer->Add(label_7, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_gridcolor_alpha, 0, wxALL, 3);
    grid_styler->Add(grid_el_sizer, 1, wxEXPAND, 0);
    container->Add(grid_styler, 0, wxALL|wxEXPAND, 3);
    helper_styler->Add(label_8, 0, wxALL, 3);
    grid_sizer_1->Add(label_9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_fill_selected, 0, wxALL, 3);
    grid_sizer_1->Add(labelfa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_fill_selected_alpha, 0, wxALL, 3);
    grid_sizer_1->Add(label_11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_border_selected, 0, wxALL, 3);
    grid_sizer_1->Add(labeloa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_border_selected_alpha, 0, wxALL, 3);
    helper_styler->Add(grid_sizer_1, 1, wxEXPAND, 0);
    helper_styler->Add(label_8_copy, 0, wxALL, 3);
    grid_sizer_1_copy->Add(label_9_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_fill, 0, wxALL, 3);
    grid_sizer_1_copy->Add(labelaeijfafe, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_fill_alpha, 0, wxALL, 3);
    grid_sizer_1_copy->Add(label_11_copy, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_border, 0, wxALL, 3);
    grid_sizer_1_copy->Add(labelahosehjfija, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_border_alpha, 0, wxALIGN_CENTER_VERTICAL, 0);
    helper_styler->Add(grid_sizer_1_copy, 1, wxEXPAND, 0);
    container->Add(helper_styler, 0, wxEXPAND, 0);
    // end wxGlade



    top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
    //top_sizer->AddSpacer(5);

    panel->SetSizer(top_sizer);
    top_sizer->Fit(panel);

    return panel;
}

wxPanel* PrefsDialog::create_sample(wxWindow *parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// LOAD LAST FILE

    wxBoxSizer* itemSizer3 = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* checkBox3 = new wxCheckBox(panel, wxID_ANY, _("&Load last project on startup"), wxDefaultPosition, wxDefaultSize);
    itemSizer3->Add(checkBox3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(itemSizer3, 0, wxGROW|wxALL, 0);
/*
    //// AUTOSAVE

    wxString autoSaveLabel = _("&Auto-save every");
    wxString minsLabel = _("mins");

    wxBoxSizer* itemSizer12 = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* checkBox12 = new wxCheckBox(panel, wxID_ANY, autoSaveLabel, wxDefaultPosition, wxDefaultSize);

    wxSpinCtrl* spinCtrl12 = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxSize(40, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 60, 1);
    
    itemSizer12->Add(checkBox12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    itemSizer12->Add(spinCtrl12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    itemSizer12->Add(new wxStaticText(panel, wxID_STATIC, minsLabel), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(itemSizer12, 0, wxGROW|wxALL, 0);

    //// TOOLTIPS

    wxBoxSizer* itemSizer8 = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* checkBox6 = new wxCheckBox(panel, wxID_ANY, _("Show &tooltips"), wxDefaultPosition, wxDefaultSize);
    itemSizer8->Add(checkBox6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(itemSizer8, 0, wxGROW|wxALL, 0);

    */
    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;


}



wxPanel* PrefsDialog::create_old(wxWindow *parent)
{
  wxPanel *panel = new wxPanel(parent, wxID_ANY);

    // begin wxGlade: prefs_display::attributes
    wxStaticBox* sizer_8_staticbox;
    wxStaticBox* sizer_3_staticbox;
    wxComboBox* m_aspectratio;
    wxStaticText* label_2;
    wxTextCtrl* m_aspectratio_x;
    wxStaticText* label_3;
    wxTextCtrl* m_aspectratio_y;
    wxStaticText* label_4;
    wxStaticText* label_10;
    wxSpinCtrl* m_gridX;
    wxStaticText* label_11;
    wxSpinCtrl* m_gridY;
    // end wxGlade
    // begin wxGlade: prefs_display::prefs_display
    sizer_8_staticbox = new wxStaticBox(panel, -1, wxT("Grid"));
    sizer_3_staticbox = new wxStaticBox(panel, -1, wxT("Aspect Ratio"));
    const wxString m_aspectratio_choices[] = { wxT("hehe")
        
    };
    m_aspectratio = new wxComboBox(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, m_aspectratio_choices, wxCB_DROPDOWN);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT("Custom:"));
    m_aspectratio_x = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT(":"));
    m_aspectratio_y = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_4 = new wxStaticText(panel, wxID_ANY, wxT("Proportion of width/height ingame by default 4:3 but you \nmight change it with cvars. (r_mode -1 / r_aspectratio / \nr_customwidth / r_customheight) if you own a widescreen \nmonitor."));
    label_10 = new wxStaticText(panel, wxID_ANY, wxT("Distance X:"));
    m_gridX = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_11 = new wxStaticText(panel, wxID_ANY, wxT("Distance Y:"));
    m_gridY = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);

    // end wxGlade
    // begin wxGlade: prefs_display::set_properties
    label_4->SetForegroundColour(wxColour(50, 153, 204));
    // end wxGlade
    wxBoxSizer* master = new wxBoxSizer(wxVERTICAL);
    // begin wxGlade: prefs_display::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_8 = new wxStaticBoxSizer(sizer_8_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_9 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer_4->Add(m_aspectratio, 0, wxALL|wxADJUST_MINSIZE, 3);
    sizer_4->Add(label_2, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 3);
    sizer_4->Add(m_aspectratio_x, 0, wxALL|wxADJUST_MINSIZE, 3);
    sizer_4->Add(label_3, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);
    sizer_4->Add(m_aspectratio_y, 0, wxALL|wxADJUST_MINSIZE, 3);
    sizer_3->Add(sizer_4, 0, wxEXPAND, 0);
    sizer_3->Add(label_4, 1, wxALL|wxEXPAND, 5);
    top_sizer->Add(sizer_3, 0, wxEXPAND, 0);
    sizer_9->Add(label_10, 0, wxALL|wxALIGN_CENTER_VERTICAL| wxADJUST_MINSIZE, 3);
    sizer_9->Add(m_gridX, 0, wxALL|wxADJUST_MINSIZE, 3);
    sizer_9->Add(label_11, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 3);
    sizer_9->Add(m_gridY, 0, wxALL|wxADJUST_MINSIZE, 3);
    sizer_8->Add(sizer_9, 0, wxEXPAND, 0);
    top_sizer->Add(sizer_8, 0, wxEXPAND, 0);
    // end wxGlade

    master->Add( top_sizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    master->AddSpacer(5);


  panel->SetSizer(master);
  master->Fit(panel);
  return panel;
}



