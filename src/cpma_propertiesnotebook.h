#ifndef CPMAPROPERTIESNOTEBOOK_H
#define CPMAPROPERTIESNOTEBOOK_H

#include "propertiesnotebookbase.h"


class CPMAVisibilityPropertiesCtrl;
class FontPropertiesCtrl;
class ColorPropertiesCtrl;
class MiscPropertiesCtrl;
class ImagePropertiesCtrl;

class CPMAPropertiesNotebook : public PropertiesNotebookBase
{
  public:
    CPMAPropertiesNotebook( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    CPMAVisibilityPropertiesCtrl  *m_vis;
    FontPropertiesCtrl      *m_font;
    ColorPropertiesCtrl     *m_color;
    ImagePropertiesCtrl     *m_image;
    MiscPropertiesCtrl        *m_misc;

};

#endif
