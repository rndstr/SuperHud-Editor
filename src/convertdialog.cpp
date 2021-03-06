#include "convertdialog.h"

#include "prefs.h"

// begin wxGlade: ::extracode
// end wxGlade



ConvertDialog::ConvertDialog(wxWindow* parent):
    wxDialog(parent, wxID_ANY, _("Convert HUD"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: ConvertDialog::ConvertDialog
    sizer_6_staticbox = new wxStaticBox(this, -1, wxT("Options"));
    sizer_4_staticbox = new wxStaticBox(this, -1, wxT("Conversion"));
    label_3 = new wxStaticText(this, wxID_ANY, wxT("From aspect ratio:"));
    const wxString m_aspectratio_from_choices[] = {
        wxT("Current"),
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)")
    };
    m_aspectratio_from = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_aspectratio_from_choices, 0);
    label_6 = new wxStaticText(this, wxID_ANY, wxT("(what the HUD was built for)"));
    label_4 = new wxStaticText(this, wxID_ANY, wxT("To aspect ratio:"));
    const wxString m_aspectratio_to_choices[] = {
        wxT("Current"),
        wxT("Standard (4:3)"),
        wxT("Widescreen (16:10)")
    };
    m_aspectratio_to = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, m_aspectratio_to_choices, 0);
    label_7 = new wxStaticText(this, wxID_ANY, wxT("(what you want it to be)"));
    m_size = new wxCheckBox(this, wxID_ANY, wxT("Size (keep element width:height ratio)"));
    m_stretchposition = new wxCheckBox(this, wxID_ANY, wxT("Stretch position (\"keep corner elements in the corner\")"));
    m_fontsize = new wxCheckBox(this, wxID_ANY, wxT("Fontsize (keep font width:height ratio)"));
    label_5 = new wxStaticText(this, wxID_ANY, wxT("NOTE: This will convert all fontsizes to pixelsize!"));
    button_1 = new wxButton(this, wxID_OK, wxEmptyString);
    button_2 = new wxButton(this, wxID_CANCEL, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
    
    // by default we select for the target ar the opposite
    if( Prefs::get().var(wxT("view_aspectratio")).sval() == wxT("4:3") )
    {
      m_aspectratio_to->SetSelection(CONVERT_16_10); // 16:10
    }
    else
      m_aspectratio_to->SetSelection(CONVERT_4_3); // 4:3
}

void ConvertDialog::set( bool size, bool stretchposition, bool fontsize )
{
  m_size->SetValue(size);
  m_stretchposition->SetValue(stretchposition);
  m_fontsize->SetValue(fontsize);
}
void ConvertDialog::set_conversion( int from, int to)
{
  m_aspectratio_from->SetSelection(from);
  m_aspectratio_to->SetSelection(to);
}

bool ConvertDialog::Validate()
{
  if( 
      m_aspectratio_from->GetSelection() == m_aspectratio_to->GetSelection() ||
      (m_aspectratio_from->GetSelection() == CONVERT_CURRENT && Prefs::get().var(wxT("view_aspectratio")).sval() == convert_to_str()) ||
      (m_aspectratio_to->GetSelection() == CONVERT_CURRENT && Prefs::get().var(wxT("view_aspectratio")).sval() == convert_from_str())
    )
  {
    return false;
  }
  return true;
}

double ConvertDialog::convert_from() const
{
  switch(m_aspectratio_from->GetSelection())
  {
    case CONVERT_4_3: return 4/3.0;
    case CONVERT_16_10: return 16/10.0;
    case CONVERT_CURRENT: // current
    default:
      break;
  }
  return Prefs::get().var(wxT("view_aspectratio")).dval(); // FIXME is this actually a good idea to return this? zZzzz
}

double ConvertDialog::convert_to() const
{
  switch(m_aspectratio_to->GetSelection())
  {
    case CONVERT_4_3: return 4/3.0;
    case CONVERT_16_10: return 16/10.0;
    case CONVERT_CURRENT: // current
    default:
      break;
  }
  return Prefs::get().var(wxT("view_aspectratio")).dval();
}

wxString ConvertDialog::convert_to_str() const
{
  switch(m_aspectratio_to->GetSelection())
  {
    case CONVERT_4_3: return wxT("4:3");
    case CONVERT_16_10: return wxT("16:10");
    case CONVERT_CURRENT: // current
    default:
      break;
  }
  return Prefs::get().var(wxT("view_aspectratio")).sval();
}

wxString ConvertDialog::convert_from_str() const
{
  switch(m_aspectratio_from->GetSelection())
  {
    case CONVERT_4_3: return wxT("4:3");
    case CONVERT_16_10: return wxT("16:10");
    case CONVERT_CURRENT: // current
    default:
      break;
  }
  return Prefs::get().var(wxT("view_aspectratio")).sval();
}

bool ConvertDialog::size() const
{
  return m_size->GetValue();
}

bool ConvertDialog::fontsize() const
{
  return m_fontsize->GetValue();
}
bool ConvertDialog::stretchposition() const
{
  return m_stretchposition->GetValue();
}

void ConvertDialog::set_properties()
{
    // begin wxGlade: ConvertDialog::set_properties
    SetTitle(wxT("Convert HUD"));
    m_aspectratio_from->SetSelection(0);
    m_aspectratio_to->SetSelection(0);
    m_size->SetToolTip(wxT("What "));
    // end wxGlade
}


void ConvertDialog::do_layout()
{
    // begin wxGlade: ConvertDialog::do_layout
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(sizer_6_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_4 = new wxStaticBoxSizer(sizer_4_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(2, 3, 3, 3);
    grid_sizer_1->Add(label_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(m_aspectratio_from, 0, wxALL, 3);
    grid_sizer_1->Add(label_6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_1->Add(label_4, 0, wxALL, 3);
    grid_sizer_1->Add(m_aspectratio_to, 0, wxALL, 3);
    grid_sizer_1->Add(label_7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_4->Add(grid_sizer_1, 1, wxEXPAND, 0);
    sizer_3->Add(sizer_4, 0, wxEXPAND, 0);
    sizer_6->Add(m_size, 0, wxALL|wxEXPAND, 3);
    sizer_6->Add(m_stretchposition, 0, wxALL|wxEXPAND, 3);
    sizer_6->Add(m_fontsize, 0, wxALL|wxEXPAND, 3);
    sizer_6->Add(label_5, 0, wxLEFT|wxEXPAND, 20);
    sizer_3->Add(sizer_6, 0, wxALL|wxEXPAND, 5);
    sizer_7->Add(20, 20, 1, wxEXPAND, 0);
    sizer_7->Add(button_1, 0, wxALL, 3);
    sizer_7->Add(button_2, 0, wxALL, 3);
    sizer_3->Add(sizer_7, 0, wxALL|wxEXPAND, 5);
    sizer_2->Add(sizer_3, 1, wxALL|wxEXPAND, 5);
    SetSizer(sizer_2);
    sizer_2->Fit(this);
    Layout();
    // end wxGlade
}





























