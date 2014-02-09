#ifndef Q4MAXPROPERTIESNOTEBOOK_H
#define Q4MAXPROPERTIESNOTEBOOK_H

#include "propertiesnotebookbase.h"


class Q4MAXVisibilityPropertiesCtrl;

class Q4MAXPropertiesNotebook : public PropertiesNotebookBase
{
  public:
    Q4MAXPropertiesNotebook( wxWindow *parent );

    void update_from_element( const elements_type& els );

  private:
    Q4MAXVisibilityPropertiesCtrl   *m_vis;

};

#endif
