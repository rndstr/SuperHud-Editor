#ifndef CPMADISPLAYCTRL_H
#define CPMADISPLAYCTRL_H

#include "displayctrlbase.h"

class CPMADisplayCtrl : public DisplayCtrlBase
{
    

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    
  public:
    CPMADisplayCtrl( wxWindow *parent );
    ~CPMADisplayCtrl();

    void init();

  protected:
  
    int width() const { return CPMADisplayCtrl::WIDTH; }
    int height() const { return CPMADisplayCtrl::HEIGHT; }

};

#endif

