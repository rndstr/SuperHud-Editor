#include "elementslistctrl.h"

#include "xpm/icons/element_disabled.xpm"
#include "xpm/icons/element_enabled.xpm"

ElementsListCtrl::ElementsListCtrl( wxWindow *parent ) :
  wxListCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER),
  m_imglist(16, 16, TRUE)
{
  SetImageList( &m_imglist, wxIMAGE_LIST_SMALL );
  m_imglist.Add(wxBitmap(element_disabled_xpm));
  m_imglist.Add(wxBitmap(element_enabled_xpm));
  InsertColumn(0, _("On?"), wxLIST_FORMAT_LEFT, 20);
  InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 200);
}

