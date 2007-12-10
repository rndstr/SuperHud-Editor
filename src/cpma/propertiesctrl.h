#ifndef __CPMA_PROPERTIESCTRL_H__
#define __CPMA_PROPERTIESCTRL_H__

#include "../propertiesctrlbase.h"

class PositionPropertiesCtrl;

class CPMAPropertiesCtrl : public PropertiesCtrlBase
{
  public:
    CPMAPropertiesCtrl( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    PositionPropertiesCtrl  *m_pos;

};

#endif // __CPMA_PROPERTIESCTRL_H__
