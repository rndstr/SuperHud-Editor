#ifndef __CPMA_PROPERTYGRIDBASE_H__
#define __CPMA_PROPERTYGRIDBASE_H__

#include "element.h"
#include "../mainframe.h"
#include "../propertiesnotebookbase.h"

/// Provides a few functions that are in use on all
/// the various grids
class CPMAPropertyGridBase : public wxPropertyGridManager
{
  public:
    CPMAPropertyGridBase( 
      wxWindow *parent, 
      wxWindowID id = wxID_ANY, 
		  const wxPoint& pos = wxDefaultPosition, 
		  const wxSize& size = wxDefaultSize, 
		  long style = wxPGMAN_DEFAULT_STYLE, 
		  const wxChar* name = wxPropertyGridManagerNameStr
      ) :
    wxPropertyGridManager(parent, id, pos, size, style, name)
    {
    }

    /// returns the currently selected element (as stored in CPMAPropertiesNotebook::update_from_element)
    /// @returns The current element or 0 if none selected
    CPMAElement* current_element() const
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesnotebook();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - PropertiesNotebook is not yet available"));
        return 0;
      }
      CPMAElement *el = static_cast<CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - there is no element selected"));
        return 0;
      }
      return el;
    }

    /// colorizes the property accordingly
    void property_inherits(wxPGPropArg id, bool inherits = true)
    {
      if( inherits )
      {
        SetPropertyTextColour( id, PROPS_COLOR_INHERITED );
        SetPropertyBackgroundColour( id, PROPS_BGCOLOR_INHERITED );
      }
      else
      {
        SetPropertyTextColour( id, PROPS_COLOR_NORMAL );
        SetPropertyBackgroundColour( id, PROPS_BGCOLOR_NORMAL );
      }
    }
    /// colorizes the property accordingly
    void property_defines( wxPGPropArg id, bool defines = true )
    {
      property_inherits(id, !defines);
    }
};

#endif // __CPMA_PROPERTYGRIDBASE_H__
