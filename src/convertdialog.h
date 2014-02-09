#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
// end wxGlade


#ifndef CONVERTDIALOG_H
#define CONVERTDIALOG_H


// indecies from wxChoice
enum
{
  CONVERT_CURRENT = 0,
  CONVERT_4_3 = 1,
  CONVERT_16_10 = 2
};


// begin wxGlade: ::extracode
// end wxGlade


class ConvertDialog: public wxDialog {
public:
    // begin wxGlade: ConvertDialog::ids
    // end wxGlade

    ConvertDialog(wxWindow* parent);


    double  convert_from() const;
    double  convert_to() const;
    wxString convert_to_str() const;
    wxString convert_from_str() const;
    bool    size() const;
    bool    stretchposition() const;
    bool    fontsize() const;

    void    set( bool size, bool stretchposition, bool fontsize );
    void    set_conversion( int from, int to);

private:
    virtual bool Validate();
    // begin wxGlade: ConvertDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ConvertDialog::attributes
    wxStaticBox* sizer_6_staticbox;
    wxStaticBox* sizer_4_staticbox;
    wxStaticText* label_3;
    wxChoice* m_aspectratio_from;
    wxStaticText* label_6;
    wxStaticText* label_4;
    wxChoice* m_aspectratio_to;
    wxStaticText* label_7;
    wxCheckBox* m_size;
    wxCheckBox* m_stretchposition;
    wxCheckBox* m_fontsize;
    wxStaticText* label_5;
    wxButton* button_1;
    wxButton* button_2;
    // end wxGlade
}; // wxGlade: end class


#endif // CONVERTDIALOG_H
