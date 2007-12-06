#ifndef __ELEMENTSLISTCTRL_H__
#define __ELEMENTSLISTCTRL_H__

/// adapted wxListCtrl that adds behaviour 
/// for use on hud elements list

class ElementsListCtrl : public wxListCtrl
{
  public:
    ElementsListCtrl( wxWindow *parent ) :
      wxListCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER)
  {}
    virtual ~ElementsListCtrl() {}


};


#endif // __ELEMENTSLISTCTRL_H__

