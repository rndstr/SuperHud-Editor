#include "elementslistctrl.h"
#include "common.h"
#include "mainframe.h"
#include "elementsctrlbase.h"



BEGIN_EVENT_TABLE(ElementsListCtrl, wxListCtrl)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemSelected)
  EVT_LIST_ITEM_DESELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemDeselected)
  EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemActivated)
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


