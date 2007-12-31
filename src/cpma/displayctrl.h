#ifndef __CPMADISPLAYCTRL_H__
#define __CPMADISPLAYCTRL_H__

#include "../displayctrlbase.h"

#include <map>

class CPMAFont;

class CPMADisplayCtrl : public DisplayCtrlBase
{
    typedef std::map<wxString, CPMAFont*> fonts_type;

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    
  public:
    CPMADisplayCtrl( wxWindow *parent ) :
      DisplayCtrlBase(parent)
    {
    }

    void init();
    void cleanup();
    void render();

  private:

  protected:
    void OnIdle( wxIdleEvent& );
    void load_background();
    int width() const { return CPMADisplayCtrl::WIDTH; }
    int height() const { return CPMADisplayCtrl::HEIGHT; }
  private:
    fonts_type   m_fonts;

};

#endif // __CPMADISPLAYCTRL_H__

