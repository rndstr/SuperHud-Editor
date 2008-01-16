// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <wx/wx.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <wx/image.h>

class wxSpinCtrl;
class wxPanel;
class wxWindow;
class wxImageList;
class wxPropertyGrid;

class PrefsDialog : public wxPropertySheetDialog
{
  public:
    PrefsDialog( wxWindow *parent );
    ~PrefsDialog();

  protected:
    wxImageList *m_imglist;
    wxPanel* create_display(wxWindow *parent);
    wxPanel* create_cpma(wxWindow *parent);
    wxPanel* create_misc(wxWindow *parent);
    wxPanel* create_advanced(wxWindow *parent);
    wxPanel* create_sample(wxWindow *parent);
    wxPanel* create_old(wxWindow *parent);

    void OnDefault( wxCommandEvent& );

  private:
    wxPropertyGrid *m_pg;
    // --- display
        // begin wxGlade: display_prefs::attributes
    wxStaticBox* helper_styler_staticbox;
    wxStaticBox* grid_styler_staticbox;
    wxStaticBox* ar_styler_staticbox;
    wxStaticText* display_title;
    wxStaticText* label_3;
    wxChoice* m_view_aspectratio;
    wxStaticText* label_1;
    wxTextCtrl* m_view_aspectratioX;
    wxStaticText* label_2;
    wxTextCtrl* m_view_aspectratioY;
    wxStaticText* label_4;
    wxSpinCtrl* m_view_gridX;
    wxStaticText* label_5;
    wxSpinCtrl* m_view_gridY;
    wxStaticText* label_6;
    wxButton* m_view_gridcolor;
    wxStaticText* label_7;
    wxSlider* m_view_gridcolor_alpha;
    wxStaticText* label_10;
    wxStaticText* label_8;
    wxStaticText* label_9;
    wxButton* m_view_fill_selected;
    wxStaticText* labelfa;
    wxSlider* m_view_fill_selected_alpha;
    wxStaticText* label_11;
    wxButton* m_view_border_selected;
    wxStaticText* labeloa;
    wxSlider* m_view_border_selected_alpha;
    wxStaticText* label_8_copy;
    wxStaticText* label_9_copy;
    wxButton* m_view_fill;
    wxStaticText* labelaeijfafe;
    wxSlider* m_view_fill_alpha;
    wxStaticText* label_11_copy;
    wxButton* m_view_border;
    wxStaticText* labelahosehjfija;
    wxSlider* m_view_border_alpha;
    // end wxGlade

    // --- cpma
        // begin wxGlade: cpma_prefs::attributes
    wxStaticText* cpma_title;
    wxStaticBox* sizer_5_staticbox;
    wxStaticBox* sizer_3_staticbox;
    wxStaticText* label_13;
    wxTextCtrl* m_q3_gamedir;
    wxButton* button_2;
    wxStaticText* label_12;
    wxTextCtrl* m_q3_background;
    wxButton* button_2_copy;
    wxCheckBox* m_q3_background_usedefault;
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
    // end wxGlade

    // --- advanced
    // begin wxGlade: advanced_prefs::attributes
    wxStaticText* advanced_title;
    wxStaticText* m_view_dragthreshold;
    wxSpinCtrl* spin_ctrl_1;
    wxStaticText* label_15;
    wxSpinCtrl* m_view_snapthreshold;
    // end wxGlade

    DECLARE_EVENT_TABLE()
};


#endif // PREFSDIALOG_H

