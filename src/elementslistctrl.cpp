#include "elementslistctrl.h"
#include "common.h"
#include "mainframe.h"
#include "elementsctrlbase.h"

#include "xpm/icons/element_disabled.xpm"
#include "xpm/icons/element_enabled.xpm"
#include "xpm/icons/elements_collection_title.xpm"
#include "xpm/icons/elements_collection_item.xpm"

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
  m_imglist.Add(wxBitmap(element_disabled_xpm));
  m_imglist.Add(wxBitmap(element_enabled_xpm));
  m_imglist.Add(wxBitmap(elements_collection_title_xpm));
  m_imglist.Add(wxBitmap(elements_collection_item_xpm));

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
  ElementsCtrlBase::indecies_type sels = ((ElementsCtrlBase*)GetParent())->get_selection();
  ElementBase *pel = 0;
  for( ElementsCtrlBase::it_indecies it = sels.begin(); it != sels.end(); ++it )
  {
    wxASSERT( GetItemData(*it) );
    if( GetItemData(*it) )
    {
      pel = reinterpret_cast<ElementBase*>( GetItemData(*it) );
      pel->set_enabled( !pel->is_enabled() ); // toggle
      update_item(*it, pel);
    }
  }
  wxGetApp().hudfile()->set_modified();
  // trigger update of other views relying on properties
  ((ElementsCtrlBase*)GetParent())->OnSelectionChanged();
}

bool ElementsListCtrl::update_item( long idx, ElementBase *pel /*=0*/ )
{
  if( !pel )
  { // fetch
    if( !GetItemData(idx) )
      return false;
    pel = (ElementBase*)GetItemData(idx);
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


