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
    wxImageList   m_imglist;

};


#endif // __ELEMENTSLISTCTRL_H__

