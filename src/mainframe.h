#ifndef MAINFRAME_H
#define MAINFRAME_H


#include <wx/frame.h>
#include <wx/aui/aui.h>

/// statusbar column
const int SB_MSG = 0;
const int SB_ELEMENT = 1;
const int SB_MOUSEPOS = 2;

class wxTextCtrl;
class ElementsCtrlBase;
class PropertiesNotebookBase;
class DisplayCtrlBase;
class Model;

class MainFrame : public wxFrame
{
  public:
    MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, 
        const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MainFrame();
    wxAuiDockArt* GetDockArt();
    void DoUpdate();

    void update_title();

    


    ElementsCtrlBase*   elementsctrl() { return m_elementsctrl; }
    PropertiesNotebookBase* propertiesctrl() { return m_propertiesnotebook; }
    DisplayCtrlBase*    displayctrl() { return m_displayctrl; }
    wxTextCtrl*         configpreview() { return m_configpreview; }
    wxStatusBar*        statusbar() { return m_statusbar; }
    Model*              model() { return m_model; }

    /// Selection changed in ElementsCtrl
    void                OnElementSelectionChanged();
    /// Properties changed in PropertiesCtrl
    void                OnPropertiesChanged();

    /// several functions to update certain stuff
    /// @{
    void                update_configpreview();
    void                update_displayctrl();
    void                update_elementsctrl();
    void                update_propertiesctrl();
    /// @}

    
    void OnMenuHelpTip( wxCommandEvent& );

  protected:
    void set_floating_hint( wxAuiManagerOption hint );
    
    int  confirm_savechanges_dlg();
    /// call this if you are about to clear the current hudfile (e.g. closing the app,
    /// opening new file, etc). it does all the checks and prompts
    /// @return False if abort
    bool confirm_saving();

    void restart_app();

    wxMenu            *m_view_menu;
    wxStatusBar       *m_statusbar;
    wxToolBar         *m_toolbar_file;

    ElementsCtrlBase  *m_elementsctrl;
    PropertiesNotebookBase *m_propertiesnotebook;
    wxTextCtrl        *m_configpreview;
    DisplayCtrlBase   *m_displayctrl;
    Model             *m_model;

  private:

    void OnClose( wxCloseEvent& ev );

    void OnMenuAbout( wxCommandEvent& );
    void OnMenuExit( wxCommandEvent& );
    void OnMenuNew( wxCommandEvent& );
    void OnMenuOpen( wxCommandEvent& );
    void OnMenuSave( wxCommandEvent& );
    void OnMenuSaveAs( wxCommandEvent& );
    void OnMenuToolsPreferences( wxCommandEvent& );
    void OnMenuToolsSnapElements( wxCommandEvent& );
    void OnMenuToolsSnapGrid( wxCommandEvent& );
    void OnMenuViewDefaultPerspective( wxCommandEvent& );
    void OnMenuViewConfigPreview( wxCommandEvent& );
    void OnMenuViewToolbarFile( wxCommandEvent& );
    void OnMenuToolsSwitchGame( wxCommandEvent& );
    void OnMenuViewGrid( wxCommandEvent& );
    void OnUpdateViewPanes( wxUpdateUIEvent& );

    void OnMenuHelpUpdate( wxCommandEvent& );

  private:
    wxAuiManager m_mgr;
    wxString     m_defaultperspective;


   DECLARE_EVENT_TABLE()
};


#endif // MAINFRAME_H
