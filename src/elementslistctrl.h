#ifndef ELEMENTSLISTCTRL_H
#define ELEMENTSLISTCTRL_H

#include "common.h"

#include "elementbase.h"

#include <wx/listctrl.h>
#include <wx/imaglist.h>

enum
{
  E_LIST_IMG_DISABLED = 0,
  E_LIST_IMG_ENABLED = 1,
  E_LIST_IMG_COLLTITLE = 2,
  E_LIST_IMG_COLLITEM = 3
};








/// adapted wxListCtrl that adds behaviour 
/// for use on hud elements list
class ElementsListCtrl : public wxListCtrl
{
  public:
    ElementsListCtrl( wxWindow *parent );
    virtual ~ElementsListCtrl() {}

    


  private:
    
    wxImageList   m_imglist;

  DECLARE_EVENT_TABLE()
};


#endif // ELEMENTSLISTCTRL_H

