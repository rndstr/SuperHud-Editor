// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef PROPERTYGRIDBASE_H
#define PROPERTYGRIDBASE_H

#include "mainframe.h"
#include "propertiesnotebookbase.h"
#include <wx/variant.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>



/// Provides a few functions that are in use on all
/// the various grids
class PropertyGridBase : public wxPropertyGridManager
{
  public:
    PropertyGridBase( 
      wxWindow *parent, 
      wxWindowID id = wxID_ANY, 
		  const wxPoint& pos = wxDefaultPosition, 
		  const wxSize& size = wxDefaultSize, 
		  long style = wxPGMAN_DEFAULT_STYLE, 
		  const wxChar* name = wxPropertyGridManagerNameStr
      ) :
    wxPropertyGridManager(parent, id, pos, size, style, name),
      m_icol(PROPS_INHERITCOLOR.to_wxColour()),
      m_ibgcol(PROPS_INHERITBGCOLOR.to_wxColour()),
      m_col(PROPS_COLOR.to_wxColour()),
      m_bgcol(PROPS_BGCOLOR.to_wxColour())

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


    /// implement this in the child class
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

#endif
