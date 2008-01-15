#include "prefsdialog.h"

#include "artprovider.h"
#include "prefs.h"

#include <wx/wx.h>
#include <wx/imaglist.h>
#include <wx/spinctrl.h>
#include <wx/bookctrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

BEGIN_EVENT_TABLE(PrefsDialog, wxPropertySheetDialog)
  EVT_BUTTON(wxID_DEFAULT, PrefsDialog::OnDefault)
END_EVENT_TABLE()

PrefsDialog::PrefsDialog(wxWindow *parent)
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  const wxSize imgsize(16, 16);
  m_imglist = new wxImageList(imgsize.GetWidth(), imgsize.GetHeight());
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_DISPLAY, wxART_OTHER, imgsize));
  m_imglist->Add(wxArtProvider::GetIcon(ART_PREFS_CPMA, wxART_OTHER, imgsize));
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
  wxPanel* cpma = create_cpma(ctrl);
  wxPanel* misc = create_misc(ctrl);
  wxPanel* advanced = create_advanced(ctrl);

  ctrl->AddPage(display, _("Display"), true, 0);
  ctrl->AddPage(cpma, _("CPMA"), false, 1);
  ctrl->AddPage(misc, _("Misc"), false, 2);
  ctrl->AddPage(advanced, _("Advanced"), false, 3);
  

  LayoutDialog();
}

PrefsDialog::~PrefsDialog()
{
  delete m_imglist;
}



wxPanel* PrefsDialog::create_display(wxWindow *parent)
{
      wxPanel* panel = new wxPanel(parent, wxID_ANY);
    // begin wxGlade: display_prefs::display_prefs
    grid_styler_staticbox = new wxStaticBox(panel, -1, wxT("Grid"));
    helper_styler_staticbox = new wxStaticBox(panel, -1, wxT("Helper"));
    ar_styler_staticbox = new wxStaticBox(panel, -1, wxT("Aspect ratio"));
    display_title = new wxStaticText(panel, wxID_ANY, wxT("Display"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT("Proportion of your ingame resolution, width:height."));
    const wxString m_view_aspectratio_choices[] = {
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)"),
        wxT("Custom")
    };
    m_view_aspectratio = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_view_aspectratio_choices, 0);
    label_1 = new wxStaticText(panel, wxID_ANY, wxT("Custom:"));
    m_view_aspectratioX = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT(":"));
    m_view_aspectratioY = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    label_4 = new wxStaticText(panel, wxID_ANY, wxT("Distance X:"));
    m_view_gridX = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_5 = new wxStaticText(panel, wxID_ANY, wxT("Distance Y:"));
    m_view_gridY = new wxSpinCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    label_6 = new wxStaticText(panel, wxID_ANY, wxT("Color:"));
    m_view_gridcolor = new wxButton(panel, ID_BTN_GRIDCOLOR, wxEmptyString);
    label_7 = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_view_gridcolor_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    label_10 = new wxStaticText(panel, wxID_ANY, wxT("Additionally drawn to help visualize the position of the\nelements better."));
    label_8 = new wxStaticText(panel, wxID_ANY, wxT("Selected Elements"));
    label_9 = new wxStaticText(panel, wxID_ANY, wxT("Background:"));
    m_view_fill_selected = new wxButton(panel, ID_BTN_FILLSEL, wxEmptyString);
    labelfa = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_view_fill_selected_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    label_11 = new wxStaticText(panel, wxID_ANY, wxT("Outline:"));
    m_view_border_selected = new wxButton(panel, ID_BTN_OUTLINESEL, wxEmptyString);
    labeloa = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_view_border_selected_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    label_8_copy = new wxStaticText(panel, wxID_ANY, wxT("Deselected Elements"));
    label_9_copy = new wxStaticText(panel, wxID_ANY, wxT("Background:"));
    m_view_fill = new wxButton(panel, ID_BTN_FILL, wxEmptyString);
    labelaeijfafe = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_view_fill_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
    label_11_copy = new wxStaticText(panel, wxID_ANY, wxT("Outline:"));
    m_view_border = new wxButton(panel, ID_BTN_OUTLINE, wxEmptyString);
    labelahosehjfija = new wxStaticText(panel, wxID_ANY, wxT("Opacity:"));
    m_view_border_alpha = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    // end wxGlade
    // begin wxGlade: display_prefs::set_properties
    display_title->SetBackgroundColour(wxColour(0, 0, 85));
    display_title->SetForegroundColour(wxColour(255, 255, 255));
    display_title->SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_3->SetForegroundColour(wxColour(0, 0, 85));
    m_view_aspectratio->SetSelection(0);
    m_view_aspectratioX->SetMinSize(wxSize(40, -1));
    m_view_aspectratioY->SetMinSize(wxSize(40, -1));
    m_view_gridX->SetMinSize(wxSize(70, -1));
    m_view_gridY->SetMinSize(wxSize(70, -1));
    label_10->SetForegroundColour(wxColour(0, 0, 85));
    label_8->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    label_8_copy->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
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
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxVERTICAL);
    sizer_8->Add(display_title, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    container->Add(sizer_8, 0, wxEXPAND, 0);
    ar_styler->Add(label_3, 0, wxALL|wxEXPAND, 3);
    ar_el_sizer->Add(m_view_aspectratio, 0, wxALL, 3);
    ar_el_sizer->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_view_aspectratioX, 0, wxALL, 3);
    ar_el_sizer->Add(label_2, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 3);
    ar_el_sizer->Add(m_view_aspectratioY, 0, wxALL, 3);
    ar_styler->Add(ar_el_sizer, 1, wxALL|wxEXPAND, 0);
    container->Add(ar_styler, 0, wxALL|wxEXPAND, 3);
    grid_el_sizer->Add(label_4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridX, 0, wxALL, 3);
    grid_el_sizer->Add(label_5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridY, 0, wxALL, 3);
    grid_el_sizer->Add(label_6, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(label_7, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_el_sizer->Add(m_view_gridcolor_alpha, 0, wxEXPAND, 0);
    grid_styler->Add(grid_el_sizer, 1, wxEXPAND, 0);
    container->Add(grid_styler, 0, wxALL|wxEXPAND, 3);
    helper_styler->Add(label_10, 0, wxALL, 3);
    helper_styler->Add(label_8, 0, wxALL, 3);
    grid_sizer_1->Add(label_9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_view_fill_selected, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(labelfa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_view_fill_selected_alpha, 0, 0, 0);
    grid_sizer_1->Add(label_11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_view_border_selected, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(labeloa, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_view_border_selected_alpha, 0, 0, 0);
    helper_styler->Add(grid_sizer_1, 0, wxEXPAND, 0);
    helper_styler->Add(label_8_copy, 0, wxALL, 3);
    grid_sizer_1_copy->Add(label_9_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_view_fill, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(labelaeijfafe, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_view_fill_alpha, 0, 0, 0);
    grid_sizer_1_copy->Add(label_11_copy, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_view_border, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(labelahosehjfija, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1_copy->Add(m_view_border_alpha, 0, 0, 0);
    helper_styler->Add(grid_sizer_1_copy, 0, wxEXPAND, 0);
    container->Add(helper_styler, 0, wxEXPAND, 0);
    // end wxGlade
    



    top_sizer->Add( container, 1, wxEXPAND|wxALIGN_CENTRE|wxALL, 5 );
    //top_sizer->AddSpacer(5);

    panel->SetSizer(top_sizer);
    top_sizer->Fit(panel);

    return panel;
}

wxPanel* PrefsDialog::create_cpma(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);
  
    // begin wxGlade: cpma_prefs::cpma_prefs
    cpma_title = new wxStaticText(panel, wxID_ANY, wxT("CPMA"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    sizer_5_staticbox = new wxStaticBox(panel, -1, wxT("Background image"));
    sizer_3_staticbox = new wxStaticBox(panel, -1, wxT("Quake3 path"));
    label_13 = new wxStaticText(panel, wxID_ANY, wxT("Where you installed Quake3 at."));
    m_q3_gamedir = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    button_2 = new wxButton(panel, ID_BTN_Q3_GAMEDIR_BROWSE, wxT("..."));
    label_12 = new wxStaticText(panel, wxID_ANY, wxT("Preferrably of width and height as power of two."));
    m_q3_background = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    button_2_copy = new wxButton(panel, ID_BTN_Q3_BACKGROUND_BROWSE, wxT("..."));
    m_q3_background_usedefault = new wxCheckBox(panel, wxID_ANY, wxT("Use default"));
    // end wxGlade
    // begin wxGlade: cpma_prefs::set_properties
    cpma_title->SetBackgroundColour(wxColour(0, 0, 85));
//    cpma_title->SetForegroundColour(wxColour(255, 255, 255));
    cpma_title->SetFont(wxFont(20, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    SetTitle(wxT("cpma_prefs"));
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

wxPanel* PrefsDialog::create_misc(wxWindow *parent)
{
  wxPanel* panel = new wxPanel(parent, wxID_ANY);

  // begin wxGlade: misc_prefs::misc_prefs
  misc_title = new wxStaticText(panel, wxID_ANY, wxT("Misc"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    sizer_2_staticbox = new wxStaticBox(panel, -1, wxT("Message dialogs"));
    sizer_7_staticbox = new wxStaticBox(panel, -1, wxT("Elements list"));
    sizer_1_staticbox = new wxStaticBox(panel, -1, wxT("Load / Save"));
    m_save_backup = new wxCheckBox(panel, wxID_ANY, wxT("Create backup before saving"));
    m_startup_load = new wxCheckBox(panel, wxID_ANY, wxT("Reload previously open HUD on startup"));
    label_14 = new wxStaticText(panel, wxID_ANY, wxT("On several occassions you are prompted a message dialog that\nyou can disable to pop up. If you want to enable them all\nagain, click the button below."));
    button_1 = new wxButton(panel, ID_BTN_RESETDIALOGS, wxT("Reset dialogs"));
    m_elements_collections = new wxCheckBox(panel, wxID_ANY, wxT("Use collections"));
    // end wxGlade
    // begin wxGlade: misc_prefs::set_properties
    SetTitle(wxT("misc_prefs"));
    label_14->SetForegroundColour(wxColour(0, 0, 85));
    misc_title->SetBackgroundColour(wxColour(0, 0, 85));
    misc_title->SetForegroundColour(wxColour(255, 255, 255));
    misc_title->SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
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
   m_pg->SetPropertyHelpString( wxT("view-snapthreshold"), _("The distance needed to snap to items") );
   m_pg->Append( new wxIntProperty(_("Move step"), wxT("view-movestep"), Prefs::get().var(wxT("view_movestep")).ival()) );
   m_pg->SetPropertyHelpString( wxT("view-movestep"), _("How many pixels to move upon pressing arrow keys") );

   m_pg->Append( new wxPropertyCategory(_("Grid color")) );
   m_pg->Append( new wxColourProperty(_("Color"), wxT("view-gridcolor"), Prefs::get().var(wxT("view_gridcolor")).wxcval()) );
   m_pg->Append( new wxIntProperty(_("Opacity"), wxT("view-gridcolor-alpha"), Prefs::get().var(wxT("view_gridcolor")).cval().a100()) );

   m_pg->Append( new wxPropertyCategory(_("Helpers color")) );
   wxPGId pid = m_pg->Append( new wxParentProperty(_("Selected elements")) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Fill"), wxT("view-helper-fill-selected"), Prefs::get().var(wxT("view_helper_fill_selected")).wxcval()) );
   m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-fill-selected-alpha"), Prefs::get().var(wxT("view_helper_fill_selected")).cval().a100()) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Border"), wxT("view-helper-border-selected"), Prefs::get().var(wxT("view_helper_border_selected")).wxcval()) );
   m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-border-selected-alpha"), Prefs::get().var(wxT("view_helper_border_selected")).cval().a100()) );
   pid = m_pg->Append( new wxParentProperty(_("Deselected elements")) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Fill"), wxT("view-helper-fill"), Prefs::get().var(wxT("view_helper_fill")).wxcval()) );
   m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-fill-alpha"), Prefs::get().var(wxT("view_helper_fill")).cval().a100()) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Border"), wxT("view-helper-border"), Prefs::get().var(wxT("view_helper_border")).wxcval()) );
   m_pg->AppendIn( pid, new wxIntProperty(_("Opacity"), wxT("view-helper-border-alpha"), Prefs::get().var(wxT("view_helper_border")).cval().a100()) );

   m_pg->Append( new wxPropertyCategory(_("Properties pane cell color")) );
   pid = m_pg->Append( new wxParentProperty(_("Default")) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Foreground"), wxT("props-color"), Prefs::get().var(wxT("props_color")).wxcval()) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Background"), wxT("props-bgcolor"), Prefs::get().var(wxT("props_bgcolor")).wxcval()) );
   pid = m_pg->Append( new wxParentProperty(_("Inherited")) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Foreground"), wxT("props-inheritcolor"), Prefs::get().var(wxT("props_inheritcolor")).wxcval()) );
   m_pg->AppendIn( pid, new wxColourProperty(_("Background"), wxT("props-inheritbgcolor"), Prefs::get().var(wxT("props_inheritbgcolor")).wxcval()) );


   m_pg->Append( new wxPropertyCategory(_("Expert")) );

   pid = m_pg->Append( new wxParentProperty(_("Elements list")) );
   wxPGProperty *child = m_pg->AppendIn( pid, new wxIntProperty(_("Collection chars match"), wxT("elements-collnamecount"), Prefs::get().var(wxT("elements_collnamecount")).ival()) );
   child->SetHelpString( _("How many characters of consecutive element names have to match for a collection to be created (Default: 3)") );
   
   pid = m_pg->Append( new wxParentProperty(_("Properties pane")) );
   child = m_pg->AppendIn( pid, new wxBoolProperty(_("Never disable properties"), wxT("props-neverdisable"), Prefs::get().var(wxT("props_neverdisable")).bval()) );
   child->SetHelpString( _("Never disable any propertes even if they would be ignored ingame (Default: NO)") );

   pid = m_pg->Append( new wxParentProperty(_("Saving")) );
   child = m_pg->AppendIn( pid, new wxBoolProperty(_("Write hidden elements"), wxT("save-writedisabled"), Prefs::get().var(wxT("save_writedisabled")).bval()) );
   child->SetHelpString( _("Not used elements are written to file but commented out (Default: YES)") );

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

void PrefsDialog::OnDefault( wxCommandEvent& )
{
  if( wxYES == wxMessageBox(_("Are you sure to reset all Advanced settings?"), _("Reset Advanced settings?"), wxYES_NO|wxICON_QUESTION) )
  {
    Prefs::get().set_default(wxT("save_writedisabled"));

    // recreate page :x (woohooh lazy)
    wxBookCtrlBase *ctrl = GetBookCtrl();
    ctrl->SetSelection(0);
    ctrl->DeletePage(3);
    wxPanel* advanced = create_advanced(ctrl);
    ctrl->AddPage(advanced, _("Advanced"), false, 3);
    ctrl->SetSelection(3);

  }
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



