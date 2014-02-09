#ifndef Q4MAXPROPERTYGRID_H
#define Q4MAXPROPERTYGRID_H

#include "propertygridbase.h"
#include "q4max_element.h"

/// Provides a few functions that are in use on all
/// the various grids
class Q4MAXPropertyGrid : public PropertyGridBase
{
  public:
    Q4MAXPropertyGrid( 
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
    const Q4MAXElement* current_element() const
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("Q4MAXPropertyGrid::current_element - PropertiesNotebook is not yet available"));
        return 0;
      }
      const Q4MAXElement* el = static_cast<const Q4MAXElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("Q4MAXPropertyGrid::current_element - there is no element selected"));
        return 0;
      }
      return el;
    }
    Q4MAXElement* current_element()
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("Q4MAXPropertyGrid::current_element - PropertiesNotebook is not yet available"));
        return 0;
      }
      Q4MAXElement *el = static_cast<Q4MAXElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("Q4MAXPropertyGrid::current_element - there is no element selected"));
        return 0;
      }
      return el;
    }
};

#endif
