
#include "mainframe.h"

#include <wx/artprov.h>
#include <wx/menu.h>

#include "common.h"
#include "cpma/elementsctrl.h"
#include "cpma/displayctrl.h"
#include "cpma/hudfile.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MainFrame::OnMenuExit)
  EVT_MENU(wxID_OPEN, MainFrame::OnMenuOpen)
  EVT_MENU(wxID_NEW, MainFrame::OnMenuNew)
END_EVENT_TABLE()




MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, 
        const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/,
        long style /*= wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER*/) :
  wxFrame(parent, id, title, pos, size, style)
{

  m_mgr.SetManagedWindow(this);

  // create menu
  wxMenuBar *menu_bar = new wxMenuBar;

  wxMenu *file_menu = new wxMenu;
  file_menu->Append( wxID_NEW, _("&New\tCtrl+N") );
  file_menu->Append( wxID_OPEN, _("&Open\tCtrl+O") );
  file_menu->AppendSeparator();
  file_menu->Append( wxID_EXIT, _("E&xit\tCtrl+Q") );

  wxMenu *elements_menu = new wxMenu;

  wxMenu *help_menu = new wxMenu;
  help_menu->Append( wxID_ABOUT, _("About") );

  menu_bar->Append( file_menu, _("File") );
  menu_bar->Append( elements_menu, _("Elements") );
  menu_bar->Append( help_menu, _("Help") );

  SetMenuBar( menu_bar );

  // statusbar plz
  CreateStatusBar();
  GetStatusBar()->SetStatusText(_("Ready"));

  // create toolbar
  wxToolBar *tool_bar_file = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
  tool_bar_file->SetToolBitmapSize(wxSize(16,16));
  tool_bar_file->AddTool( wxID_NEW, _("New"), wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_OTHER, wxSize(16,16)) );
  tool_bar_file->AddTool( wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16,16)) );
  tool_bar_file->Realize();
  

  wxToolBar *tool_bar_game = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORZ_TEXT);
  tool_bar_file->SetToolBitmapSize(wxSize(16,16));
  tool_bar_game->AddTool( IDM_GAME_CPMA, wxT("CPMA"), wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_OTHER, wxSize(16,16)) );
  tool_bar_game->AddTool( IDM_GAME_Q4MAX, wxT("Q4MAX"), wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_OTHER, wxSize(16,16)) );
  tool_bar_game->Realize();
 


  // create panes
  m_mgr.AddPane( new ElementsCtrlBase(this, wxID_ANY),
      wxAuiPaneInfo().Name(wxT("elements")).Caption(_("Elements")).CloseButton(true).MaximizeButton(true)
      );
  m_mgr.AddPane( create_cpma_displayctrl(), 
      wxAuiPaneInfo().Name(wxT("display")).Caption(_("Display")).MaximizeButton(true).
      CenterPane()
      );
  m_mgr.AddPane(tool_bar_file, wxAuiPaneInfo().Name(wxT("tb-file")).Caption(_("File")).
	  ToolbarPane().Top().Row(1).Position(1).LeftDockable(false).RightDockable(false)
	  );
  m_mgr.AddPane(tool_bar_game, wxAuiPaneInfo().Name(wxT("tb-game")).Caption(_("Game selection")).
	  ToolbarPane().Top().Row(1).Position(2).LeftDockable(false).RightDockable(false)
	  );

  // default transparency hints throw assertions all over the place
  // on linux
#ifndef WIN32
  set_floating_hint( wxAUI_MGR_RECTANGLE_HINT );
#endif
  m_mgr.Update();
}

wxWindow* MainFrame::create_cpma_elementsctrl()
{
  CPMAElementsCtrl *ctrl = new CPMAElementsCtrl(this);

  return ctrl;
}

wxWindow* MainFrame::create_cpma_displayctrl()
{
  CPMADisplayCtrl *ctrl = new CPMADisplayCtrl(this);

  return ctrl;
}

void MainFrame::set_floating_hint( wxAuiManagerOption hint )
{
  // disable all hints
  unsigned int f = m_mgr.GetFlags();
  f &= ~wxAUI_MGR_TRANSPARENT_HINT;
  f &= ~wxAUI_MGR_VENETIAN_BLINDS_HINT;
  f &= ~wxAUI_MGR_RECTANGLE_HINT;

  f |= hint;
  m_mgr.SetFlags(f);
}

void MainFrame::OnMenuExit( wxCommandEvent& )
{
  Close(true);
}

void MainFrame::OnMenuNew( wxCommandEvent& )
{

}

void MainFrame::OnMenuOpen( wxCommandEvent& )
{
  wxLogDebug(wxT("open"));
  int ret = wxID_OK;
  wxFileDialog dlg(
      this,
      _("Open..."),
      wxT(""),
      wxT(""),
      wxT("Hud Files (*.cfg)|*.cfg|All Files (*.*)|*.*"),
#if wxCHECK_VERSION(2,7,0)
      wxFD_OPEN | wxFD_FILE_MUST_EXIST
#else
      wxOPEN | wxFILE_MUST_EXIST
#endif
      );
  if( wxID_OK == (ret = dlg.ShowModal()) )
  {
  }

}

MainFrame::~MainFrame()
{
  m_mgr.UnInit();
}

wxAuiDockArt* MainFrame::GetDockArt()
{
  return m_mgr.GetArtProvider();
}

void MainFrame::DoUpdate()
{
  m_mgr.Update();
}

