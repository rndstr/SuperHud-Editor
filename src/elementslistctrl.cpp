#include "elementslistctrl.h"
#include "common.h"
#include "mainframe.h"
#include "elementsctrlbase.h"

#include <wx/dnd.h>


// NOTE that this probably isn't the way it's supposed to be
// we accept text although we are actually only accepting Elements.
// but hey, ... we only need to know if there was a drop and then
// move all selected elements to there.
class ListDrop : public wxTextDropTarget
{
  public:
    ListDrop( ElementsListCtrl *list) : m_list(list) {}
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
    {
      const elements_type& els = static_cast<ElementsCtrlBase*>(m_list->GetParent())->selected_elements();
      const indecies_type& idx = static_cast<ElementsCtrlBase*>(m_list->GetParent())->selected_indecies();
      if( 0 == els.size() )
        return false;
      wxLogDebug(wxT("DROP ") + data);
      int flags = wxLIST_HITTEST_BELOW;
      long hit = hititem(x, y);
      if( hit == wxNOT_FOUND )
        return false;
       wxASSERT( m_list->GetItemData(hit) );

      ElementBase *after = reinterpret_cast<ElementBase*>(m_list->GetItemData(hit));
      // move all items
      for( size_t rev = els.size(); rev != 0; --rev )
        wxGetApp().hudfile()->move_element_after( els[rev-1], after );

      // TODO simple but expensive ;) hooray
      static_cast<ElementsCtrlBase*>(m_list->GetParent())->list_refresh(wxGetApp().hudfile()->elements());
      // TODO reselect items :o
      // TODO scroll to item

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
      static wxRect bound;
      static bool drawn = false;
      long hit = hititem(x, y);
      wxSize s = m_list->GetSize();
      wxLogDebug(wxT("%d # %d %d / %d %d"), hit, x, y, s.x, s.y);
      

      // scroll?
      wxRect rh;
      m_list->GetItemRect(0, rh, wxLIST_RECT_BOUNDS);
      int scroll = rh.GetHeight();
      int snap = rh.GetHeight()*2;
      if( y < snap )
        m_list->ScrollList(0, -scroll);
      else if( y > s.y - snap )
        m_list->ScrollList(0, scroll);


      // write in statusbar as long as we don't have a visual indication where it's inserted :x
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

      // draw a line below

      // FIXME while moving upwards this does leave some lines there :|
      /*
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

BEGIN_EVENT_TABLE(ElementsListCtrl, wxListCtrl)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemSelected)
  EVT_LIST_ITEM_DESELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemDeselected)
  EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemActivated)
  EVT_LIST_BEGIN_DRAG(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnBeginDrag)
END_EVENT_TABLE()

ElementsListCtrl::ElementsListCtrl( wxWindow *parent ) :
  wxListCtrl( parent, ID_LISTCTRL_ELEMENTS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER),
  m_imglist(16, 16, TRUE)
{
  SetImageList( &m_imglist, wxIMAGE_LIST_SMALL );
  m_imglist.Add(wxArtProvider::GetBitmap(ART_ELEMENT_DISABLED, wxART_OTHER, wxSize(16,16)));
  m_imglist.Add(wxArtProvider::GetBitmap(ART_ELEMENT_ENABLED, wxART_OTHER, wxSize(16,16)));
  m_imglist.Add(wxArtProvider::GetBitmap(ART_ELEMENTS_COLLECTION_TITLE, wxART_OTHER, wxSize(16,16)));
  m_imglist.Add(wxArtProvider::GetBitmap(ART_ELEMENTS_COLLECTION_ITEM, wxART_OTHER, wxSize(16,16)));

  InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 20);
  InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 200);
  ListDrop *ld = new ListDrop(this);
  SetDropTarget(ld);
}
  

void ElementsListCtrl::OnItemDeselected( wxListEvent& ev )
{
  ((ElementsCtrlBase*)GetParent())->OnSelectionChanged();
}

void ElementsListCtrl::OnItemSelected( wxListEvent& ev )
{
  ((ElementsCtrlBase*)GetParent())->OnSelectionChanged();
}

void ElementsListCtrl::OnItemActivated( wxListEvent& ev )
{
  elements_type& els = static_cast<ElementsCtrlBase*>(GetParent())->selected_elements();
  indecies_type& idx = static_cast<ElementsCtrlBase*>(GetParent())->selected_indecies();
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

void ElementsListCtrl::OnBeginDrag( wxListEvent& ev )
{
  ev.Allow();
  wxGetApp().mainframe()->statusbar()->PushStatusText(_("Drag&Drop"), SB_MSG);
  wxTextDataObject my_data(wxT("This text will be dragged."));
  wxDropSource dragSource( this );
	dragSource.SetData( my_data );
	wxDragResult result = dragSource.DoDragDrop( TRUE );
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
  wxGetApp().mainframe()->statusbar()->PopStatusText(SB_MSG);
  
  
  //elements_type& els = static_cast<ElementsCtrlBase*>(GetParent())->selected_elements();
  //ev.Get

}


bool ElementsListCtrl::update_item( long idx, const ElementBase *pel )
{
  if( !pel )
  { // fetch
    if( !GetItemData(idx) )
      return false;
    pel = reinterpret_cast<const ElementBase*>(GetItemData(idx));
  }
  wxASSERT(pel != 0);
  wxListItem info;
  info.m_mask = wxLIST_MASK_IMAGE;
  info.m_col = 1;
  info.m_itemId = idx;
  info.m_image = (pel->is_enabled() ? E_LIST_IMG_ENABLED : E_LIST_IMG_DISABLED);
  SetItem( info );
  return true;
}

bool ElementsListCtrl::update_item( const ElementBase *pel )
{
  // look for item index
  long idx = -1;
  wxListItem info;
  for ( ;; )
  {
    idx = GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
    if ( idx == -1 )
        break;
    if( GetItemData(idx) == reinterpret_cast<const wxUIntPtr>(pel) )
      return update_item(idx, pel);
  }
  return false;
}


