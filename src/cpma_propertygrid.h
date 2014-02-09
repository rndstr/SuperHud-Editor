#ifndef CPMAPROPERTYGRID_H
#define CPMAPROPERTYGRID_H

#include "propertygridbase.h"
#include "cpma_element.h"

/// Provides a few functions that are in use on all
/// the various grids
class CPMAPropertyGrid : public PropertyGridBase
{
  public:
    CPMAPropertyGrid( 
      wxWindow *parent, 
      wxWindowID id = wxID_ANY, 
		  const wxPoint& pos = wxDefaultPosition, 
		  const wxSize& size = wxDefaultSize, 
		  long style = wxPGMAN_DEFAULT_STYLE, 
		  const wxChar* name = wxPropertyGridManagerNameStr
      ) : PropertyGridBase(parent, id, pos, size, style, name)
    {
    }


  protected:
    /// returns the currently selected element (as stored in CPMAPropertiesNotebook::update_from_element)
    /// @returns The current element or 0 if none selected
    const CPMAElement* current_element() const
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGrid::current_element - PropertiesNotebook is not yet available"));
        return 0;
      }
      const CPMAElement* el = static_cast<const CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGrid::current_element - there is no element selected"));
        return 0;
      }
      return el;
    }
    CPMAElement* current_element()
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGrid::current_element - PropertiesNotebook is not yet available"));
        return 0;
      }
      CPMAElement *el = static_cast<CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGrid::current_element - there is no element selected"));
        return 0;
      }
      return el;
    }
};

#endif
