#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include <wx/frame.h>
#include <wx/aui/aui.h>

class MainFrame : public wxFrame
{
  public:
    MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, 
        const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MainFrame();
    wxAuiDockArt* GetDockArt();
    void DoUpdate();

  protected:
    wxWindow* create_cpma_elementsctrl();
    wxWindow* create_cpma_displayctrl();

    void set_floating_hint( wxAuiManagerOption hint );

  private:

    void OnMenuExit( wxCommandEvent& );
    void OnMenuNew( wxCommandEvent& );
    void OnMenuOpen( wxCommandEvent& );

  private:
    wxAuiManager m_mgr;


   DECLARE_EVENT_TABLE()
};


#endif // __MAINFRAME_H__
