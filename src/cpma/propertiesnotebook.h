#ifndef __CPMA_PROPERTIESNOTEBOOK_H__
#define __CPMA_PROPERTIESNOTEBOOK_H__

#include "../propertiesnotebookbase.h"


class VisibilityPropertiesCtrl;
class FontPropertiesCtrl;
class ColorPropertiesCtrl;
class MiscPropertiesCtrl;

class CPMAPropertiesNotebook : public PropertiesNotebookBase
{
  public:
    CPMAPropertiesNotebook( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    VisibilityPropertiesCtrl  *m_vis;
    FontPropertiesCtrl      *m_font;
    ColorPropertiesCtrl     *m_color;
    MiscPropertiesCtrl        *m_misc;

};

#endif // __CPMA_PROPERTIESNOTEBOOK_H__
