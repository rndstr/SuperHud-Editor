#ifndef Q4MAX_TEXTPROPERTIESCTRL_H
#define Q4MAX_TEXTPROPERTIESCTRL_H

#include "q4max_propertygrid.h"

class ElementBase;

class Q4MAXTextPropertiesCtrl : public Q4MAXPropertyGrid
{
  public:
    Q4MAXTextPropertiesCtrl( wxWindow *parent );

    void from_element( const ElementBase *el );
    /// @arg reset Whether to reset the items (delete&append)
    void update_layout( bool reset = true );

    void ExpandAll( bool expand = true );

  private:
    void OnItemChanged( wxPropertyGridEvent& ev );
    void OnItemChanging( wxPropertyGridEvent& ev );
    static wxString textalign_element_to_ui( const wxString& ta );
    static wxString textalign_ui_to_element( const wxString& ta );


    DECLARE_EVENT_TABLE()
};

#endif
