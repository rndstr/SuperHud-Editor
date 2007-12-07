
#include "mainframe.h"

#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/stdpaths.h>

#include "common.h"
#include "GameSelectionDialog.h"
#include "factorybase.h"

#include "cpma/elementsctrl.h"
#include "cpma/displayctrl.h"
#include "cpma/hudfile.h"

DECLARE_APP(SHEApp);

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MainFrame::OnMenuExit)
  EVT_MENU(wxID_OPEN, MainFrame::OnMenuOpen)
  EVT_MENU(wxID_NEW, MainFrame::OnMenuNew)
  EVT_MENU(ID_MENU_TOOLS_GAMESELECTION, MainFrame::OnMenuGameSelection)
  EVT_MENU(ID_MENU_TOOLS_PREFERENCES, MainFrame::OnMenuPreferences)
  EVT_MENU(ID_MENU_VIEW_DEFAULTPERSPECTIVE, MainFrame::OnMenuDefaultPerspective)
  EVT_CLOSE(MainFrame::OnClose)
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
  menu_bar->Append( file_menu, _("&File") );

  wxMenu *tools_menu = new wxMenu;
  //tools_menu->Append( ID_MENU_TOOLS_GAMESELECTION, _("&Switch game...") );
  //tools_menu->AppendSeparator();
  tools_menu->Append( ID_MENU_TOOLS_PREFERENCES, _("&Preferences\tAlt+P") );
  menu_bar->Append( tools_menu, _("&Tools") );

  wxMenu *elements_menu = new wxMenu;
  menu_bar->Append( elements_menu, _("&Elements") );

  wxMenu *view_menu= new wxMenu;
  view_menu->Append( ID_MENU_VIEW_DEFAULTPERSPECTIVE, _("Default Perspective") );
  //view_menu->AppendSeparator();
  //view_menu->Append( ID_MENU_VIEW_TEXTOUTPUT, _("Text output") ); // FIXME make this checkboxable
  menu_bar->Append( view_menu, _("&View") );

  wxMenu *help_menu = new wxMenu;
  help_menu->Append( wxID_ABOUT, _("&About") );
  menu_bar->Append( help_menu, _("Help") );

  SetMenuBar( menu_bar );

  // statusbar plz
  CreateStatusBar();
  GetStatusBar()->SetStatusText(_("Ready"));

  // create toolbar
  wxToolBar *tool_bar_file = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
  tool_bar_file->SetToolBitmapSize(wxSize(16,16));
  tool_bar_file->AddTool( wxID_NEW, _("New"), wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_TOOLBAR, wxSize(16,16)) );
  tool_bar_file->AddTool( wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(16,16)) );
  tool_bar_file->Realize();
 


  // create panes
  m_elementsctrl = wxGetApp().factory()->create_elementsctrl(this);
  m_mgr.AddPane( m_elementsctrl,
      wxAuiPaneInfo().Name(wxT("elements")).Caption(_("Elements")).MaximizeButton(true).CloseButton(false)
      );
  m_mgr.AddPane( wxGetApp().factory()->create_displayctrl(this), 
      wxAuiPaneInfo().Name(wxT("display")).Caption(_("Display")).MaximizeButton(true).
      CenterPane()
      );
  m_mgr.AddPane(tool_bar_file, wxAuiPaneInfo().Name(wxT("tb-file")).Caption(_("File")).
	  ToolbarPane().Top().Row(1).Position(1).LeftDockable(false).RightDockable(false)
	  );
  m_textpreview = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  m_textpreview->SetFont(wxFont(8, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
  m_mgr.AddPane( m_textpreview, wxAuiPaneInfo().Name(wxT("textoutput")).Caption(_("Text output")).
    CloseButton(true).MaximizeButton(true)
    );

  m_defaultperspective = m_mgr.SavePerspective();
  m_mgr.LoadPerspective( Prefs::get().perspective );

  // default transparency hints throw assertions all over the place
  // on linux
#ifndef WIN32
  set_floating_hint( wxAUI_MGR_RECTANGLE_HINT );
#endif
  m_mgr.Update();
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

void MainFrame::OnMenuPreferences( wxCommandEvent& )
{
  wxMessageBox(wxT("Not yet available, edit config file directly:\n") + wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );
}

void MainFrame::OnMenuGameSelection( wxCommandEvent& )
{
  GameSelectionDialog dlg(this, wxID_ANY, _T("Select your game"));
  int ret = dlg.ShowModal();
  if( ret == ID_BTN_CPMA )
    Prefs::get().game = wxT("cpma");
  else if( ret == ID_BTN_Q4MAX )
    Prefs::get().game = wxT("q4max");
  Prefs::get().startup_gameselection = dlg.startup_gameselection();
}

void MainFrame::OnMenuExit( wxCommandEvent& )
{
  Close(true);
}

void MainFrame::OnMenuNew( wxCommandEvent& )
{
  wxGetApp().hudfile()->on_new();
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
    if( wxGetApp().hudfile()->load( dlg.GetPath() ) )
    {
    }
    else
    {
      wxLogError( _("Failed reading Hud from file `%s'"), dlg.GetPath().c_str() );
    }
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

void MainFrame::OnClose( wxCloseEvent& ev )
{

  wxLogDebug(wxT("CLOSE"));
  Prefs::get().perspective = m_mgr.SavePerspective();

  ev.Skip();
}

void MainFrame::OnMenuDefaultPerspective( wxCommandEvent& )
{
  m_mgr.LoadPerspective( m_defaultperspective );
}
