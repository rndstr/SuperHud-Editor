#include "elementslistctrl.h"
#include "common.h"
#include "mainframe.h"
#include "elementsctrlbase.h"
#include "artprovider.h"





BEGIN_EVENT_TABLE(ElementsListCtrl, wxListCtrl)
  
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

  InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 18);
  InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 200);
}
  

