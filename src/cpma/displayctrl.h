#ifndef __CPMADISPLAYCTRL_H__
#define __CPMADISPLAYCTRL_H__

#include "../displayctrlbase.h"
class CPMADisplayCtrl : public DisplayCtrlBase
{
  public:
    CPMADisplayCtrl( wxWindow *parent ) :
      DisplayCtrlBase(parent)
    {
    }

};

#endif // __CPMADISPLAYCTRL_H__

