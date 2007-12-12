#ifndef __PROPERTIESNOTEBOOKBASE_H__
#define __PROPERTIESNOTEBOOKBASE_H__

#include "superhudeditor.h"
#include "elementbase.h"
#include "hudfilebase.h"
#include <wx/aui/auibook.h>

const wxColour PROPS_COLOR_INHERITED = wxColour(100, 100, 150);
const wxColour PROPS_BGCOLOR_INHERITED = wxColour(240, 240, 255);
const wxColour PROPS_COLOR_NORMAL = wxColour(0, 0, 0);
const wxColour PROPS_BGCOLOR_NORMAL = wxColour(255, 255, 255);

class PropertiesNotebookBase : public wxAuiNotebook
{
  public:
    PropertiesNotebookBase( wxWindow* parent ) :
      wxAuiNotebook( parent, ID_NOTEBOOK_PROPERTIES, wxDefaultPosition, wxDefaultSize, 
        wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TOP ),
        m_curel(0)
    {
    }

    virtual void update_from_element( const elements_type& els ) = 0;

    ElementBase     *curel() { return m_curel; }

  protected:
    ElementBase   *m_curel;///< element that is currently being displayed



};

#endif // __PROPERTIESCTRLBASE_H__
