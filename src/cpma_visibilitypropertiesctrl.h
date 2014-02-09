#ifndef CPMA_VISIBILITYPROPERTIESCTRL_H
#define CPMA_VISIBILITYPROPERTIESCTRL_H


#include "cpma_propertygrid.h"

class ElementBase;

class CPMAVisibilityPropertiesCtrl : public CPMAPropertyGrid
{
  public:
    CPMAVisibilityPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

    void ExpandAll( bool expand = true );

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnElementVisibility( wxCommandEvent& ev );
    void OnAlign( wxCommandEvent& ev );
 



    DECLARE_EVENT_TABLE()

};


#endif
