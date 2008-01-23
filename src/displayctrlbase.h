// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#ifndef DISPLAYCTRLBASE_H
#define DISPLAYCTRLBASE_H

#include "common.h"

#include "font.h"
#include <wx/gdicmn.h>
#include <map>

typedef enum {
  DRAG_NONE = 0,
  DRAG_START = 1,
  DRAG_DRAGGING = 2,
  DRAG_ABORT
} eDragMode;

typedef std::map<wxString, IFont*> fonts_type;
typedef fonts_type::iterator it_fonts;
typedef fonts_type::const_iterator cit_fonts;

class Texture;
class ElementBase;
class Model;

class DisplayCtrlBase : public wxGLCanvas
{

  public:
    DisplayCtrlBase( wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~DisplayCtrlBase() {}
        
    /// this shouldn't be called too early. preferrably in the OnPaint() function
    /// at first call
    virtual void init();
    /// you should call this in your child class' dtor
    virtual void cleanup();



    const fonts_type& fonts() const { return m_fonts; }
    IFont*    font( const wxString& name );

    const Texture* const texture_default() const { return m_texdefault; }
    const Texture* const texture_model() const { return m_texmodel; }


    void render_helper( const wxRect& rect, bool selected = false ) const;

    void reset_projection_mode();
    /// width of internal coordinate system
    virtual int width() const = 0;
    /// height of internal coordinate system
    virtual int height() const = 0;

    /// finds the item(s) at that point
    /// @arg p The point where we are looking for items
    /// @arg toggle If true and we click on a point where several items underlay then we switch to next one, otherwise we keep it.
    ElementBase*    element_hittest( const wxPoint& p, bool toggle = true );

    /// deletes the old background and loads new one (according to prefs)
    /// only call this from outside if it has changed
    void load_background();

  protected:
    bool          m_initialized;
    Texture       *m_background;
    wxRect        m_hudrect;

    ElementBase   *m_drag_el;
    eDragMode     m_drag_mode;
    wxPoint       m_drag_start; ///< mouse position when we started dragging, hudcoords

    fonts_type    m_fonts;

    Texture       *m_texdefault;
    Texture       *m_texmodel; ///< what we display if user selected model (we can't render it (yet) ourselves : ((( 

    Model         *m_fish;
    wxTimer       m_timer_anim;
    float         m_fishrot;

    virtual void render();


    void OnSize( wxSizeEvent& );
    void OnEraseBackground( wxEraseEvent& );
    void OnMouse( wxMouseEvent& );
    void OnPaint( wxPaintEvent& );
    void OnKeyDown( wxKeyEvent& );
    void OnTimerAnim( wxTimerEvent& );

    /// smallest distance (max view_snapthreshold) to other elements
    /// @returns The 2d vector we have to move the selection to snap or wxPoint(0,0) if it's disabled by prefs 
    wxPoint snap_to_elements() const;

    wxPoint snap_to_grid() const;

    void move_selected_items( int x, int y );
    void resize_selected_items( int x, int y );
    

    /// sets up the opengl view to use 2D
    void prepare2d();
    /// sets up the opengl view to use 3D
    void prepare3d();
    
    wxPoint panel_to_hud( const wxPoint& p ) const;


    DECLARE_EVENT_TABLE()

};


#endif // DISPLAYCTRLBASE_H

