#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "hudfilebase.h"

#include <wx/frame.h>
#include <wx/aui/aui.h>

class ElementsCtrlBase;

class MainFrame : public wxFrame
{
  public:
    MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, 
        const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MainFrame();
    wxAuiDockArt* GetDockArt();
    void DoUpdate();

    void OnMenuGameSelection( wxCommandEvent& );


    ElementsCtrlBase* elementsctrl() { return m_elementsctrl; }

  protected:
    void set_floating_hint( wxAuiManagerOption hint );

    ElementsCtrlBase *m_elementsctrl;

  private:

    void OnClose( wxCloseEvent& ev );

    void OnMenuExit( wxCommandEvent& );
    void OnMenuNew( wxCommandEvent& );
    void OnMenuOpen( wxCommandEvent& );
    void OnMenuPreferences( wxCommandEvent& );

  private:
    wxAuiManager m_mgr;


   DECLARE_EVENT_TABLE()
};


#endif // __MAINFRAME_H__
