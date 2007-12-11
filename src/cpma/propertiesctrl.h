#ifndef __CPMA_PROPERTIESCTRL_H__
#define __CPMA_PROPERTIESCTRL_H__

#include "../propertiesctrlbase.h"


class VisibilityPropertiesCtrl;
class FontPropertiesCtrl;
class ColorPropertiesCtrl;
class MiscPropertiesCtrl;

class CPMAPropertiesCtrl : public PropertiesCtrlBase
{
  public:
    CPMAPropertiesCtrl( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    VisibilityPropertiesCtrl  *m_vis;
    FontPropertiesCtrl      *m_font;
    ColorPropertiesCtrl     *m_color;
    MiscPropertiesCtrl        *m_misc;

};

#endif // __CPMA_PROPERTIESCTRL_H__
