#ifndef __CPMA_IMAGEPROPERTIESCTRL_H__
#define __CPMA_IMAGEPROPERTIESCTRL_H__

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

#include "propertygridbase.h"

class ElementBase;

class ImagePropertiesCtrl : public CPMAPropertyGridBase
{
  public:
    ImagePropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

  private:
    static bool use_model( CPMAElement *el );
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    void append_picture_properties( const CPMAElement *el =0 );
    void append_model_properties( const CPMAElement *el =0 );



    DECLARE_EVENT_TABLE()
};

#endif // __FONTPROPERTIESCTRL_H__
