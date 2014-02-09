#ifndef CPMACOLORPROPERTIESCTRL_H
#define CPMACOLORPROPERTIESCTRL_H

#include "cpma_propertygrid.h"

class ElementBase;

class ColorPropertiesCtrl : public CPMAPropertyGrid
{
  public:
    ColorPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    void update_layout();

    void ExpandAll( bool expand = true );

  private:
    wxColourProperty *m_color;

    void OnItemChanging( wxPropertyGridEvent& ev );
    void OnItemChanged( wxPropertyGridEvent& ev );

    static wxString textalign_element_to_ui( const wxChar& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif
