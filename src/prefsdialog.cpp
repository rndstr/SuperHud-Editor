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
    wxStaticBox* grid_styler_staticbox;
    wxChoice* m_aspectratio;
    wxStaticText* label_1;
    wxTextCtrl* m_aspectratioX;
    wxStaticText* label_2;
    wxTextCtrl* m_aspectratioY;
    wxStaticText* label_3;
    // end wxGlade
    // begin wxGlade: display_prefs::display_prefs
    grid_styler_staticbox = new wxStaticBox(panel, -1, wxT("Grid"));
    const wxString m_aspectratio_choices[] = {_("4:3 (Standard)"), _("16:10 (Widescreen")
        
    };
    m_aspectratio = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, m_aspectratio_choices, 0);
    label_1 = new wxStaticText(panel, wxID_ANY, wxT("Custom:"));
    m_aspectratioX = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT(":"));
    m_aspectratioY = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT("Proportion of width:height ingame."));
    // end wxGlade
    // begin wxGlade: display_prefs::set_properties
    m_aspectratioX->SetMinSize(wxSize(40, -1));
    m_aspectratioY->SetMinSize(wxSize(40, -1));
    label_3->SetForegroundColour(wxColour(0, 0, 85));
    // end wxGlade
    // begin wxGlade: display_prefs::do_layout
    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* container = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* grid_styler = new wxStaticBoxSizer(grid_styler_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(m_aspectratio, 0, wxALL, 3);
    sizer_1->Add(5, 5, 1, wxEXPAND, 0);
    sizer_1->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_1->Add(m_aspectratioX, 0, wxALL, 3);
    sizer_1->Add(label_2, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 3);
    sizer_1->Add(m_aspectratioY, 0, wxALL, 3);
    grid_styler->Add(sizer_1, 1, wxALL|wxEXPAND, 0);
    grid_styler->Add(label_3, 0, wxALL|wxEXPAND, 3);
    container->Add(grid_styler, 0, wxALL|wxEXPAND, 5);
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


