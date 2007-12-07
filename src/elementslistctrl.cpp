#include "elementslistctrl.h"
#include "common.h"

#include "xpm/icons/element_disabled.xpm"
#include "xpm/icons/element_enabled.xpm"
#include "xpm/icons/elements_collection.xpm"
#include "xpm/icons/elements_collection_marker.xpm"

BEGIN_EVENT_TABLE(ElementsListCtrl, wxListCtrl)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemSelected)
END_EVENT_TABLE()

ElementsListCtrl::ElementsListCtrl( wxWindow *parent ) :
  wxListCtrl( parent, ID_LISTCTRL_ELEMENTS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER),
  m_imglist(16, 16, TRUE)
{
  SetImageList( &m_imglist, wxIMAGE_LIST_SMALL );
  m_imglist.Add(wxBitmap(element_disabled_xpm));
  m_imglist.Add(wxBitmap(element_enabled_xpm));
  m_imglist.Add(wxBitmap(elements_collection_xpm));
  m_imglist.Add(wxBitmap(elements_collection_marker_xpm));

  InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 20);
  InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 200);
}
void ElementsListCtrl::OnItemSelected( wxListEvent& ev )
{
  wxLogDebug(wxT("SEL"));
  if( ev.GetData() == 0 )
  { // collection
    const wxListItem& sel = ev.GetItem();
    wxListItem info;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = sel.GetId();
    if( GetItem(info) )
    {
      wxString collname = info.GetText();
      if( !collname.empty() )
      { // now select all following items that start with that collname
        int id = sel.GetId()+1;
        while( id < GetItemCount() )
        {
          info.m_mask = wxLIST_MASK_TEXT;
          info.m_col = 1;
          info.m_itemId = id;
          GetItem(info);
          wxLogDebug(wxT("checking %s (%s)"), info.GetText().c_str(), collname.c_str());
          if( !info.GetText().StartsWith(collname) )
          {
            wxLogDebug(wxT(" NOPE"));
            break;
          }
          info.m_mask = wxLIST_MASK_STATE;
          info.m_state = wxLIST_STATE_SELECTED;
          info.m_stateMask = wxLIST_STATE_SELECTED;
          SetItem(info);
          ++id;
        }

      }
    }
    else
      wxLogDebug(wxT("Received no text from column"));
  }
}

