#ifndef __ELEMENTSLISTCTRL_H__
#define __ELEMENTSLISTCTRL_H__


#include <wx/listctrl.h>
#include <wx/imaglist.h>

/// adapted wxListCtrl that adds behaviour 
/// for use on hud elements list
class ElementsListCtrl : public wxListCtrl
{
  public:
    ElementsListCtrl( wxWindow *parent );
    virtual ~ElementsListCtrl() {}

  private:
    void          OnItemSelected( wxListEvent& ev );
    wxImageList   m_imglist;

  DECLARE_EVENT_TABLE()
};


#endif // __ELEMENTSLISTCTRL_H__

