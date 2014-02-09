#ifndef CPMA_FONTPROPERTIESCTRL_H
#define CPMA_FONTPROPERTIESCTRL_H

#include "cpma_propertygrid.h"

class ElementBase;

class FontPropertiesCtrl : public CPMAPropertyGrid
{
  public:
    FontPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    /// @arg reset Whether to reset the items (delete&append)
    void update_layout( bool reset = true );

    void ExpandAll( bool expand = true );

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxChar& ta );
    static wxChar textalign_ui_to_element( const wxString& ta );
    static int fontsizetype_ui_to_element( const wxString& fs );
    static wxString fontsizetype_element_to_ui( int fst );


    DECLARE_EVENT_TABLE()
};

#endif
