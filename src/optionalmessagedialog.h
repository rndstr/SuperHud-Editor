#ifndef OPTIONALMESSAGEDIALOG_H
#define OPTIONALMESSAGEDIALOG_H

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/artprov.h>

#include <wx/statline.h>

#include <map>
#include <vector>

/// NOTE that you shouldn't misuse wxID_CANCEl because if a user
/// closes with wxCLOSE_BOX wxID_CANCEL is sent and we catch this
/// in EVT_CLOSE and return default button!
/// ALSO make sure you select an aborting default button....
class OptionalMessageDialog: public wxDialog
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
    OptionalMessageDialog( const wxString& name, int saveid = wxID_ANY, int default_button = wxID_ANY, wxArtID bitmapid = wxART_QUESTION );
    /// create it.
    virtual bool Create(wxWindow* parent, const wxString& msg, const wxString& title = wxT(""), int id = wxID_ANY, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize );

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
    int   m_saveid;
    int   m_storedid;
    wxString m_name; ///< unique name this dialog is identified with, also used in prefs saving.
  
    bool m_created; ///< in ShowModal and Show the dialog must have been created, we track it with this.

    wxArtID m_bitmapid;

    int m_default_button; ///< the id of the default button (i.e. the one that will be selected)

    // displayedname -> id
    std::vector<buttoninfo_s> m_buttoninfos;
    std::vector<wxButton*> m_buttons;

    wxStaticText* m_message;
    wxStaticLine* m_horiz_line;
    wxCheckBox* m_shownomore;
};


#endif // OPTIONALMESSAGEDIALOG_H
