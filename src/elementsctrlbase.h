#ifndef ELEMENTSCTRLBASE_H
#define ELEMENTSCTRLBASE_H

#include "common.h"

#include <wx/wx.h>
#include <wx/image.h>



// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

#include "elementslistctrl.h"
#include "elementbase.h"
#include "hudfilebase.h"
#include <list>

const wxColour ELEMENTS_COLL_COLOR = wxColour(*wxWHITE);
const wxColour ELEMENTS_COLL_BGCOLOR = wxColour(*wxBLACK);
const wxColour ELEMENTS_HIDDEN_COLOR = wxColour(177, 177, 177);
const wxColour ELEMENTS_COLOR = wxColour(*wxBLACK);

typedef std::list<int>  indecies_type;
typedef indecies_type::iterator it_indecies;
typedef indecies_type::const_iterator cit_indecies;

class ElementsCtrlBase: public wxPanel
{
  public:
    // begin wxGlade: ElementsCtrlBase::ids
    // end wxGlade

    ElementsCtrlBase(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);
    virtual ~ElementsCtrlBase();


    virtual void clear();
    virtual void append( ElementBase *el );
    virtual void list_refresh( const elements_type& elements );

    virtual void OnSelectionChanged();
    //virtual void OnMouseDClick( wxMouseEvent& ev );

    ElementsListCtrl* listctrl() { return m_list; }

    indecies_type&  selected_indecies() { return m_selidx; }
    elements_type&  selected_elements() { return m_selels; }

    /// Searches through the list for item with data pointing to the element pel
    /// and updates it accordingly to its state.
    bool          update_item( const ElementBase *pel );
    void          update_items();

    void          select_item( const ElementBase* const pel, bool select = true );
    void          select_item( long idx, bool select =true );
    void          deselect_all();
    bool          is_selected( const ElementBase* const el) const;

    /// moves the selection to the next item (disregarding collection titles)
    void          select_next();

    
    void          OnCopy( wxCommandEvent& );
    void          OnPaste( wxCommandEvent& );

  protected:
    
    /// an ordered list (by idx) of selected indecies
    /// NOTE that is the index in the list
    indecies_type   m_selidx;
    /// an ordered list (by idx) of selected elements
    elements_type   m_selels;

    ElementBase     *m_copyfrom;

    wxMenu          *m_elpopup;

    /// update m_selidx and m_selels according to current selection
    void update_selection();


  private:
    
    void          OnDelete( wxCommandEvent& );
    void          OnReset( wxCommandEvent& );
    void          OnItemSelected( wxListEvent& ev );
    void          OnItemDeselected( wxListEvent& ev );
    void          OnItemActivated( wxListEvent& ev );
    void          OnKeyDown( wxListEvent& ev );
    void          OnBeginDrag( wxListEvent& );
    void          OnItemRightClick( wxListEvent& );
    void          OnInsertNotuniq( wxCommandEvent& );
    void          OnBtnInsert( wxCommandEvent& );
    long          index_by_pointer( const ElementBase* const el ) const;
    ElementBase*  pointer_by_index( long idx ) const;
    void          show_element_popup( bool only_insert = false, const wxPoint& p = wxDefaultPosition );


    /// @arg idx The item index in the list we want to update according to pel
    /// @arg pel If this is 0 then we fetch the data ourselves (GetItemData of the idx-th item in list)
    bool          update_item( long idx, const ElementBase* const pel = 0 );

private:
    // begin wxGlade: ElementsCtrlBase::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ElementsCtrlBase::attributes
    wxBitmapButton* m_btn_insert;
    wxStaticLine* static_line_2;
    wxBitmapButton* m_btn_delete;
    ElementsListCtrl* m_list;
    wxBitmapButton* m_btn_copy;
    wxBitmapButton* m_btn_paste;
    wxStaticLine* static_line_1;
    wxBitmapButton* m_btn_reset;
    // end wxGlade
    /* FIXWXGLADE
    ElementsListCtrl* m_list;
    */

    DECLARE_EVENT_TABLE()
}; // wxGlade: end class


#endif // ELEMENTSCTRLBASE_H
