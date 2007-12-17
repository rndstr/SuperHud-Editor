#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "hudfilebase.h"

#include <wx/frame.h>
#include <wx/aui/aui.h>

class ElementsCtrlBase;
class PropertiesNotebookBase;

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
    PropertiesNotebookBase* propertiesnotebook() { return m_propertiesnotebook; }
    wxTextCtrl*         configpreview() { return m_configpreview; }

    /// Selection changed in ElementsCtrl
    void                OnElementSelectionChanged();
    /// Properties changed in PropertiesCtrl
    void                OnPropertiesChanged();

  protected:
    void set_floating_hint( wxAuiManagerOption hint );
    void update_configpreview();

    wxMenu            *m_view_menu;

    ElementsCtrlBase  *m_elementsctrl;
    PropertiesNotebookBase *m_propertiesnotebook;
    wxTextCtrl        *m_configpreview;

  private:

    void OnClose( wxCloseEvent& ev );

    void OnMenuAbout( wxCommandEvent& );
    void OnMenuExit( wxCommandEvent& );
    void OnMenuNew( wxCommandEvent& );
    void OnMenuOpen( wxCommandEvent& );
    void OnMenuSave( wxCommandEvent& );
    void OnMenuSaveAs( wxCommandEvent& );
    void OnMenuPreferences( wxCommandEvent& );
    void OnMenuDefaultPerspective( wxCommandEvent& );
    void OnMenuConfigPreview( wxCommandEvent& );
    void OnMenuSwitchGame( wxCommandEvent& );

  private:
    wxAuiManager m_mgr;
    wxString     m_defaultperspective;


   DECLARE_EVENT_TABLE()
};


#endif // __MAINFRAME_H__
