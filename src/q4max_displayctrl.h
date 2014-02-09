#ifndef Q4MAXDISPLAYCTRL_H
#define Q4MAXDISPLAYCTRL_H

#include "displayctrlbase.h"

class Q4MAXDisplayCtrl : public DisplayCtrlBase
{
    

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    
  public:
    Q4MAXDisplayCtrl( wxWindow *parent );
    ~Q4MAXDisplayCtrl();

    void init();

  protected:
  
    int width() const { return Q4MAXDisplayCtrl::WIDTH; }
    int height() const { return Q4MAXDisplayCtrl::HEIGHT; }

};

#endif

