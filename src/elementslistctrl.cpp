#include "elementslistctrl.h"
#include "common.h"
#include "mainframe.h"

#include <wx/sstream.h>
#include <wx/txtstrm.h>

#include "xpm/icons/element_disabled.xpm"
#include "xpm/icons/element_enabled.xpm"
#include "xpm/icons/elements_collection_title.xpm"
#include "xpm/icons/elements_collection_item.xpm"

#include <list>
using namespace std;

BEGIN_EVENT_TABLE(ElementsListCtrl, wxListCtrl)
  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemSelected)
  EVT_LIST_ITEM_DESELECTED(ID_LISTCTRL_ELEMENTS, ElementsListCtrl::OnItemDeselected)
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
void ElementsListCtrl::OnSelectionChanged()
{
  // update textcontrol
  wxTextCtrl *txt = wxGetApp().mainframe()->textpreview();

  wxString out;
  wxStringOutputStream sos(&out);
  wxTextOutputStream tos(sos);
  // get all selected
  long idx = -1;
  list<int> print; // stores all indecies that should be printed
  wxListItem info;
  for ( ;; )
  {
    idx = GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if ( idx == -1 )
        break;
    // this item is selected
    info.m_mask = wxLIST_MASK_DATA|wxLIST_MASK_TEXT;
    info.m_col = 1;
    info.m_itemId = idx;
    if( GetItem(info) )
    {
      if( info.GetData() )
      { // a real element
        print.push_back(idx);
        
        
      }
      else
      { // collection item, get all following items with same text
        wxString collname = info.GetText();
        int i = idx+1;
        while( i < GetItemCount() )
        {
          info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA;
          info.m_col = 1;
          info.m_itemId = i;
          if( !GetItem(info) )
            break;
          if( info.GetText().StartsWith(collname) )
            print.push_back(i);
          else
            break;
          ++i;
        }
      }
    }
  }
  print.sort();
  print.unique();
  for( list<int>::iterator it = print.begin(); it != print.end(); ++it )
  {
    info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA;
    info.m_col = 1;
    info.m_itemId = *it;
    if( GetItem(info) )
    {
      ElementBase *el = (ElementBase*)info.GetData();
      HudFileBase::write_element(tos, *el);
    }
    else
      tos << wxT("\n# ERROR: failed retrieving iteminfo\n");
  }
  
  txt->SetValue(out);
}
void ElementsListCtrl::OnItemDeselected( wxListEvent& ev )
{
  OnSelectionChanged();
}

void ElementsListCtrl::OnItemSelected( wxListEvent& ev )
{
  OnSelectionChanged();
  
  /*
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
          SetItemState( id, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
          ++id;
        }

      }
    }
    else
      wxLogDebug(wxT("Received no text from column"));
  }
  */
}

