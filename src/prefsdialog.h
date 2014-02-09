#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include "common.h"

#include <wx/wx.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <wx/image.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class wxSpinCtrl;
class wxPanel;
class wxWindow;
class wxImageList;
class wxPropertyGrid;


/// we store this here to make sure if more pages will be added we
/// not accidently refresh (delete&insert) the wrong page... as i
/// can't figure out how to get index from pagename, yet.
// so ATTENTION this depends on ENABLE_CPMA ... .... :x
const int ADVANCED_IDX = 4;

class PrefsDialog : public wxPropertySheetDialog
{
  public:
    PrefsDialog( wxWindow *parent );
    ~PrefsDialog();

  protected:
    wxImageList *m_imglist;
    wxPanel* create_display(wxWindow *parent);
#if ENABLE_CPMA
    wxPanel* create_cpma(wxWindow *parent);
#endif
    wxPanel* create_network(wxWindow *parent);
    wxPanel* create_misc(wxWindow *parent);
    wxPanel* create_advanced(wxWindow *parent);

    void  fill_defaults();

    void OnOk( wxCommandEvent& );
    void OnAdvancedDefault( wxCommandEvent& );
#if ENABLE_CPMA
    void OnCheckQ3BackgroundUseDefault( wxCommandEvent& );
#endif
    void OnChoiceAspectratio( wxCommandEvent& );

  private:
    
    // --- display
    // begin wxGlade: display_prefs::attributes
    wxStaticBox* grid_styler_staticbox;
    wxStaticBox* ar_styler_staticbox;
    wxStaticText* display_title;
    wxStaticText* label_3;
    wxChoice* m_view_aspectratio;
    wxStaticText* label_1;
    wxTextCtrl* m_view_aspectratio_custom;
    wxStaticText* label_2;
    wxStaticText* label_7;
    wxStaticText* label_4;
    wxSpinCtrl* m_view_gridX;
    wxStaticText* label_5;
    wxSpinCtrl* m_view_gridY;
    // end wxGlade
        

#if ENABLE_CPMA
    // --- cpma
    // begin wxGlade: cpma_prefs::attributes
    wxStaticBox* sizer_5_staticbox;
    wxStaticBox* sizer_3_staticbox;
    wxStaticText* cpma_title;
    wxStaticText* label_13;
    wxTextCtrl* m_q3_gamedir;
    wxButton* button_2;
    wxStaticText* label_12;
    wxTextCtrl* m_q3_background;
    wxButton* button_2_copy;
    wxCheckBox* m_q3_background_usedefault;
    // end wxGlade
#endif
        
    // --- network
    // begin wxGlade: network_prefs::attributes
    wxStaticBox* sizer_13_staticbox;
    wxStaticBox* sizer_12_staticbox;
    wxStaticText* network_title;
    wxCheckBox* m_startup_checkforupdate;
    wxTextCtrl* m_net_proxy;
    wxStaticText* label_8;
    // end wxGlade

    // --- misc
    // begin wxGlade: misc_prefs::attributes
    wxStaticBox* sizer_7_staticbox;
    wxStaticBox* sizer_2_staticbox;
    wxStaticBox* sizer_1_staticbox;
    wxStaticText* misc_title;
    wxCheckBox* m_save_backup;
    wxCheckBox* m_startup_load;
    wxStaticText* label_14;
    wxButton* button_1;
    wxCheckBox* m_elements_collections;
    wxStaticText* label_6;
    // end wxGlade
    

    // --- advanced
    // all those that have parents.. dunno how to access those values otherwise? :x
    wxPropertyGrid *m_pg;
    wxPGProperty *m_vgc;
    wxPGProperty *m_vgca;
    wxPGProperty *m_vhfs;
    wxPGProperty *m_vhfsa;
    wxPGProperty *m_vhbs;
    wxPGProperty *m_vhbsa;
    wxPGProperty *m_vhf;
    wxPGProperty *m_vhfa;
    wxPGProperty *m_vhb;
    wxPGProperty *m_vhba;
    wxPGProperty *m_pcol;
    wxPGProperty *m_pbgcol;
    wxPGProperty *m_picol;
    wxPGProperty *m_pibgcol;
    wxPGProperty *m_ec;
    wxPGProperty *m_pnd;
    wxPGProperty *m_swd;



    DECLARE_EVENT_TABLE()
};


#endif // PREFSDIALOG_H

