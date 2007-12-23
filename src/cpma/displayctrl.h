#ifndef __CPMADISPLAYCTRL_H__
#define __CPMADISPLAYCTRL_H__

#include "../displayctrlbase.h"
class CPMADisplayCtrl : public DisplayCtrlBase
{
  static const int WIDTH = 640;
  static const int HEIGHT = 480;
  public:
    CPMADisplayCtrl( wxWindow *parent ) :
      DisplayCtrlBase(parent)
    {
    }

    void init();
    void cleanup();

    void load_background();
    void OnIdle( wxIdleEvent& );

    void render();

  protected:
    
    int width() const { return CPMADisplayCtrl::WIDTH; }
    int height() const { return CPMADisplayCtrl::HEIGHT; }

};

#endif // __CPMADISPLAYCTRL_H__

