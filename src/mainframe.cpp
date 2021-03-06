#include "mainframe.h"

#include "misc.h"


#include "factorybase.h"
#include "prefs.h"
#include "prefsdialog.h"
#include "convertdialog.h"
#include "downloadtext.h"
#include "scrolledmessagedialog.h"


#if ENABLE_CPMA
  #include "cpma_elementsctrl.h"
  #include "cpma_propertiesnotebook.h"
  #include "cpma_displayctrl.h"
  #include "cpma_hudfile.h"
#endif
#if ENABLE_Q4MAX
  #include "q4max_elementsctrl.h"
  #include "q4max_propertiesnotebook.h"
  #include "q4max_displayctrl.h"
  #include "q4max_hudfile.h"
#endif

#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/tipdlg.h>
#include <wx/cmdproc.h>
#include <wx/tokenzr.h>
#include <wx/aboutdlg.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/docview.h>
#include <wx/confbase.h>

#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "xpm/icons/superhudeditor.xpm"
#endif

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_ABOUT, MainFrame::OnMenuAbout)
  EVT_MENU(wxID_EXIT, MainFrame::OnMenuExit)
  EVT_MENU(wxID_SAVE, MainFrame::OnMenuSave)
  EVT_MENU(wxID_SAVEAS, MainFrame::OnMenuSaveAs)
  EVT_MENU(wxID_OPEN, MainFrame::OnMenuOpen)
  EVT_MENU(wxID_NEW, MainFrame::OnMenuNew)
  EVT_MENU(wxID_COPY, MainFrame::OnMenuCopy)
  EVT_MENU(wxID_PASTE, MainFrame::OnMenuPaste)
#if HAS_CMDPROC
  EVT_MENU(wxID_UNDO, MainFrame::OnMenuUndo)
  EVT_MENU(wxID_REDO, MainFrame::OnMenuRedo)
#endif
  EVT_MENU(ID_MENU_TOOLS_SWITCHGAME, MainFrame::OnMenuToolsSwitchGame)
  EVT_MENU(ID_MENU_TOOLS_CONVERT, MainFrame::OnMenuToolsConvert)
  EVT_MENU(ID_MENU_TOOLS_SNAPELEMENTS, MainFrame::OnMenuToolsSnapElements)
  EVT_MENU(ID_MENU_TOOLS_SNAPGRID, MainFrame::OnMenuToolsSnapGrid)
  EVT_MENU(ID_MENU_TOOLS_PREFERENCES, MainFrame::OnMenuToolsPreferences)
  EVT_MENU(ID_MENU_VIEW_DEFAULTPERSPECTIVE, MainFrame::OnMenuViewDefaultPerspective)
  EVT_MENU_RANGE(ID_MENU_VIEW_FOCUSELEMENTS, ID_MENU_VIEW_FOCUSPROPERTIES, MainFrame::OnMenuViewFocus)
  EVT_CLOSE(MainFrame::OnClose)
  EVT_MENU(ID_MENU_VIEW_CONFIGPREVIEW, MainFrame::OnMenuViewConfigPreview)
  EVT_MENU(ID_MENU_VIEW_TOOLBAR_FILE, MainFrame::OnMenuViewToolbarFile)
  EVT_MENU(ID_MENU_VIEW_GRID, MainFrame::OnMenuViewGrid)
  EVT_MENU(ID_MENU_VIEW_HELPER, MainFrame::OnMenuViewHelper)
#if HAS_WEBUPDATER
  EVT_MENU(ID_MENU_HELP_UPDATE, MainFrame::OnMenuHelpUpdate)
#endif
  EVT_MENU(ID_MENU_HELP_TIP, MainFrame::OnMenuHelpTip)
  
  EVT_TOOL(ID_TOOL_VIEW_SUPPRESSHELPERGRID, MainFrame::OnToolViewSuppress)
#ifdef ENABLE_Q4MAX
  EVT_CHOICE(ID_TOOL_VIEW_VISIBLE, MainFrame::OnToolViewVisible)
#endif
  
  EVT_DOWNLOAD(MainFrame::OnDownload)
  EVT_UPDATE_UI_RANGE(ID_MENU_VIEW_CONFIGPREVIEW, ID_MENU_VIEW_TOOLBAR_FILE, MainFrame::OnUpdateViewPanes)
  EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, MainFrame::OnMRUFile)
  EVT_MENU_RANGE(ID_MENU_HELP_WIKI, ID_MENU_HELP_WEBSITE, MainFrame::OnMenuHelpURL)
#ifndef NDEBUG
  EVT_MENU(ID_MENU_DEBUG_LISTREFRESH, MainFrame::OnMenuDebugListRefresh)
  EVT_MENU(ID_MENU_DEBUG_RUELPS, MainFrame::OnMenuDebugRuelps)
#endif
END_EVENT_TABLE()




MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, 
        const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/,
        long style /*= wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER*/) :
  wxFrame(parent, id, title, pos, size, style),
    m_view_menu(0),
    m_edit_menu(0),
    m_statusbar(0),
    m_toolbar_file(0),
    m_toolbar_view(0),
    m_elementsctrl(0),
    m_propertiesnotebook(0),
    m_configpreview(0),
    m_displayctrl(0)
    
{

  SetIcon(wxICON(superhudeditor));
  m_mgr.SetManagedWindow(this);

  // create menu
  wxMenuItem *item;
  wxMenuBar *menu_bar = new wxMenuBar;

  wxMenu *file_menu = new wxMenu;
  wxMenu *recent_submenu = new wxMenu;
  file_menu->Append( wxID_NEW, _("&New\tCtrl+N") );
  file_menu->Append( wxID_OPEN, _("&Open...\tCtrl+O") );
  file_menu->AppendSubMenu( recent_submenu, _("&Recent") );
  file_menu->AppendSeparator();
  file_menu->Append( wxID_SAVE, _("&Save\tCtrl+S") );
  file_menu->Append( wxID_SAVEAS, _("Save &As...\tCtrl+Shift+S") );
  file_menu->AppendSeparator();
  file_menu->Append( wxID_EXIT, _("E&xit\tCtrl+Q") );
  menu_bar->Append( file_menu, _("&File") );
  m_history = new wxFileHistory();
  m_history->UseMenu(recent_submenu);
  if( wxConfigBase::Get() )
    m_history->Load( *wxConfigBase::Get() );

  m_edit_menu = new wxMenu;
#if HAS_CMDPROC
  m_edit_menu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"));
  m_edit_menu->Append(wxID_REDO, _("&Redo\tCtrl+Y"));
  m_edit_menu->AppendSeparator();
#endif
  m_edit_menu->Append( wxID_COPY, _("&Copy\tCtrl+C") );
  m_edit_menu->Append( wxID_PASTE, _("&Paste\tCtrl+V") );
  menu_bar->Append( m_edit_menu, _("&Edit") );

  wxMenu *tools_menu = new wxMenu;
#if HAS_MULTIPLE_GAMES
  tools_menu->Append( ID_MENU_TOOLS_SWITCHGAME, _("&Switch Game") );
  tools_menu->AppendSeparator();
#endif
  tools_menu->Append( ID_MENU_TOOLS_CONVERT, _("Convert &HUD...") );
  tools_menu->AppendSeparator();
  tools_menu->AppendCheckItem( ID_MENU_TOOLS_SNAPELEMENTS, _("&Snap to &Elements\tCtrl+E") );
  tools_menu->AppendCheckItem( ID_MENU_TOOLS_SNAPGRID, _("Snap to &Grid\tCtrl+R") );
  tools_menu->AppendSeparator();
  item = tools_menu->Append( ID_MENU_TOOLS_PREFERENCES, _("&Preferences...\tCtrl+P") );
  item->SetBitmap(wxArtProvider::GetBitmap(wxART_HELP_SETTINGS, wxART_MENU));
  menu_bar->Append( tools_menu, _("&Tools") );
  tools_menu->Check( ID_MENU_TOOLS_SNAPELEMENTS, Prefs::get().var(wxT("view_snapelements")).bval() );
  tools_menu->Check( ID_MENU_TOOLS_SNAPGRID, Prefs::get().var(wxT("view_snapgrid")).bval() );

//  wxMenu *elements_menu = new wxMenu;
//  menu_bar->Append( elements_menu, _("&Elements") );

  m_view_menu = new wxMenu;
  wxMenu *view_layout_submenu = new wxMenu;
  view_layout_submenu->Append( ID_MENU_VIEW_DEFAULTPERSPECTIVE, _("&Default Layout") );
  view_layout_submenu->AppendSeparator();
  view_layout_submenu->AppendCheckItem( ID_MENU_VIEW_CONFIGPREVIEW, _("Config &Preview\tCtrl+T"), _("Display the config preview panel") );
//  view_layout_submenu->AppendCheckItem( ID_MENU_VIEW_TOOLBAR_FILE, _("Toolbar &File"), _("Display the file toolbar") );
  m_view_menu->AppendSubMenu(view_layout_submenu, _("&Layout"), _("Select which panes you would like to be visible or not"));
  m_view_menu->AppendSeparator();
  m_view_menu->AppendCheckItem( ID_MENU_VIEW_GRID, _("Display &Grid\tCtrl+G"), _("Draws a grid over the HUD") );
  m_view_menu->AppendCheckItem( ID_MENU_VIEW_HELPER, _("Display &Helper\tCtrl+H"), _("Draws helper over elements to improve visibility") );
  m_view_menu->AppendSeparator();
  //m_view_menu->Append( ID_MENU_VIEW_FOCUSPROPERTIES, _("Focus Properties\tF6") );
  m_view_menu->Append( ID_MENU_VIEW_FOCUSELEMENTS, _("Focus Elementlist\tF7") );
  m_view_menu->Append( ID_MENU_VIEW_FOCUSHUD, _("Focus HUD View\tF8") );
  menu_bar->Append( m_view_menu, _("&View") );

  wxMenu *help_menu = new wxMenu;
#if HAS_WEBUPDATER
  // so far only win32 :x
  help_menu->Append( ID_MENU_HELP_UPDATE, _("&Web Updater...") );
#endif
  help_menu->Append( ID_MENU_HELP_TIP, _("&Tip of the Day") );
  help_menu->Append( ID_MENU_HELP_WIKI, _("Visit &Mod Wiki"), wxGetApp().factory()->wikiurl() );
  help_menu->Append( ID_MENU_HELP_WEBSITE, _("Visit &Website"), APP_URL );
  help_menu->Append( wxID_ABOUT, _("&About\tCtrl+A") );
  
  menu_bar->Append( help_menu, _("&Help") );

#ifndef NDEBUG
  wxMenu *debug_menu = new wxMenu;
  debug_menu->Append( ID_MENU_DEBUG_LISTREFRESH, wxT("Refresh elementlist") );
  debug_menu->AppendSeparator();
  debug_menu->Append( ID_MENU_DEBUG_RUELPS, wxT("Ruelps") );

  menu_bar->Append( debug_menu, wxT("Debug") );
#endif

  SetMenuBar( menu_bar );

  // statusbar plz
  m_statusbar = CreateStatusBar(3);
  m_statusbar->SetStatusText(_("Ready"), SB_MSG);
  int statusbar_widths[] = { -3, -1, 100 };
  m_statusbar->SetStatusWidths(3, statusbar_widths);
  int statusbar_styles[] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_FLAT };
  m_statusbar->SetStatusStyles( 3, statusbar_styles );
  

  // create toolbars
  m_toolbar_file = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
  m_toolbar_file->SetToolBitmapSize(wxSize(16,16));
  m_toolbar_file->AddTool( wxID_NEW, _("New"), wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(16,16)), _("New HUD") );
  m_toolbar_file->AddTool( wxID_OPEN, _("Open..."), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(16,16)), _("Open HUD...") );
  m_toolbar_file->AddSeparator();
  m_toolbar_file->AddTool( wxID_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(16,16)), _("Save HUD") );
  m_toolbar_file->AddTool( wxID_SAVEAS, _("Save As..."), wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(16,16)), _("Save HUD As...") );
  m_toolbar_file->Realize();

  m_toolbar_view = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORZ_TEXT);
  m_toolbar_view->SetToolBitmapSize(wxSize(16,16));
  m_toolbar_view->AddTool(ID_TOOL_VIEW_SUPPRESSHELPERGRID, _("Preview"),  wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_TOOLBAR, wxSize(16,16)), _("Temporarily disables drawing of Helpers and Grid"), wxITEM_CHECK);
#ifdef ENABLE_Q4MAX
  if( wxGetApp().is_q4max() )
  {
    m_toolbar_view->AddSeparator();
    wxArrayString cs;
    cs.Add(_("All"));
    cs.Add(_("DUEL"));
    cs.Add(_("TDM"));
    cs.Add(_("CTF"));
    wxChoice *c = new wxChoice(m_toolbar_view, ID_TOOL_VIEW_VISIBLE, wxDefaultPosition, wxDefaultSize, cs);
    m_toolbar_view->AddControl(new wxStaticText(m_toolbar_view, wxID_ANY, _("Visible:")));
    m_toolbar_view->AddControl(c);
    c->SetSelection(0);
  }
#endif
  m_toolbar_view->Realize();
 


  // create panes
  m_displayctrl = wxGetApp().factory()->create_displayctrl(this);
  m_mgr.AddPane( m_displayctrl, 
      wxAuiPaneInfo().Name(wxT("display")).Caption(_("Display")).MaximizeButton(true).CloseButton(false).
      CenterPane().MinSize(320,240)
      );
  m_elementsctrl = wxGetApp().factory()->create_elementsctrl(this);
  m_mgr.AddPane( m_elementsctrl,
      wxAuiPaneInfo().Name(wxT("elements")).Caption(_("Elements")).MaximizeButton(true).CloseButton(false).
      MinSize(150,100).BestSize(200,-1)
      );
  m_configpreview = new wxTextCtrl(this, ID_TEXTCTRL_CONFIGPREVIEW, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  wxFont sysfont = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
  // FIXME this doesn't give fixed width font on wxGTK! ffs
  m_configpreview->SetFont( sysfont );
  m_mgr.AddPane( m_configpreview, wxAuiPaneInfo().Name(wxT("configpreview")).Caption(_("Config Preview")).
    CloseButton(true).MaximizeButton(true).MinSize(wxSize(150,100)).BestSize(200,100).Hide()
    );

  m_propertiesnotebook = wxGetApp().factory()->create_propertiesnotebook(this);
  m_mgr.AddPane( m_propertiesnotebook, 
      wxAuiPaneInfo().Name(wxT("properties")).Caption(_("Properties")).MaximizeButton(true).CloseButton(false).
      Right().MinSize(150,100).BestSize(200,-1)
      );

  m_mgr.AddPane(m_toolbar_file, wxAuiPaneInfo().Name(wxT("tb-file")).Caption(_("File")).
	  ToolbarPane().Top().Row(1).Position(1).LeftDockable(false).RightDockable(false).CloseButton(false)
	  );
  m_mgr.AddPane(m_toolbar_view, wxAuiPaneInfo().Name(wxT("tb-view-")+wxGetApp().factory()->dirname_moddata()).Caption(_("View")).
	  ToolbarPane().Top().Row(1).Position(2).LeftDockable(false).RightDockable(false).CloseButton(false)
	  );
  
  m_defaultperspective = m_mgr.SavePerspective();

  GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, wxColour(200, 200, 200));
  GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(200, 200, 200));
  // FIXME not working..
  GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, wxColour(100, 100, 100));
  GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(100, 100, 100));
  

  // reset perspective if config was saved with different app
  if( Prefs::get().oldappversion() == APP_VERSION )
  {
    m_mgr.LoadPerspective( Prefs::get().var(wxT("app_perspective")).sval() );
    m_mgr.GetPane(wxT("tb-view-")+wxGetApp().factory()->dirname_moddata()).Show();
  }
  //m_view_menu->Check( ID_MENU_VIEW_TOOLBAR_FILE, m_toolbar_file->IsShown() ); // done through UpdateUI event
  //m_view_menu->Check( ID_MENU_VIEW_CONFIGPREVIEW, m_configpreview->IsShown() ); // done through UpdateUI event
  m_view_menu->Check( ID_MENU_VIEW_GRID, Prefs::get().var(wxT("view_grid")).bval() );
  m_view_menu->Check( ID_MENU_VIEW_HELPER, Prefs::get().var(wxT("view_helper")).bval() );

  
  
  /*
  // set icon depending on gamename
  if( Prefs::get().var(wxT("game")) == wxT("q4max") )
    SetIcon( wxArtProvider::GetIcon(ART_Q4MAX, wxART_FRAME_ICON, wxSize(16,16)) );
  else
    SetIcon( wxArtProvider::GetIcon(ART_Q4MAX, wxART_FRAME_ICON, wxSize(16,16)) );
    */

  if( wxGetApp().is_firststart() )
    SetSize(wxSize(1000,500));

  //SetMinSize(wxSize(1000,600));
  if( Prefs::get().var(wxT("app_maximized")).bval() )
    Maximize();
  else if( Prefs::get().var(wxT("app_height")).ival() != -1 && Prefs::get().var(wxT("app_width")).ival() != -1 )
    SetSize( Prefs::get().var(wxT("app_width")).ival(), Prefs::get().var(wxT("app_height")).ival() );
  
#ifndef WIN32
  // default transparency hints on linux throw assertions all over the place
  // orly? yarly
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

void MainFrame::check_for_updates()
{
  m_statusbar->PushStatusText(_("Version check..."), SB_MSG);
  wxDownloadText *dl = new wxDownloadText( this, APP_URL_VERSIONCHECK );
}


void MainFrame::OnDownload( wxDownloadEvent& event )
{
  m_statusbar->PopStatusText(SB_MSG);
  bool invalid_response = false;
  int status = event.status();
  if( status == wxDownloadEvent::DOWNLOAD_FAIL )
  {
    wxString msg = _("Version check failed, try again later or\ncheck your proxy settings in Tools->Preferences|Network.");
    wxLogError(msg);
  }
  else if( status == wxDownloadEvent::DOWNLOAD_COMPLETE )
  {
    wxString txt = event.text();
    wxLogDebug(wxT("Received from server:\n") + txt);
    wxStringTokenizer tok(txt, wxT("\n"));
    if( tok.CountTokens() == 0 ) 
    { invalid_response = true; goto mofo; }

    wxString version = tok.GetNextToken();
    wxStringTokenizer vtok(version, wxT(" "));
    if( vtok.CountTokens() != 3 && vtok.CountTokens() != 4 ) 
    { invalid_response = true; goto mofo; }
    long major, minor, release;
    wxString releasetype = wxT("");
    if( !vtok.GetNextToken().ToLong(&major, 10) )
    { invalid_response = true; goto mofo; }
    if( !vtok.GetNextToken().ToLong(&minor, 10) )                                                                                                         
    { invalid_response = true; goto mofo; }
    if( !vtok.GetNextToken().ToLong(&release, 10) )
    { invalid_response = true; goto mofo; }
    if( vtok.HasMoreTokens() )
      releasetype = vtok.GetNextToken();

    int check = she::versioncheck( major, minor, release, releasetype );
    wxLogDebug(wxT("versioncheck returned: %d"), check);
    if( check < 0 ) // new version!
    {
      ScrolledMessageDialog dlg(wxID_CANCEL);

      dlg.add_button(_("&Later"), wxID_CANCEL);
      dlg.add_button(_("&Visit website"), wxID_YES);
#if HAS_WEBUPDATER
      dlg.add_button(_("Launch &Webupdater"), wxID_NO);
#endif
      

      wxString msg = _("\\o/ There is a new version available: ");
      msg += wxString::Format( wxT("%i.%i.%i "), major, minor, release) + (!releasetype.empty() ? releasetype : wxT(""));
      msg += wxString::Format(_("\n(you have %s installed)"), APP_VERSION.c_str());
      msg += wxT("\n\n");
#if HAS_WEBUPDATER
      msg += _("Go to Help->Web Updater... or visit the website: ") + APP_URL;
#else
      msg += _("Visit the website: ") + APP_URL;
#endif

      wxString text;
      //text += _("Do you want to visit ") + APP_URL + wxT(" ?\n");
      while( tok.HasMoreTokens() )
        text +=  tok.GetNextToken() + wxT("\n");

      dlg.Create(0, msg, text, _("Version check"));
      int ret = dlg.ShowModal();
      if( wxID_YES == ret )
        wxLaunchDefaultBrowser( APP_URL );
#if HAS_WEBUPDATER
      else if( wxID_NO == ret )
        launch_webupdater();
#endif
    }
    else if( check == 0 )
    { // is only displayed if manual update check.
      wxMessageDialog dlg(0, _("Your version is up to date."), _("Version check"), wxICON_INFORMATION);
    }
    else // check > 1
    { // is only displayed if manual update check.
      wxMessageDialog dlg(0, _("You have a newer version than the versioncheck reported Oo"), _("Version check"), wxICON_INFORMATION);
    }
  }

mofo:
  if( invalid_response )
  {
    wxString txt = event.text();
    wxLogWarning( _("Version check failed, the server response was invalid,\ntry again later:\n\n") +  txt);
  }
}


void MainFrame::OnMenuToolsPreferences( wxCommandEvent& )
{
  PrefsDialog dlg(this);
  wxString gamedir = wxGetApp().factory()->dir_game();
  if( wxID_OK == dlg.ShowModal() )
  {
    Prefs::get().save();
    m_displayctrl->reset_projection_mode();
    m_displayctrl->load_background();
    update_displayctrl();
    update_elementsctrl();
    update_propertiesctrl();

    // FIXME props_color/props_bgcolor/props_inheritcolor/propsinheritbgcolor all need a restart as well
    if( wxGetApp().factory()->dir_game().Cmp(gamedir) != 0 )
      wxMessageBox( _("Please restart for certain changes to take effect") );
  }
//  wxMessageBox(wxT("Not yet available, edit config file directly:\n") + wxStandardPaths::Get().GetUserDataDir() + wxT("/") + APP_CONFIG );
}

void MainFrame::OnMenuToolsSwitchGame( wxCommandEvent& )
{
  Prefs::get().setb(wxT("startup_gameselection"), true);
  Prefs::get().save();
  restart_app();
}

void MainFrame::OnMenuToolsConvert( wxCommandEvent& )
{
  // default checking
  static bool _s = true, _sp = true, _f = false; // store previous selection...
  ConvertDialog dlg(this);
  dlg.set(_s, _sp, _f);
  if( wxID_OK == dlg.ShowModal() )
  {
    _s = dlg.size(); _sp = dlg.stretchposition(); _f = dlg.fontsize();
    wxGetApp().hudfile()->convert_all( dlg.convert_from(), dlg.convert_to(), _s, _sp, _f );

    Prefs::get().set(wxT("view_aspectratio"), dlg.convert_to_str());
    m_displayctrl->reset_projection_mode();
    update_displayctrl();
  }
}

void MainFrame::OnMenuToolsSnapElements( wxCommandEvent& ev )
{
  Prefs::get().setb(wxT("view_snapelements"), ev.IsChecked());
}

void MainFrame::OnMenuToolsSnapGrid( wxCommandEvent& ev )
{
  Prefs::get().setb(wxT("view_snapgrid"), ev.IsChecked());
}

void MainFrame::OnMenuExit( wxCommandEvent& )
{
  Close(true);
}

#include "varcontainer.h"
void MainFrame::OnMenuAbout( wxCommandEvent& )
{
  wxAboutDialogInfo info;
  info.SetName(APP_NAME);
  info.SetVersion(APP_VERSION);
  wxString desc = wxT("Head-Up Display Editor for\nQuake3 CPMA and Quake4 Q4MAX\nunder GPL License.");
  desc += wxT("\n\n");
  desc += wxVERSION_STRING;
  desc += wxT("\nBuildtime: ");
  desc += APP_BUILDTIME;
  info.SetDescription(desc);
  info.SetCopyright(wxT("(C) 2006-2008 Roland Schilter <rolansch@ethz.ch>"));
  info.SetWebSite(APP_URL, APP_URL);
  wxAboutBox(info);
}

void MainFrame::OnMenuNew( wxCommandEvent& )
{
  if( !confirm_saving() )
    return;
  wxGetApp().hudfile()->OnNew();
  update_title();
  m_elementsctrl->OnSelectionChanged();
}

void MainFrame::OnMenuOpen( wxCommandEvent& )
{
  if( !confirm_saving() )
    return;
  wxGetApp().hudfile()->OnOpen();
  update_title();
  m_elementsctrl->OnSelectionChanged();
  m_history->AddFileToHistory(wxGetApp().hudfile()->filename());
  
}

void MainFrame::OnMenuSave( wxCommandEvent& )
{
  wxGetApp().hudfile()->OnSave();
  update_title();
}

void MainFrame::OnMenuSaveAs( wxCommandEvent& )
{
  wxGetApp().hudfile()->OnSave(true);
  update_title();
}

void MainFrame::OnMenuCopy( wxCommandEvent& ev )
{
  m_elementsctrl->OnCopy(ev);
}

void MainFrame::OnMenuPaste( wxCommandEvent& ev )
{
  m_elementsctrl->OnPaste(ev);
}

#if HAS_CMDPROC
void MainFrame::OnMenuUndo( wxCommandEvent& )
{
  wxGetApp().cmds()->Undo();
}

void MainFrame::OnMenuRedo( wxCommandEvent& )
{
  wxGetApp().cmds()->Redo();
}
#endif


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

int MainFrame::confirm_savechanges_dlg()
{
  wxMessageDialog dlg(
      this,
      _("Save changes to the current HUD?"),
      _("Confirm"),
      wxYES_NO|wxCANCEL|wxYES_DEFAULT|wxICON_QUESTION );
  return dlg.ShowModal();
}

bool MainFrame::confirm_saving()
{
  HudFileBase *hf = wxGetApp().hudfile();

  int save = (hf->is_modified()? wxID_YES : wxID_NO);
  
  // ask if it's modified
  if( wxGetApp().hudfile()->is_modified() )
    save = confirm_savechanges_dlg();

  if( wxID_CANCEL == save )
    return false; // user aborted question
  if( wxID_YES == save )
    if( wxID_CANCEL == wxGetApp().hudfile()->OnSave() )
      return false; // user aborted save dialog

  return true; // user went through... 
}

void MainFrame::OnClose( wxCloseEvent& ev )
{
  wxString exec = wxGetApp().exec();
  wxGetApp().set_exec(wxEmptyString); // we will set it again if this function does not abort

  wxLogDebug(wxT("MainFrame::OnClose"));

  if( !confirm_saving() )
    return;

  
  // save view
  Prefs::get().set(wxT("app_perspective"), m_mgr.SavePerspective());
  Prefs::get().seti(wxT("app_width"), GetSize().GetWidth());
  Prefs::get().seti(wxT("app_height"), GetSize().GetHeight());
  Prefs::get().setb(wxT("app_maximized"), IsMaximized());
  wxGetApp().factory()->set_startup_loadfile( wxGetApp().hudfile()->filename() );

  if( wxConfigBase::Get() )
    m_history->Save(*wxConfigBase::Get());

  if( !exec.empty() )
    wxLogDebug(wxT("MainFrame::OnClose - will exec: ") + exec);
  wxGetApp().set_exec(exec);

  ev.Skip();// this->Destroy()?!
}

void MainFrame::OnMenuViewDefaultPerspective( wxCommandEvent& )
{
  m_mgr.LoadPerspective( m_defaultperspective );
}

void MainFrame::OnMenuViewConfigPreview( wxCommandEvent& )
{
  wxAuiPaneInfo& info = m_mgr.GetPane(wxT("configpreview"));
  if( m_view_menu->IsChecked(ID_MENU_VIEW_CONFIGPREVIEW) )
  {
    info.Show(true);
    update_configpreview();
  }
  else
    info.Show(false);
  DoUpdate();
}

void MainFrame::OnMenuViewToolbarFile( wxCommandEvent& )
{
  wxAuiPaneInfo& info = m_mgr.GetPane(wxT("tb-file"));
  info.Show( m_view_menu->IsChecked(ID_MENU_VIEW_TOOLBAR_FILE) );
  DoUpdate();
}

void MainFrame::OnMenuViewGrid( wxCommandEvent& ev )
{
  Prefs::get().setb(wxT("view_grid"), ev.IsChecked());
  update_displayctrl();
}

void MainFrame::OnMenuViewHelper( wxCommandEvent& ev )
{
  Prefs::get().setb(wxT("view_helper"), ev.IsChecked());
  update_displayctrl();
}

void MainFrame::OnMenuViewFocus( wxCommandEvent& ev )
{
  wxLogDebug(wxT("focus"));
  switch( ev.GetId() )
  {
    case ID_MENU_VIEW_FOCUSELEMENTS:
      m_elementsctrl->SetFocus();
      break;
    case ID_MENU_VIEW_FOCUSHUD:
      m_displayctrl->SetFocus();
      break;
    case ID_MENU_VIEW_FOCUSPROPERTIES:
      m_propertiesnotebook->SetFocus();
      break;
  }
}


void MainFrame::OnMenuHelpTip( wxCommandEvent& )
{
  wxTipProvider *provider = wxCreateFileTipProvider(
      wxStandardPaths::Get().GetDataDir() + PATH_SEP + wxT("data") + PATH_SEP + wxT("tips.txt"), 
      Prefs::get().var(wxT("startup_tipidx")).ival()
      );
  Prefs::get().setb(wxT("startup_tips"), wxShowTip(this, provider));
  Prefs::get().seti(wxT("startup_tipidx"), provider->GetCurrentTip());
  delete provider;
}

void MainFrame::OnUpdateViewPanes( wxUpdateUIEvent& ev )
{
  if( ev.GetId() == ID_MENU_VIEW_CONFIGPREVIEW )
    ev.Check( m_configpreview->IsShown() );
  else if( ev.GetId() == ID_MENU_VIEW_TOOLBAR_FILE )
    ev.Check( m_toolbar_file->IsShown() );
}

void MainFrame::OnToolViewSuppress( wxCommandEvent& ev )
{
  Prefs::get().setb(wxT("view_suppresshelpergrid"), ev.IsChecked());
  update_displayctrl();
}

#ifdef ENABLE_Q4MAX
void MainFrame::OnToolViewVisible( wxCommandEvent& ev )
{
  int vis;
  switch( ev.GetInt() )
  {
  case 1:
    vis = MAX_E_VIS_DUEL;
    break;
  case 2:
    vis = MAX_E_VIS_TDM;
    break;
  case 3:
    vis = MAX_E_VIS_CTF;
    break;
  case 0:
  default:
    vis = MAX_E_VIS_ALL;
    break;
  }
  Prefs::get().seti(wxT("view_visible"), vis);
  update_displayctrl();
  // do not refresh but just update their attributes
  m_elementsctrl->update_items();
}
#endif

void MainFrame::OnMRUFile( wxCommandEvent& ev )
{
  wxString f(m_history->GetHistoryFile(ev.GetId() - wxID_FILE1));
  if( !f.empty() )
  {
    wxGetApp().hudfile()->OnOpen(f);
    update_title();
    m_elementsctrl->OnSelectionChanged();
  }
}

void MainFrame::OnMenuDebugListRefresh( wxCommandEvent& )
{
  update_elementsctrl();
}

void MainFrame::OnMenuDebugRuelps( wxCommandEvent& )
{
  m_elementsctrl->update_items();
}

// call this in the event handler used to show the wxWebUpdateDlg
void MainFrame::update_and_exit(bool savelog /*= FALSE*/,
     				bool restart /*= TRUE*/,
     				const wxString &xrc /*= wxEmptyString*/, 	// --xrc option won't be given using wxEmptyString
         			const wxString &res /*= wxEmptyString*/,	// --res option won't be given using wxEmptyString
            		const wxString &xml /*= wxEmptyString*/,	// --xml option won't be given using wxEmptyString
         			const wxString &uri /*= wxEmptyString*/)	// --uri option won't be given using wxEmptyString
{
	wxString opts;
 
 	if (savelog)
  		opts += wxT(" --savelog");
    if (restart)
    	opts += wxT(" --restart");
    if (!xrc.IsEmpty())
     	opts += wxT(" --xrc=\"") + xrc + wxT("\"");
    if (!res.IsEmpty())
    	opts += wxT(" --res=\"") + res + wxT("\"");
 	if (!xml.IsEmpty())
  		opts += wxT(" --xml=\"") + xml + wxT("\"");
 	if (!uri.IsEmpty())
  		opts += wxT(" --uri=\"") + uri + wxT("\"");

  wxLogDebug(wxT("Invoking webupdater: ") + opts);
  wxString cmd;
#ifdef __WXMSW__
	cmd = wxT("webupdater.exe") + opts;
#else	
	cmd = wxT("./webupdater") + opts;
#endif
  wxGetApp().set_exec(cmd);
  Close(true);
}

void MainFrame::restart_app()
{
  wxGetApp().set_exec(wxStandardPaths::Get().GetExecutablePath());
	Close(true);
}

#if HAS_WEBUPDATER
void MainFrame::launch_webupdater()
{
  // we only allow the updater to be called in release version.. otherwise there will be wrong versions executed etcetc.
  // saves some confusion.
#ifdef NDEBUG
  wxString dd = wxStandardPaths::Get().GetDataDir() + PATH_SEP;
  update_and_exit(true, true, 
    dd + wxT("data/webupdater/webupdatedlg.xrc"), 
    wxT("wxWebUpdateLogDlg"),
    dd + wxT("data/webupdater/local.xml")
    );
#else
  wxLogMessage(wxT("Updater is not available in debug version"));
#endif
}


void MainFrame::OnMenuHelpUpdate( wxCommandEvent& )
{
  launch_webupdater();
}
#endif

void MainFrame::OnMenuHelpURL( wxCommandEvent& ev )
{
  if( ev.GetId() == ID_MENU_HELP_WIKI )
    wxLaunchDefaultBrowser( wxGetApp().factory()->wikiurl() );
  else if( ev.GetId() == ID_MENU_HELP_WEBSITE )
    wxLaunchDefaultBrowser( APP_URL );
}

void MainFrame::OnElementSelectionChanged()
{
  wxASSERT(m_elementsctrl);
  wxASSERT(m_statusbar);
  elements_type& els = m_elementsctrl->selected_elements();

  // -- update propertiesctrl title
  wxString caption;
  wxAuiPaneInfo& propsinfo = m_mgr.GetPane(wxT("properties"));
  if( els.size() == 1 )
  {
    caption = _("Properties");
    caption += wxT(": ") + els.front()->name();
    wxRect r = els.front()->iget_rect();
    m_statusbar->SetStatusText( els.front()->name() + wxString::Format(wxT(" @ (%i,%i)-(%i,%i)"), r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom()), SB_ELEMENT );
  }
  else if( els.size() == 0 )
  {
    caption = _("Properties");
    caption += wxT(": ") + wxString(_("(none)"));
    m_statusbar->SetStatusText( wxT(""), SB_ELEMENT );
  }
  else
  {
    caption = _("Properties");
    caption += wxT(": ");
    caption += _("(multiple)");
    m_statusbar->SetStatusText( _("(multiple)"), SB_ELEMENT );
  }
  propsinfo.Caption( caption );
  m_propertiesnotebook->SetLabel(caption);
  DoUpdate();

  // -- render view
  m_displayctrl->Refresh();

  // -- update configpreview
  update_configpreview();

  // -- update propertiesctrl
  m_propertiesnotebook->update_from_element(els);
}

void MainFrame::OnPropertiesChanged()
{
  update_displayctrl();
  update_configpreview();
}

void MainFrame::update_configpreview()
{
  if( !m_configpreview->IsShown() )
    return;
  wxASSERT(m_elementsctrl);
  elements_type& els = m_elementsctrl->selected_elements();
  // -- update config preview
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


void MainFrame::update_displayctrl()
{
  m_displayctrl->Refresh();
}

void MainFrame::update_elementsctrl()
{
  const elements_type& els = wxGetApp().hudfile()->elements();
  m_elementsctrl->list_refresh(els);
}

void MainFrame::update_propertiesctrl()
{
  elements_type& els = wxGetApp().elementsctrl()->selected_elements();
  m_propertiesnotebook->update_from_element(els);
}

