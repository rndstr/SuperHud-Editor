#ifndef CPMA_IMAGEPROPERTIESCTRL_H
#define CPMA_IMAGEPROPERTIESCTRL_H

#include "cpma_propertygrid.h"

class ElementBase;

class ImagePropertiesCtrl : public CPMAPropertyGrid
{
  public:
    ImagePropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    /// @arg reset Whether to reset the items (delete&append)
    void update_layout( bool reset = true );

    void ExpandAll( bool expand = true );

  private:
    static bool use_model( CPMAElement *el );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    void append_picture_properties( const CPMAElement *el =0 );
    void append_model_properties( const CPMAElement *el =0 );



    DECLARE_EVENT_TABLE()
};

#endif
