
#include "scrolledmessagedialog.h"

#include "common.h"
#include "prefs.h"

ScrolledMessageDialog::ScrolledMessageDialog( int default_button /*=wxID_ANY*/, wxArtID bitmapid /*= wxART_QUESTION*/ ) :
  wxDialog(),
  m_created(false),
  m_bitmapid(bitmapid),
  m_default_button(default_button)
{
}
 
bool ScrolledMessageDialog::Create(wxWindow* parent, 
  const wxString& msg, const wxString& text, const wxString& title /*=wxT("")*/, 
  int id /*=wxID_ANY*/, const wxPoint& pos /*=wxDefaultPosition*/, const wxSize& size /*=wxDefaultSize*/ )
{
  if( m_created )
    return true;

  wxASSERT_MSG( m_buttoninfos.size() > 0, wxT("You need to add some buttons [add_button(text, id)] before you can create the OptionalMessageDialog.") );

  if( !parent )
    parent = wxGetApp().mainframe();

  if( !wxDialog::Create(parent, id, (title.empty() ? APP_NAME : title), pos, size ) )
    return false;

  // -- create objects
  m_message = new wxStaticText(this, wxID_ANY, msg);
  m_text = new wxTextCtrl(this, wxID_ANY, text, wxDefaultPosition, wxSize(500,250), 
    wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_AUTO_URL);

  wxFont sysfont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
  sysfont.SetFaceName( wxT("Courier New") );
  m_text->SetFont( sysfont );
  wxStaticText *stitle = new wxStaticText(this, wxID_ANY, _("Information"));
  stitle->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));

  for( std::vector<buttoninfo_s>::const_iterator cit = m_buttoninfos.begin(); cit != m_buttoninfos.end(); ++cit )
  {
    wxButton *but = new wxButton(this, cit->id, cit->label);
    if( cit->id == m_default_button )
      but->SetDefault();
    m_buttons.push_back( but );
  }
  m_buttoninfos.clear();
  wxStaticBitmap *icon = new wxStaticBitmap(this, wxID_ANY, 
      wxArtProvider::GetBitmap(m_bitmapid, wxART_MESSAGE_BOX)
      );

  // -- set properties
//  SetTitle(title);

  // -- layout
  wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer_dlg = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizer_msg = new wxBoxSizer(wxHORIZONTAL);
  sizer_msg->Add(icon, 0, wxALL, 3);
  sizer_msg->Add(m_message, 1, wxEXPAND|wxADJUST_MINSIZE|wxALL, 3);
  sizer_dlg->Add(sizer_msg);
  sizer_dlg->Add(stitle, 0, wxALL, 3);
  sizer_dlg->Add(m_text, 1, wxEXPAND|wxALL, 3);

  // expanding invisible item to make buttons align right.
  sizer_buttons->Add(20, 20, 1, wxADJUST_MINSIZE, 0);

  for( std::vector<wxButton*>::const_iterator it = m_buttons.begin(); it != m_buttons.end(); ++it )
  {
    sizer_buttons->Add( *it, 0, wxALL, 10 );
    Connect( (*it)->GetId(), wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ScrolledMessageDialog::OnButton) );
  }
  
  sizer_dlg->Add(sizer_buttons, 0, wxEXPAND, 0);
  top_sizer->Add(sizer_dlg, 0, wxALL, 10);
  SetAutoLayout(true);
  SetSizer(top_sizer);
  top_sizer->Fit(this);
  top_sizer->SetSizeHints(this);
  Layout();

  m_created = true;
  return true;
}


void ScrolledMessageDialog::OnButton( wxCommandEvent& ev )
{
  EndModal(ev.GetId());
}

int ScrolledMessageDialog::ShowModal()
{
  wxASSERT( m_created );
  return wxDialog::ShowModal();
}


bool ScrolledMessageDialog::Show( bool show /*=true*/ )
{
  wxASSERT( m_created );
  return wxDialog::Show(show);
}

void ScrolledMessageDialog::set_message( const wxString& msg )
{
  m_message->SetLabel( msg );
  Layout();
}

void ScrolledMessageDialog::add_button( const wxString& text, int id )
{
  m_buttoninfos.push_back( buttoninfo_s(text, id) );
}

void ScrolledMessageDialog::add_button_yesno()
{
  add_button(_("Yes"), wxID_YES);
  add_button(_("No"), wxID_NO);
}


