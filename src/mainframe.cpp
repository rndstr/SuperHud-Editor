
#include "mainframe.h"

#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/stdpaths.h>
#include <wx/file.h>

#include "common.h"
#include "factorybase.h"

#include "cpma/elementsctrl.h"
#include "cpma/propertiesnotebook.h"
#include "cpma/displayctrl.h"
#include "cpma/hudfile.h"

//DECLARE_APP(SHEApp);

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_ABOUT, MainFrame::OnMenuAbout)
  EVT_MENU(wxID_EXIT, MainFrame::OnMenuExit)
  EVT_MENU(wxID_SAVE, MainFrame::OnMenuSave)
  EVT_MENU(wxID_SAVEAS, MainFrame::OnMenuSaveAs)
  EVT_MENU(wxID_OPEN, MainFrame::OnMenuOpen)
  EVT_MENU(wxID_NEW, MainFrame::OnMenuNew)
  EVT_MENU(ID_MENU_TOOLS_SWITCHGAME, MainFrame::OnMenuSwitchGame)
  EVT_MENU(ID_MENU_TOOLS_PREFERENCES, MainFrame::OnMenuPreferences)
  EVT_MENU(ID_MENU_VIEW_DEFAULTPERSPECTIVE, MainFrame::OnMenuDefaultPerspective)
  EVT_CLOSE(MainFrame::OnClose)
  EVT_MENU(ID_MENU_VIEW_CONFIGPREVIEW, MainFrame::OnMenuConfigPreview)
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
  file_menu->Append( wxID_OPEN, _("&Open...\tCtrl+O") );
  file_menu->AppendSeparator();
  file_menu->Append( wxID_SAVE, _("&Save\tCtrl+S") );
  file_menu->Append( wxID_SAVEAS, _("Save &As...\tCtrl+Shift+S") );
  file_menu->AppendSeparator();
  file_menu->Append( wxID_EXIT, _("E&xit\tCtrl+Q") );
  menu_bar->Append( file_menu, _("&File") );

  wxMenu *tools_menu = new wxMenu;
  tools_menu->Append( ID_MENU_TOOLS_SWITCHGAME, _("&Switch game") );
  tools_menu->AppendSeparator();
  tools_menu->Append( ID_MENU_TOOLS_PREFERENCES, _("&Preferences\tAlt+P") );
  menu_bar->Append( tools_menu, _("&Tools") );

  wxMenu *elements_menu = new wxMenu;
  menu_bar->Append( elements_menu, _("&Elements") );

  m_view_menu= new wxMenu;
  m_view_menu->Append( ID_MENU_VIEW_DEFAULTPERSPECTIVE, _("&Reset View") );
  m_view_menu->AppendSeparator();
  m_view_menu->AppendCheckItem( ID_MENU_VIEW_CONFIGPREVIEW, _("[FIXME]Config Preview"), _("Display the Config Preview panel") );
  m_view_menu->AppendCheckItem( ID_MENU_VIEW_TOOLBAR_FILE, _("[FIXME]Toolbar File"), _("Display the File toolbar") );
  menu_bar->Append( m_view_menu, _("&View") );

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
  tool_bar_file->AddTool( wxID_NEW, _("New"), wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_TOOLBAR, wxSize(16,16)), _("New") );
  tool_bar_file->AddTool( wxID_OPEN, _("Open..."), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(16,16)), _("Open...") );
  tool_bar_file->AddTool( wxID_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(16,16)), _("Save") );
  tool_bar_file->AddTool( wxID_SAVEAS, _("Save As..."), wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(16,16)), _("Save As...") );
  tool_bar_file->Realize();
 


  // create panes
  m_elementsctrl = wxGetApp().factory()->create_elementsctrl(this);
  m_mgr.AddPane( m_elementsctrl,
      wxAuiPaneInfo().Name(wxT("elements")).Caption(_("Elements")).MaximizeButton(true).CloseButton(false)
      );
  m_configpreview = new wxTextCtrl(this, ID_TEXTCTRL_CONFIGPREVIEW, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  m_configpreview->SetFont(wxFont(8, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
  m_mgr.AddPane( m_configpreview, wxAuiPaneInfo().Name(wxT("configpreview")).Caption(_("Config Preview")).
    CloseButton(true).MaximizeButton(true)
    );
  m_mgr.AddPane( wxGetApp().factory()->create_displayctrl(this), 
      wxAuiPaneInfo().Name(wxT("display")).Caption(_("Display")).MaximizeButton(true).CloseButton(false).
      CenterPane()
      );

  m_propertiesnotebook = wxGetApp().factory()->create_propertiesnotebook(this);
  m_mgr.AddPane( m_propertiesnotebook, 
      wxAuiPaneInfo().Name(wxT("properties")).Caption(_("Properties")).MaximizeButton(true).CloseButton(false).
      Right()
      );

  m_mgr.AddPane(tool_bar_file, wxAuiPaneInfo().Name(wxT("tb-file")).Caption(_("File")).
	  ToolbarPane().Top().Row(1).Position(1).LeftDockable(false).RightDockable(false)
	  );
  

  m_defaultperspective = m_mgr.SavePerspective();
  m_mgr.LoadPerspective( Prefs::get().perspective );
  m_view_menu->Check( ID_MENU_VIEW_CONFIGPREVIEW, m_configpreview->IsShown() );

  if( Prefs::get().game == wxT("q4max") )
    SetIcon( wxArtProvider::GetIcon(ART_Q4MAX, wxART_FRAME_ICON, wxSize(16,16)) );
  else
    SetIcon( wxArtProvider::GetIcon(ART_CPMA, wxART_FRAME_ICON, wxSize(16,16)) );

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

void MainFrame::OnMenuSwitchGame( wxCommandEvent& )
{
  Prefs::get().startup_gameselection = true;
  Prefs::get().save();
  wxMessageBox(_("Please restart the application to select your game"));
}

void MainFrame::OnMenuExit( wxCommandEvent& )
{
  Close(true);
}
#include "pakmanagerbase.h"
void MainFrame::OnMenuAbout( wxCommandEvent& )
{
  wxLogDebug(wxT("about"));
  wxGetApp().pakmanager()->load(0, wxT("test/test.txt"));
  wxGetApp().pakmanager()->load(0, wxT("icons/iconf_red1.tga"));
}

void MainFrame::OnMenuNew( wxCommandEvent& )
{
  wxGetApp().hudfile()->on_new();
  update_title();
  m_elementsctrl->OnSelectionChanged();
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
    wxBeginBusyCursor();
    if( wxGetApp().hudfile()->load( dlg.GetPath() ) )
    {
    }
    else
      wxLogError( _("Failed reading Hud from file `%s'"), dlg.GetPath().c_str() );
    update_title();
    wxEndBusyCursor();
    m_elementsctrl->OnSelectionChanged();
  }
}

void MainFrame::OnMenuSave( wxCommandEvent& )
{
  wxString filename = wxGetApp().hudfile()->filename();
  if( filename.empty() )
  {
    int ret = wxID_OK;
    wxFileDialog dlg(
        this,
        _("Save..."),
        wxT(""),
        wxT(""),
        wxT("Hud Files (*.cfg)|*.cfg|All Files (*.*)|*.*"),
  #if wxCHECK_VERSION(2,7,0)
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
  #else // 2.6
        wxSAVE|wxOVERWRITE_PROMPT
  #endif
        );
    if( wxID_OK == (ret = dlg.ShowModal()) )
      filename = dlg.GetPath();
  }
  if( filename.empty() )
    return; // user clicked cancel

  if( Prefs::get().save_backup && wxFile::Exists(filename)  )
  {
    wxString target = filename + wxT(".bak");
    if( !wxCopyFile( filename, target, true ) )
      wxLogError( _("Failed creating backup file: %s"), target.c_str() );
  }
  wxBeginBusyCursor();
  if( !wxGetApp().hudfile()->save( filename ) )
    wxLogError( _("Failed writing Hud to `%s'"), filename.c_str() );
  update_title();
  wxEndBusyCursor();
}

void MainFrame::OnMenuSaveAs( wxCommandEvent& )
{
  int ret = wxID_OK;
  wxFileDialog dlg(
      this,
      _("Save As..."),
      wxT(""),
      wxT(""),
      wxT("Hud Files (*.cfg)|*.cfg|All Files (*.*)|*.*"),
#if wxCHECK_VERSION(2,7,0)
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT
#else // 2.6
      wxSAVE|wxOVERWRITE_PROMPT
#endif
      );
  if( wxID_OK != (ret = dlg.ShowModal()) )
    return; // user clicked cancel

  wxString filename = dlg.GetPath();
  
  wxBeginBusyCursor();
  if( !wxGetApp().hudfile()->save( filename ) )
    wxLogError( _("Failed writing Hud to `%s'"), filename.c_str() );
  update_title();
  wxEndBusyCursor();
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

void MainFrame::update_title()
{
  wxString fn = wxGetApp().hudfile()->filename();
  wxString mod = (wxGetApp().hudfile()->is_modified() ? wxT("*") : wxEmptyString);

  if( !fn.empty() )
    SetTitle( mod + fn + wxT(" - ") + APP_CAPTION );
  else
    SetTitle( mod + wxT("[no file] - ") + APP_CAPTION );


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

void MainFrame::OnMenuConfigPreview( wxCommandEvent& )
{
  m_configpreview->Show( m_view_menu->IsChecked(ID_MENU_VIEW_CONFIGPREVIEW) );
  DoUpdate();
}

#include <wx/sstream.h>
#include <wx/txtstrm.h>
void MainFrame::OnElementSelectionChanged()
{
  wxASSERT(m_elementsctrl);
  elements_type& els = m_elementsctrl->selected_elements();

  // -- update propertiesctrl title
  wxString caption;
  wxAuiPaneInfo& propsinfo = m_mgr.GetPane(wxT("properties"));
  if( els.size() == 1 )
  {
    caption = _("Properties");
    caption += wxT(": ") + els.front()->name();
  }
  else if( els.size() == 0 )
  {
    caption = _("Properties");
    caption += wxT(": ") + wxString(_("(none)"));
  }
  else
  {
    caption = _("Properties");
    caption += wxT(": ") + wxString(_("(multiple)"));
  }
  propsinfo.Caption( caption );
  DoUpdate();

  // -- update configpreview
  update_configpreview();

  // -- update propertiesctrl
  m_propertiesnotebook->update_from_element(els);
}

void MainFrame::OnPropertiesChanged()
{
  update_configpreview();
}

void MainFrame::update_configpreview()
{
  wxASSERT(m_elementsctrl);
  elements_type& els = m_elementsctrl->selected_elements();
  // -- update config preview
  // TODO put configpreview in own class
  wxString out;
  wxStringOutputStream sos(&out);
  wxTextOutputStream tos(sos);
  // get all selected
  wxListItem info;
  HudFileBase *hf = wxGetApp().hudfile();
  for( cit_elements cit = els.begin(); cit != els.end(); ++cit )
  {
    hf->write_element(tos, *(*cit));
  }  
  m_configpreview->SetValue(out);
}

