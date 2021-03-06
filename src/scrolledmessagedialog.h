#ifndef SCROLLEDMESSAGEDIALOG_H
#define SCROLLEDMESSAGEDIALOG_H

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/artprov.h>
#include <wx/textctrl.h>

#include <wx/statline.h>

#include <map>
#include <vector>

/// NOTE that you shouldn't misuse wxID_CANCEL because if a user
/// closes with wxCLOSE_BOX wxID_CANCEL is sent and we catch this
/// in EVT_CLOSE and return default button!
/// -> you should only use wxID_CANCEL if that is the default
/// action..
/// ALSO make sure you select an aborting default button....
class ScrolledMessageDialog : public wxDialog
{
  struct buttoninfo_s
  {
    buttoninfo_s( const wxString& l, int i ) : label(l), id(i) {}
    wxString label;
    int id;
  };
  public:


    /// @arg name         An unique name
    /// @arg saveid       Which id we store (in prefs) if a user decides to not show it anymore, use wxID_ANY for the user's pressed button
    ScrolledMessageDialog( int default_button = wxID_ANY, wxArtID bitmapid = wxART_QUESTION );
    /// create it.
    virtual bool Create(wxWindow* parent, const wxString& msg, const wxString& text, const wxString& title = wxT(""), 
      int id = wxID_ANY, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize );

    virtual int ShowModal();
    virtual bool Show( bool show =true );

    void set_message( const wxString& msg );

    // wxGTK button order is No|Cancel|Yes
    void add_button( const wxString& text, int id );
    void add_button_yesno();

  protected:
    void OnButton( wxCommandEvent& ev );
    void EndModal(int retCode);

  private:
    void set_properties();
    void do_layout();

  protected:
    bool m_created; ///< in ShowModal and Show the dialog must have been created, we track it with this.

    wxArtID m_bitmapid;

    int m_default_button; ///< the id of the default button (i.e. the one that will be selected)

    // displayedname -> id
    std::vector<buttoninfo_s> m_buttoninfos;
    std::vector<wxButton*> m_buttons;

    wxStaticText* m_message;
    wxTextCtrl*    m_text;
};


#endif
