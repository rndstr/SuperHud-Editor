#include "elementsctrlbase.h"
#include "propertiesnotebookbase.h"
#include "elementbase.h"
#include "misc.h"
#include "mainframe.h"
#include "prefs.h"
#include "optionalmessagedialog.h"
#include "command.h"
#include "artprovider.h"

#include <wx/dnd.h>

#include <algorithm>
#include <functional>

//#define COLLECTION_INCLUDES_PARENT 1

BEGIN_EVENT_TABLE(ElementsCtrlBase, wxPanel)
  EVT_BUTTON(wxID_COPY, ElementsCtrlBase::OnCopy)
  EVT_BUTTON(wxID_PASTE, ElementsCtrlBase::OnPaste)
  EVT_BUTTON(wxID_CLEAR, ElementsCtrlBase::OnReset)
  EVT_BUTTON(wxID_DELETE, ElementsCtrlBase::OnDelete)
  EVT_BUTTON(ID_BTN_INSERT, ElementsCtrlBase::OnBtnInsert)

  EVT_MENU(wxID_COPY, ElementsCtrlBase::OnCopy)
  EVT_MENU(wxID_PASTE, ElementsCtrlBase::OnPaste)
  EVT_MENU(wxID_DELETE, ElementsCtrlBase::OnDelete)
  EVT_MENU(wxID_CLEAR, ElementsCtrlBase::OnReset)

  EVT_LIST_KEY_DOWN(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnKeyDown)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnItemSelected)
  EVT_LIST_ITEM_DESELECTED(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnItemDeselected)
  EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnItemActivated)
  EVT_LIST_BEGIN_DRAG(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnBeginDrag)
  EVT_LIST_ITEM_RIGHT_CLICK(ID_LISTCTRL_ELEMENTS, ElementsCtrlBase::OnItemRightClick)
  EVT_MENU_RANGE( ID_INSERT_NOTUNIQ, ID_INSERT_NOTUNIQ_END, ElementsCtrlBase::OnInsertNotuniq)
END_EVENT_TABLE()

// begin wxGlade: ::extracode
// end wxGlade

// Note that this probably isn't the way it's supposed to be
// we accept text although we are actually only accepting Elements.
// but hey, ... we only need to know if there was a drop and then
// move all selected elements to there. so if a user drops text here
// from somewhere else it should be ok.
class ListDrop : public wxTextDropTarget
{
  public:
    ListDrop( ElementsListCtrl *list) : m_list(list) {}
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
    {
      // copy selection as it changes after the list_refresh
      const elements_type els = static_cast<ElementsCtrlBase*>(m_list->GetParent())->selected_elements();
      if( 0 == els.size() )
        return false; // no elements selected
      int flags = wxLIST_HITTEST_BELOW;
      long hit = hititem(x, y);
      if( hit == wxNOT_FOUND )
        return false; // the drop wasn't over an existing item

      wxASSERT( m_list->GetItemData(hit) );
#if HAS_CMDPROC
      ReorderCommand *s = new ReorderCommand(_("Reorder elements"));
#endif
      ElementBase *after = reinterpret_cast<ElementBase*>(m_list->GetItemData(hit));
      // move all selected items after the one the cursor is over
      for( size_t rev = els.size(); rev != 0; --rev )
        wxGetApp().hudfile()->move_element_after( els[rev-1], after );
#if HAS_CMDPROC
      s->takepost();
      wxGetApp().cmds()->Submit(s);
#endif

      // TODO simple but expensive ;) hooray (doesn't flicker on MSW, checkcheck for GTK)
      // Note this is already done in ReorderCommand::Do
      //static_cast<ElementsCtrlBase*>(m_list->GetParent())->list_refresh(wxGetApp().hudfile()->elements());
      // TODO ignore EVT_LIST_ITEM_SELECTED during this loop

      wxGetApp().elementsctrl()->list_refresh(wxGetApp().hudfile()->elements());
      for( cit_elements cit = els.begin(); cit != els.end(); ++cit )
        static_cast<ElementsCtrlBase*>(m_list->GetParent())->select_item(*cit);

    //  wxGetApp().mainframe()->elementsctrl()->OnSelectionChanged();

      return true;
    }

    long hititem( wxCoord x, wxCoord y )
    {
      int flags = wxLIST_HITTEST_BELOW;
      long hit = m_list->HitTest(wxPoint(x, y), flags);
      if( hit == wxNOT_FOUND )
        return wxNOT_FOUND;
#if 1 // does not allow to drop on collection
      if( !m_list->GetItemData(hit) )
        return wxNOT_FOUND; // collection

#else // we can drop onto collections (although it just picks the item before, maybe it just be the one after though anyway)
      // skip collectiontitles backwards
      while( hit >= 0 && !m_list->GetItemData(hit) )
        --hit;
      wxASSERT( m_list->GetItemData(hit) || hit < 0 );
      if( hit < 0 )
        return wxNOT_FOUND; // there are no more items
#endif
      return hit;
    }
    virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
    {
      // if cursor is at top or bottom position while dragging we scroll
      
      wxRect rh;
      m_list->GetItemRect(0, rh, wxLIST_RECT_BOUNDS); // just figure out what the boundaries are to snap/scroll
      wxSize s = m_list->GetSize();
      int scroll = rh.GetHeight();
      int snap = rh.GetHeight()*2;
      // [wxGTK] ScrollList not inplemented?
      if( y < snap )
        m_list->ScrollList(0, -scroll);
      else if( y > s.y - snap )
        m_list->ScrollList(0, scroll);

      // write in statusbar as long as we don't have a visual indication where it's inserted :x
      long hit = hititem(x, y);
      
      if( hit == wxNOT_FOUND )
      {
        wxGetApp().mainframe()->statusbar()->SetStatusText(_("Can't drop on collection"), (SB_MSG));
        return wxDragNone;
      }
      else
      {
        wxListItem info;
        info.m_mask = wxLIST_MASK_TEXT;
        info.m_col = 1;
        info.m_itemId = hit;
        if( m_list->GetItem(info) )
          wxGetApp().mainframe()->statusbar()->SetStatusText(_("Insert after: ") + info.m_text, (SB_MSG));
      }

      // draw a line where we are to insert the element(s)
      // FIXME this does leave some lines drawn :/
      /*
      static wxRect bound;
      static bool drawn = false;

      wxClientDC dc(m_list);
      dc.SetPen(*wxBLACK_PEN);
      if( drawn )
      { // only reset if it was drawn
        dc.SetLogicalFunction( wxINVERT );
        dc.DrawLine( bound.GetLeft(), bound.GetBottom(), bound.GetRight(), bound.GetBottom());
        drawn = false;
      }
      if( hit != wxNOT_FOUND )
      {
        m_list->GetItemRect(hit, bound, wxLIST_RECT_BOUNDS);
        dc.SetLogicalFunction( wxCLEAR);
        dc.DrawLine( bound.GetLeft(), bound.GetBottom(), bound.GetRight(), bound.GetBottom());
        drawn = true;
      }
      */
      return wxTextDropTarget::OnDragOver(x, y, def);
    }
  private:
    ElementsListCtrl *m_list;
};





ElementsCtrlBase::ElementsCtrlBase(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL),
    m_copyfrom(0),
    m_elpopup(0)
{
  /* FIXWXGLADE
    m_btn_insert = new wxBitmapButton(this, ID_BTN_INSERT, wxArtProvider::GetBitmap(wxART_NEW, wxART_BUTTON, wxSize(16,16)));
    static_line_2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    m_btn_delete = new wxBitmapButton(this, wxID_DELETE, wxArtProvider::GetBitmap(wxART_DELETE, wxART_BUTTON, wxSize(16,16)));
    m_list = new ElementsListCtrl(this);
    m_btn_copy = new wxBitmapButton( this, wxID_COPY, wxArtProvider::GetBitmap(wxART_COPY, wxART_BUTTON, wxSize(16,16)) );
    m_btn_paste = new wxBitmapButton( this, wxID_PASTE, wxArtProvider::GetBitmap(wxART_PASTE, wxART_BUTTON, wxSize(16,16)) );
    static_line_1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    m_btn_reset = new wxBitmapButton(this, wxID_CLEAR, wxArtProvider::GetBitmap(ART_CLEAR, wxART_BUTTON, wxSize(16,16)));
    
  */
    // begin wxGlade: ElementsCtrlBase::ElementsCtrlBase
    m_btn_insert = new wxBitmapButton(this, ID_BTN_INSERT, wxArtProvider::GetBitmap(wxART_NEW, wxART_BUTTON, wxSize(16,16)));
    static_line_2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    m_btn_delete = new wxBitmapButton(this, wxID_DELETE, wxArtProvider::GetBitmap(wxART_DELETE, wxART_BUTTON, wxSize(16,16)));
    m_list = new ElementsListCtrl(this);
    m_btn_copy = new wxBitmapButton( this, wxID_COPY, wxArtProvider::GetBitmap(wxART_COPY, wxART_BUTTON, wxSize(16,16)) );
    m_btn_paste = new wxBitmapButton( this, wxID_PASTE, wxArtProvider::GetBitmap(wxART_PASTE, wxART_BUTTON, wxSize(16,16)) );
    static_line_1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    m_btn_reset = new wxBitmapButton(this, wxID_CLEAR, wxArtProvider::GetBitmap(ART_CLEAR, wxART_BUTTON, wxSize(16,16)));

    set_properties();
    do_layout();
    // end wxGlade
    ListDrop *ld = new ListDrop(m_list);
    m_list->SetDropTarget(ld);
}

ElementsCtrlBase::~ElementsCtrlBase()
{
  wxDELETE(m_elpopup);
}

void ElementsCtrlBase::set_properties()
{
  // FIXWXGLADE
    // begin wxGlade: ElementsCtrlBase::set_properties
    m_btn_insert->SetSize(m_btn_insert->GetBestSize());
    m_btn_delete->SetToolTip(wxT("Remove selected element(s) from hud"));
    m_btn_delete->SetSize(m_btn_delete->GetBestSize());
    m_btn_copy->SetToolTip(wxT("Copy element properties"));
    m_btn_copy->SetSize(m_btn_copy->GetBestSize());
    m_btn_paste->SetToolTip(wxT("Paste properties to selected elements (except rectangle)"));
    m_btn_paste->SetSize(m_btn_paste->GetBestSize());
    m_btn_reset->SetToolTip(wxT("Reset element properties to defaults (except rectangle)"));
    m_btn_reset->SetSize(m_btn_reset->GetBestSize());
    // end wxGlade
    m_btn_paste->Disable();
}


void ElementsCtrlBase::do_layout()
{
    // begin wxGlade: ElementsCtrlBase::do_layout
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(m_btn_insert, 0, wxALL, 2);
    sizer_1->Add(static_line_2, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    sizer_1->Add(m_btn_delete, 0, wxALL, 2);
    sizer_3->Add(sizer_1, 0, wxEXPAND, 0);
    sizer_3->Add(m_list, 1, wxEXPAND, 0);
    sizer_4->Add(m_btn_copy, 0, wxALL, 2);
    sizer_4->Add(m_btn_paste, 0, wxALL, 2);
    sizer_4->Add(static_line_1, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    sizer_4->Add(m_btn_reset, 0, wxALL, 2);
    sizer_3->Add(sizer_4, 0, wxEXPAND, 0);
    SetSizer(sizer_3);
    sizer_3->Fit(this);
    // end wxGlade
}



void ElementsCtrlBase::append( ElementBase *el )
{
  long idx = m_list->InsertItem(m_list->GetItemCount(), wxEmptyString, -1);
  m_list->SetItem( idx, 1, el->name(), (el->is_enabled() ? E_LIST_IMG_ENABLED : E_LIST_IMG_DISABLED));
  m_list->SetItemData( idx, (long)(el) );
  if( el->flags() & E_NOTUNIQ )
    m_list->SetItemFont(idx, *wxITALIC_FONT);
  else if( !el->is_rendered() )
    m_list->SetItemTextColour( idx, ELEMENTS_HIDDEN_COLOR );
}

void ElementsCtrlBase::clear()
{
  m_list->DeleteAllItems();
}

void ElementsCtrlBase::list_refresh( const elements_type& elements )
{
  clear();
  for( cit_elements cit = elements.begin(); cit != elements.end(); ++cit )
    append(*cit);

  int collnamecount = Prefs::get().var(wxT("elements_collnamecount")).ival();
  if( Prefs::get().var(wxT("elements_collections")).bval() )
  {
    // now insert collection items
    wxString collname;
    int collcount = 0; // how many collection titles we already inserted
    for( size_t i=1; i < elements.size(); ++i )
    {
      if( elements[i]->name().Left(collnamecount) == elements[i-1]->name().Left(collnamecount) && elements[i]->name().Left(collnamecount) != collname)
      { // we found at least two items, that's enough
        collname = elements[i]->name().Left(collnamecount);

        // how many items belong to this collection?
        size_t g;
        for( g=i-1; g < elements.size(); ++g )
        {
          if( elements[g]->name().Left(collnamecount) != collname )
            break;
        }
        // items [i-1,g-1] have same 3 starting characters
        // maybe they share even more? figeur out
        int minshare = 666;
        m_list->SetItem(i-1+collcount, 0, wxEmptyString, E_LIST_IMG_COLLITEM);
        for( size_t h=i; h <= g-1; ++h )
        {
          m_list->SetItem(h+collcount, 0, wxEmptyString, E_LIST_IMG_COLLITEM);
          minshare = wxMin(she::common_start(elements[h]->name(), elements[h-1]->name()), minshare);
        }
        
        size_t realinsert = i;
        /*
        // also include the previous E_PARENT items.
#if COLLECTION_INCLUDES_PARENT
        for( size_t k=i-2; k>0; --k )
        {
          if( elements[k]->flags() & E_PARENT )
            --realinsert;
          else
            break;
        }
#endif
        */

        // insert collection title
        wxListItem li;
        li.SetMask(wxLIST_MASK_TEXT);
        li.SetId(realinsert-1+collcount);
        li.SetFont(*wxITALIC_FONT);
        li.SetTextColour(ELEMENTS_COLL_COLOR);
        li.SetBackgroundColour(ELEMENTS_COLL_BGCOLOR);
        
        long idx = m_list->InsertItem(li);
        m_list->SetItem(idx, 0, wxEmptyString, E_LIST_IMG_COLLTITLE);
        collname = elements[i]->name().Left(minshare);
        she::wxTrim(collname, wxT("_"));
        m_list->SetItem(idx, 1, collname, -1);
        m_list->SetItemData(idx, 0);
        
        ++collcount;
        i = g-1; // skip over values we just put in a collection
      }
    }
  }
}


void ElementsCtrlBase::OnCopy( wxCommandEvent& )
{
  if( m_selels.size() != 1 )
    return;

  m_copyfrom = m_selels.front();
  m_btn_paste->Enable(true);
}

void ElementsCtrlBase::OnPaste( wxCommandEvent& )
{
  if( m_selels.size() == 0 || m_copyfrom == 0 )
    return;

  std::for_each(m_selels.begin(), m_selels.end(), std::bind2nd(std::mem_fun(&ElementBase::copy_from), m_copyfrom));

  wxGetApp().mainframe()->update_displayctrl();
  wxGetApp().mainframe()->update_propertiesctrl();
  wxGetApp().mainframe()->update_configpreview();
  // update item
  cit_indecies idit = m_selidx.begin();
  wxASSERT( m_selidx.size() == m_selels.size() );
  for( cit_elements elit = m_selels.begin(); elit != m_selels.end(); ++elit, ++idit )
    update_item(*idit, *elit);
  
  wxGetApp().hudfile()->set_modified();
}

void ElementsCtrlBase::OnReset( wxCommandEvent& )
{
  if( m_selels.size() == 0 )
    return;

  OptionalMessageDialog dlg(wxT("dlg_reset"), wxID_YES, wxID_NO); 
  dlg.add_button_yesno();
  wxString msg =  _("This fills default values on all properties (except rectangle) of selected elements:\n\n");
  for( cit_elements cit = m_selels.begin(); cit != m_selels.end(); ++cit )
    msg += (*cit)->name() + wxT("\n");

  msg += wxT("\n");
  msg += _("Proceed?");
  dlg.Create(0, msg, _("Reset properties?"));

  if( dlg.ShowModal() != wxID_YES )
    return;

  wxGetApp().hudfile()->set_modified();

  std::for_each(m_selels.begin(), m_selels.end(), std::mem_fun(&ElementBase::reset));

  wxGetApp().mainframe()->update_displayctrl();
  wxGetApp().mainframe()->update_propertiesctrl();
  wxGetApp().mainframe()->update_configpreview();
  // update item
  cit_indecies idit = m_selidx.begin();
  wxASSERT( m_selidx.size() == m_selels.size() );
  for( cit_elements elit = m_selels.begin(); elit != m_selels.end(); ++elit, ++idit )
    update_item(*idit, *elit);
}

void ElementsCtrlBase::OnDelete( wxCommandEvent& )
{
  if( m_selels.size() == 0 )
    return;

  OptionalMessageDialog dlg(wxT("dlg_reset"), wxID_YES, wxID_NO); 
  dlg.add_button_yesno();
  wxString msg =  _("This will delete all selected elements:\n\n");
  for( cit_elements cit = m_selels.begin(); cit != m_selels.end(); ++cit )
    msg += (*cit)->name() + wxT("\n");

  msg += wxT("\n");
  msg += _("Proceed?");
  dlg.Create(0, msg, _("Remove elements?"));

  if( dlg.ShowModal() != wxID_YES )
    return;

  wxGetApp().hudfile()->set_modified();

  // delete all notuniq elements
  for( it_elements it = m_selels.begin(); it != m_selels.end(); ++it )
  {
    if( (*it)->is_removable() )
    {
      if( m_copyfrom == (*it) )
        m_copyfrom = 0;
      wxGetApp().hudfile()->remove( *it );
    }
  }
  wxGetApp().mainframe()->update_elementsctrl();
  OnSelectionChanged();
}

void ElementsCtrlBase::OnItemDeselected( wxListEvent& ev )
{
//  wxLogDebug(wxT("ElementsCtrlBase::OnItemDeselected"));
  OnSelectionChanged();
}

void ElementsCtrlBase::OnItemSelected( wxListEvent& ev )
{
//  wxLogDebug(wxT("ElementsCtrlBase::OnItemSelected"));
  OnSelectionChanged();
  m_list->EnsureVisible( ev.GetIndex() );
}

void ElementsCtrlBase::OnItemActivated( wxListEvent& ev )
{
  elements_type& els = selected_elements();
  indecies_type& idx = selected_indecies();
  ElementBase *pel = 0;
  cit_indecies idxit = idx.begin();
  for( it_elements it = els.begin(); it != els.end(); ++it, ++idxit )
  {
    (*it)->set_enabled( !(*it)->is_enabled() ); // toggle
    update_item(*idxit, *it);
  }
  wxGetApp().hudfile()->set_modified();
  // trigger update of other views relying on properties

  // propagate
  wxGetApp().mainframe()->OnPropertiesChanged();
}

void ElementsCtrlBase::OnKeyDown( wxListEvent& ev )
{
  int kc = tolower(ev.GetKeyCode());
  wxLogDebug(wxT("ElementsCtrlBase::OnKeyDown - %d"), kc);
  if( kc >= 'a' && kc <= 'z' )
  {
    const long startidx = (m_selidx.size() == 0 ? 0 : m_selidx.front());
    long idx = startidx;
    wxListItem info;
    for( ;; )
    {
      idx = m_list->GetNextItem(idx, wxLIST_NEXT_BELOW, wxLIST_STATE_DONTCARE);
      if( idx == -1 ) idx = 0;
      if( idx == startidx )
        break;
      info.m_mask = wxLIST_MASK_TEXT;
      info.m_col = 1;
      info.m_itemId = idx;
      // skip over collection titles and those that don't start with the pressed key
      if( m_list->GetItem(info) && m_list->GetItemData(idx) != 0 && info.m_text.Lower().StartsWith(wxString::Format(wxT("%c"), kc)) )
      {
        deselect_all();
        select_item(idx);
        break;
      }
    }
  }
}

void ElementsCtrlBase::select_next()
{
  const long startidx = (m_selidx.size() == 0 ? 0 : m_selidx.front());
  long idx = startidx;
  wxListItem info;
  for( ;; )
  {
    idx = m_list->GetNextItem(idx, wxLIST_NEXT_BELOW, wxLIST_STATE_DONTCARE);
    if( idx == -1 ) idx = 0;
    if( idx == startidx )
      break; // only one element to choose from? shouldn't happen normally
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = idx;
    if( m_list->GetItem(info) )
    {
      // skip over collection titles
      if( m_list->GetItemData(idx) != 0 )
      {
        deselect_all();
        select_item(idx);
        break;
      }
    }
  }
}

void ElementsCtrlBase::OnBtnInsert( wxCommandEvent& )
{
  show_element_popup(true);
}

void ElementsCtrlBase::OnItemRightClick( wxListEvent& ev )
{
  if( ev.GetIndex() == wxNOT_FOUND )
    return;
  show_element_popup();
}

void ElementsCtrlBase::show_element_popup( bool only_insert /*=false*/, const wxPoint& p /*=wxDefaultPosition*/ )
{
  // we only display if there is exactly one element selected
  if( m_selels.size() == 0 )
    return; 

  wxDELETE(m_elpopup);
  m_elpopup = new wxMenu();

  const notuniqs_type& notuniqs = wxGetApp().hudfile()->notuniq_elements();
  int i=0;
  wxMenuItem *item;
  ElementBase *el;
  for( cit_notuniqs cit = notuniqs.begin(); cit != notuniqs.end() && i <= (ID_INSERT_NOTUNIQ_END - ID_INSERT_NOTUNIQ); ++cit, ++i )
  {
    el = wxGetApp().hudfile()->find_element(*cit);
    item = m_elpopup->Append(ID_INSERT_NOTUNIQ+i, wxString::Format(_("Insert %s"), cit->c_str()), el ? el->desc() : wxT("HELP"));
    item->Enable( m_selels.size() == 1);
  }

  if( !only_insert )
  {
    m_elpopup->AppendSeparator();

    item = m_elpopup->Append( wxID_COPY, _("Copy") ); // only for 1 element selected
    item->Enable( m_selels.size() == 1 );
    item = m_elpopup->Append( wxID_PASTE, _("Paste") ); // only if there is something to copy from
    item->Enable( m_copyfrom != 0 );

    m_elpopup->AppendSeparator();

    // only enable delete if we actually can delete the element (i.e. it's a notuniq one)
    item = m_elpopup->Append( wxID_DELETE, _("Remove") ); // only for removable elements
    item->Enable( m_selels.size() > 0 );
    // only display if all selected elements are notuniqs!
    for( cit_elements cit = m_selels.begin(); cit != m_selels.end(); ++cit )
      if( !(*cit)->is_removable() )
      {
        item->Enable(false);
        break;
      }
    item = m_elpopup->Append( wxID_CLEAR, _("Reset") ); // always if something is selected
  }

  PopupMenu(m_elpopup, wxDefaultPosition);
}

void ElementsCtrlBase::OnInsertNotuniq( wxCommandEvent& ev )
{
  int idx = ev.GetId() - ID_INSERT_NOTUNIQ;

  if( m_selels.size() != 1 )
    return;
  const notuniqs_type& notuniqs = wxGetApp().hudfile()->notuniq_elements();
  ElementBase *el = wxGetApp().hudfile()->create_element_from_default( notuniqs[idx] );
  wxGetApp().hudfile()->insert( m_selels.front(), el );
  wxGetApp().mainframe()->update_elementsctrl();

  long itemidx = index_by_pointer(el);
  m_list->EnsureVisible( itemidx );
  select_item( itemidx );
}

void ElementsCtrlBase::OnBeginDrag( wxListEvent& ev )
{
  ev.Allow();
  wxTextDataObject my_data(wxT("ruelps"));
  wxDropSource dragSource( this );
	dragSource.SetData( my_data );
	wxDragResult result = dragSource.DoDragDrop( TRUE );
  wxGetApp().mainframe()->statusbar()->SetStatusText(_("Ready"), (SB_MSG));
  /*
  wxString pc;
  switch ( result )
        {
            case wxDragError:   pc = _T("Error!");    break;
            case wxDragNone:    pc = _T("Nothing");   break;
            case wxDragCopy:    pc = _T("Copied");    break;
            case wxDragMove:    pc = _T("Moved");     break;
            case wxDragCancel:  pc = _T("Cancelled"); break;
            default:            pc = _T("Huh?");      break;
  }
  wxLogDebug(pc);
  */
}





void ElementsCtrlBase::update_selection()
{
  m_selidx.clear();
  m_selels.clear();
  // -- get indecies
  long idx = -1;
  wxListItem info;
  for ( ;; )
  {
    idx = m_list->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if ( idx == -1 )
        break;
    // this item is selected
    // NOTE that wxLIST_MASK_DATA| retrieval does not work on wxGTK!
    // use GetItemData(idx)
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = idx;
    if( m_list->GetItem(info) )
    {
      if( m_list->GetItemData(idx) )
      { // a real element
        m_selidx.push_back(idx);
      }
      else
      { // collection item, get all following items with same text
        wxString collname = info.GetText();
        int i = idx+1;
        while( i < m_list->GetItemCount() )
        {
          info.m_mask = wxLIST_MASK_TEXT;
          info.m_col = 1;
          info.m_itemId = i;
          if( !m_list->GetItem(info) )
            break;
          if( info.GetText().StartsWith(collname) )
            m_selidx.push_back(i);
          else
            break;
          ++i;
        }
      }
    }
  }
  m_selidx.sort();
  m_selidx.unique();

  // -- get elements
  HudFileBase *hf = wxGetApp().hudfile();
  for( cit_indecies cit = m_selidx.begin(); cit != m_selidx.end(); ++cit )
  {
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = *cit;
    if( m_list->GetItem(info) && m_list->GetItemData(*cit) )
    {
      ElementBase *el = reinterpret_cast<ElementBase*>(m_list->GetItemData(*cit));
      m_selels.push_back(el);
    }
  }  
}

bool ElementsCtrlBase::is_selected( const ElementBase* const el) const
{
  return (std::find(m_selels.begin(), m_selels.end(), el) != m_selels.end() );
}

void ElementsCtrlBase::OnSelectionChanged()
{ 
  update_selection();

  // -- first update this control's stuff that we have to do upon selection change
  m_btn_copy->Enable( m_selels.size() == 1 );
  wxGetApp().mainframe()->edit_menu()->Enable( wxID_COPY, m_btn_copy->IsEnabled() );
  m_btn_paste->Enable( m_copyfrom != 0 && m_selels.size() > 0 );
  wxGetApp().mainframe()->edit_menu()->Enable( wxID_PASTE, m_btn_paste->IsEnabled() );
  m_btn_reset->Enable( m_selels.size() > 0 );

  m_btn_insert->Enable( m_selels.size() == 1 );
  m_btn_delete->Enable( m_selels.size() > 0 );
  for( cit_elements cit = m_selels.begin(); cit != m_selels.end(); ++cit )
    if( !(*cit)->is_removable() )
    {
      m_btn_delete->Enable(false);
      break;
    }

  // -- propagate the element selection
  wxGetApp().mainframe()->OnElementSelectionChanged();
}

ElementBase* ElementsCtrlBase::pointer_by_index( long idx ) const
{
  wxUIntPtr data = m_list->GetItemData(idx);
  if( !data )
    return 0;
  return reinterpret_cast<ElementBase*>(data);
}

long ElementsCtrlBase::index_by_pointer( const ElementBase* const el ) const
{
  long idx = wxNOT_FOUND;
  wxListItem info;
  for ( ;; )
  {
    idx = m_list->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
    if ( idx == -1 || m_list->GetItemData(idx) == reinterpret_cast<const wxUIntPtr>(el) )
      break;
  }
  return idx;
}

void ElementsCtrlBase::update_items()
{
  for( int i=0; i < m_list->GetItemCount(); ++i )
    update_item(i);
}

bool ElementsCtrlBase::update_item( long idx, const ElementBase *pel /*=0*/ )
{
  if( !pel )
  { // no element pointer supplied, fetch from list itemdata
    if( !m_list->GetItemData(idx) )
      return false;
    pel = reinterpret_cast<const ElementBase*>(m_list->GetItemData(idx));
  }
  wxASSERT(pel != 0);
  wxListItem info;
  info.m_mask = wxLIST_MASK_IMAGE;
  info.m_col = 1;
  info.m_itemId = idx;
  info.m_image = (pel->is_enabled() ? E_LIST_IMG_ENABLED : E_LIST_IMG_DISABLED);
  if( 0 == (pel->flags() & E_NOTUNIQ) && !pel->is_rendered() )
    m_list->SetItemTextColour( idx, ELEMENTS_HIDDEN_COLOR );
  else
    m_list->SetItemTextColour( idx, ELEMENTS_COLOR );
  m_list->SetItem( info );
  return true;
}

bool ElementsCtrlBase::update_item( const ElementBase* const pel )
{
  // look for item index
  long idx = index_by_pointer(pel);
  if( idx == wxNOT_FOUND )
    return false;
  return update_item(idx, pel);
}


void ElementsCtrlBase::select_item( const ElementBase* const pel, bool select /*=true*/ )
{
  long idx = index_by_pointer(pel);
  if( idx == wxNOT_FOUND )
    return;
  m_list->SetItemState(idx, (select ? wxLIST_STATE_SELECTED : 0), wxLIST_STATE_SELECTED);
}
void ElementsCtrlBase::select_item( long idx, bool select /*=true*/ )
{
  if( idx == wxNOT_FOUND )
    return;
  m_list->SetItemState(idx, (select ? wxLIST_STATE_SELECTED : 0), wxLIST_STATE_SELECTED);
}

void ElementsCtrlBase::deselect_all()
{
  // TODO just ignore deselect events during this operation otherwise with each deselect we enumerate all rest selection oO
  for( int i=0; i < m_list->GetItemCount(); ++i )
    m_list->SetItemState(i, 0, wxLIST_STATE_SELECTED);
}




