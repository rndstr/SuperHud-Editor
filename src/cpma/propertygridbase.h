#ifndef __CPMA_PROPERTYGRIDBASE_H__
#define __CPMA_PROPERTYGRIDBASE_H__

#include "element.h"
#include "../mainframe.h"
#include "../propertiesnotebookbase.h"
#include <wx/variant.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

const wxColour PROPS_COLOR_INHERITED_DEFAULT = wxColour(100, 100, 150);
const wxColour PROPS_BGCOLOR_INHERITED_DEFAULT = wxColour(240, 240, 255);
const wxColour PROPS_COLOR_NORMAL_DEFAULT = wxColour(0, 0, 0);
const wxColour PROPS_BGCOLOR_NORMAL_DEFAULT = wxColour(255, 255, 255);

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
    wxPropertyGridManager(parent, id, pos, size, style, name),
      m_icol(PROPS_COLOR_INHERITED_DEFAULT),
      m_ibgcol(PROPS_BGCOLOR_INHERITED_DEFAULT),
      m_col(PROPS_COLOR_NORMAL_DEFAULT),
      m_bgcol(PROPS_BGCOLOR_NORMAL_DEFAULT)

    {
    }

    void set_colors( const wxColour& icol, const wxColour& ibgcol, const wxColour& col, const wxColour& bgcol )
    {
      m_icol = icol;
      m_ibgcol = ibgcol;
      m_col = col;
      m_bgcol = bgcol;
    }


    /*
    /// reimplement ExpandAll  which was removed in wxpropgrid 1.3
    // FIXME this crashes upon leaving item->noitem :| so use selective Expand/Collapse(propid) which seems to work
    bool ExpandAll( bool expand = true )
    {
      wxPGVIterator it;
      for( it = GetVIterator(wxPG_ITERATE_ALL); !it.AtEnd(); it.Next() )
      {
        wxPGPropertyWithChildren* pwc = (wxPGPropertyWithChildren*) it.GetProperty();
        if( pwc->GetParentingType() != 0 ) //PT_NONE
          pwc->SetExpanded( expand );
      }
      RefreshGrid();
      return true;
    }
    */

    virtual void ExpandAll( bool expand = true )
    {
    }

    virtual void CollapseAll( bool collapse = true )
    {
      ExpandAll(!collapse);
    }

    /// as wxPropGrid does not support the additional bool we implement that here
    bool Expand( wxPGPropArg id, bool expand = true )
    {
      if( expand )
        return wxPropertyGridManager::Expand(id);
      return wxPropertyGridManager::Collapse(id);
    }

  protected:
    wxColour m_icol; ///< text color for inherited values
    wxColour m_ibgcol; ///< background color for inherited values
    wxColour m_col; ///< text color for own values
    wxColour m_bgcol; ///< background color for own values



    /// returns the currently selected element (as stored in CPMAPropertiesNotebook::update_from_element)
    /// @returns The current element or 0 if none selected
    const CPMAElement* current_element() const
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
      if( !p )
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - PropertiesNotebook is not yet available"));
        return 0;
      }
      const CPMAElement* el = static_cast<const CPMAElement*>(p->curel());
      if( !el ) 
      {
        wxLogDebug(wxT("CPMAPropertyGridBase::curel() - there is no element selected"));
        return 0;
      }
      return el;
    }
    CPMAElement* current_element()
    {
      PropertiesNotebookBase *p = wxGetApp().mainframe()->propertiesctrl();
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
        SetPropertyTextColour( id, m_icol );
        SetPropertyBackgroundColour( id, m_ibgcol );
      }
      else
      {
        SetPropertyTextColour( id, m_col );
        SetPropertyBackgroundColour( id, m_bgcol );
      }
    }
    /// colorizes the property accordingly
    void property_defines( wxPGPropArg id, bool defines = true )
    {
      property_inherits(id, !defines);
    }
};

#endif // __CPMA_PROPERTYGRIDBASE_H__
