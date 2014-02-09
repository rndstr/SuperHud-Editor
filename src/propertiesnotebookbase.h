#ifndef PROPERTIESNOTEBOOKBASE_H
#define PROPERTIESNOTEBOOKBASE_H

#include "common.h"

#include "superhudeditor.h"
#include "hudfilebase.h"
#include "color4.h"
#include <wx/aui/auibook.h>

const Color4 PROPS_INHERITCOLOR = Color4(0.f, 0.f, 0.f);
const Color4 PROPS_INHERITBGCOLOR = Color4(0.937f, 0.937f, 1.f);
const Color4 PROPS_COLOR = Color4(0.f, 0.f, 0.f);
const Color4 PROPS_BGCOLOR = Color4(1.f, 1.f, 1.f);

class ElementBase;

class PropertiesNotebookBase : public wxAuiNotebook
{
  public:
    PropertiesNotebookBase( wxWindow* parent ) :
      wxAuiNotebook( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, wxDefaultSize, 
        wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TOP | wxAUI_NB_SCROLL_BUTTONS ),
        m_curel(0)
    {
    }

    virtual void update_from_element( const elements_type& els ) = 0;

    ElementBase*       curel() { return m_curel; }
    const ElementBase* curel() const { return m_curel; }

  protected:
    ElementBase   *m_curel;///< element that is currently being displayed



};

#endif
