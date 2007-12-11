#ifndef __ELEMENTSLISTCTRL_H__
#define __ELEMENTSLISTCTRL_H__

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

    bool          update_item( const ElementBase *pel );


  private:
    void          OnItemSelected( wxListEvent& ev );
    void          OnItemDeselected( wxListEvent& ev );
    void          OnItemActivated( wxListEvent& ev );
    bool          update_item( long idx, const ElementBase *pel );
      
    
    wxImageList   m_imglist;

  DECLARE_EVENT_TABLE()
};


#endif // __ELEMENTSLISTCTRL_H__

