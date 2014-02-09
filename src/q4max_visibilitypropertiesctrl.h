#ifndef CPMA_VISIBILITYPROPERTIESCTRL_H
#define CPMA_VISIBILITYPROPERTIESCTRL_H


#include "q4max_propertygrid.h"

class ElementBase;

class Q4MAXVisibilityPropertiesCtrl : public Q4MAXPropertyGrid
{
  public:
    Q4MAXVisibilityPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

    void ExpandAll( bool expand = true );

  private:
    void OnRightClick( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnElementVisibility( wxCommandEvent& ev );
    void OnAlign( wxCommandEvent& ev );
    void OnBtn( wxCommandEvent& ev );
 



    DECLARE_EVENT_TABLE()

};


#endif
