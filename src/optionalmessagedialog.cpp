
#include "optionalmessagedialog.h"

#include "prefs.h"
#include "superhudeditor.h"
#include "mainframe.h"

OptionalMessageDialog::OptionalMessageDialog( const wxString& name, int saveid /*= wxID_ANY*/, wxArtID bitmapid /*= wxART_QUESTION*/ ) :
  wxDialog(),
  m_saveid(saveid),
  m_storedid( Prefs::get().var(name).intval() ),
  m_name(name),
  m_created(false),
  m_bitmapid(bitmapid)
{
}
 
bool OptionalMessageDialog::Create(wxWindow* parent, 
  const wxString& msg, const wxString& title /*=wxT("")*/, 
  int id /*=wxID_ANY*/, const wxPoint& pos /*=wxDefaultPosition*/, const wxSize& size /*=wxDefaultSize*/ )
{
  if( m_storedid != -1 || m_created )
    return true;

  wxASSERT_MSG( m_buttondefs.size() > 0, wxT("You need to add some buttons [add_button(text, id)] before you can create the OptionalMessageDialog.") );

  if( !parent )
    parent = wxGetApp().mainframe();

  if( !wxDialog::Create(parent, id, (title.empty() ? APP_NAME : title), pos, size ) )
    return false;

  // -- create objects
  m_message = new wxStaticText(this, -1, msg);
  for( std::map<wxString,int>::const_iterator cit = m_buttondefs.begin(); cit != m_buttondefs.end(); ++cit )
    m_buttons.push_back( new wxButton(this, cit->second, cit->first) );
  m_horiz_line = new wxStaticLine(this, -1);
  m_shownomore = new wxCheckBox(this, -1, _("Don't bug me again!"));
  m_buttondefs.clear();
  wxStaticBitmap *icon = new wxStaticBitmap(this, wxID_ANY, 
      wxArtProvider::GetBitmap(m_bitmapid, wxART_MESSAGE_BOX)
      );

  // -- set properties
//  SetTitle(title);

  // -- layout
  wxBoxSizer* sizer_dlg = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizer_msg = new wxBoxSizer(wxHORIZONTAL);
  sizer_msg->Add(icon, 0, wxALL, 10);
  sizer_msg->Add(m_message, 1, wxEXPAND|wxADJUST_MINSIZE|wxALL, 10);
  sizer_dlg->Add(sizer_msg);

  // expanding invisible item to make buttons align right.
  sizer_buttons->Add(20, 20, 1, wxADJUST_MINSIZE, 0);

  for( std::list<wxButton*>::const_iterator cit = m_buttons.begin(); cit != m_buttons.end(); ++cit )
  {
    sizer_buttons->Add( *cit, 0, wxALL, 10 );
    Connect( (*cit)->GetId(), wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionalMessageDialog::OnButton) );
  }
  
  sizer_dlg->Add(sizer_buttons, 0, wxEXPAND, 0);
  sizer_dlg->Add(m_horiz_line, 0, wxEXPAND, 0);
  sizer_dlg->Add(m_shownomore, 0, wxALL, 3);
  SetAutoLayout(true);
  SetSizer(sizer_dlg);
  sizer_dlg->Fit(this);
  sizer_dlg->SetSizeHints(this);
  Layout();

  m_created = true;
  return true;
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
  m_buttondefs.insert( std::make_pair(text, id) );
}

void OptionalMessageDialog::add_button_yesno()
{
  add_button(_("Yes"), wxID_YES);
  add_button(_("No"), wxID_NO);
}


