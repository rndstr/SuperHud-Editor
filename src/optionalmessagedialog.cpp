#include "optionalmessagedialog.h"

#include "prefs.h"
#include "superhudeditor.h"
#include "mainframe.h"


OptionalMessageDialog::OptionalMessageDialog( const wxString& name, int saveid /*= wxID_ANY*/, int default_button /*=wxID_ANY*/, wxArtID bitmapid /*= wxART_QUESTION*/ ) :
  wxDialog(),
  m_saveid(saveid),
  m_storedid( Prefs::get().var(name).ival() ),
  m_name(name),
  m_created(false),
  m_bitmapid(bitmapid),
  m_default_button(default_button)
{
}
 
bool OptionalMessageDialog::Create(wxWindow* parent, 
  const wxString& msg, const wxString& title /*=wxT("")*/, 
  int id /*=wxID_ANY*/, const wxPoint& pos /*=wxDefaultPosition*/, const wxSize& size /*=wxDefaultSize*/ )
{
  wxWindow *defaultitem = 0;

  if( m_storedid != -1 || m_created )
    return true;

  wxASSERT_MSG( m_buttoninfos.size() > 0, wxT("You need to add some buttons [add_button(text, id)] before you can create the OptionalMessageDialog.") );

  if( !parent )
    parent = wxGetApp().mainframe();

  if( !wxDialog::Create(parent, id, (title.empty() ? APP_NAME : title), pos, size ) )
    return false;

  // -- create objects
  m_message = new wxStaticText(this, wxID_ANY, msg);
  for( std::vector<buttoninfo_s>::const_iterator cit = m_buttoninfos.begin(); cit != m_buttoninfos.end(); ++cit )
  {
    wxButton *but = new wxButton(this, cit->id, cit->label);
    if( wxID_CANCEL == cit->id && m_default_button != wxID_CANCEL )
    {
      wxASSERT_MSG(false, wxT("button with id wxID_CANCEL found that is not the default action"));
    }
    m_buttons.push_back( but );
  }
  m_horiz_line = new wxStaticLine(this, wxID_ANY);
  m_shownomore = new wxCheckBox(this, wxID_ANY, _("Don't bug me again!"));
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
  sizer_msg->Add(icon, 0, wxALL, 5);
  sizer_msg->Add(m_message, 1, wxEXPAND|wxADJUST_MINSIZE|wxALL, 5);
  sizer_dlg->Add(sizer_msg);

  // expanding invisible item to make buttons align right.
  sizer_buttons->Add(20, 20, 1, wxADJUST_MINSIZE, 0);

  for( std::vector<wxButton*>::const_iterator it = m_buttons.begin(); it != m_buttons.end(); ++it )
  {
    sizer_buttons->Add( *it, 0, wxALL, 3 );
    if( (*it)->GetId() == m_default_button )
      defaultitem = *it;
    Connect( (*it)->GetId(), wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionalMessageDialog::OnButton) );
  }
  
  if( defaultitem )
  {
    SetDefaultItem(defaultitem); // this doesn't look like it does something on win32
    defaultitem->SetFocus(); // this works yay
  }

  sizer_dlg->Add(sizer_buttons, 0, wxEXPAND|wxALL, 5);
  sizer_dlg->Add(m_horiz_line, 0, wxEXPAND, 0);
  sizer_dlg->Add(m_shownomore, 0, wxALL, 3);
  top_sizer->Add(sizer_dlg, 0, wxALL, 10);
  SetAutoLayout(true);
  SetSizer(top_sizer);
  top_sizer->Fit(this);
  top_sizer->SetSizeHints(this);
  Layout();

  m_created = true;
  return true;
}


void OptionalMessageDialog::EndModal(int retCode)
{
  wxDialog::EndModal( retCode == wxID_CANCEL ? m_default_button : retCode );
}
void OptionalMessageDialog::OnButton( wxCommandEvent& ev )
{
  EndModal(ev.GetId());
}

int OptionalMessageDialog::ShowModal()
{
  if( m_storedid != -1 )
    return m_storedid;

  wxASSERT( m_created );

  int ret = wxDialog::ShowModal();
  if( m_shownomore->GetValue() )
    Prefs::get().seti(m_name, (m_saveid == wxID_ANY ? ret : m_saveid));
  return ret;
}


bool OptionalMessageDialog::Show( bool show /*=true*/ )
{
  if( m_storedid != -1 )
    return IsShown();

  wxASSERT( m_created );
  return wxDialog::Show(show);
}

void OptionalMessageDialog::set_message( const wxString& msg )
{
  m_message->SetLabel( msg );
  Layout();
}

void OptionalMessageDialog::add_button( const wxString& text, int id )
{
  m_buttoninfos.push_back( buttoninfo_s(text, id) );
}

void OptionalMessageDialog::add_button_yesno()
{
  add_button(wxEmptyString, wxID_YES);
  add_button(wxEmptyString, wxID_NO);
}


