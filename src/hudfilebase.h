#ifndef HUDFILEBASE_H
#define HUDFILEBASE_H

#include "common.h"
#include "elementbase.h"
#include <vector>

typedef std::vector<ElementBase*>           elements_type;
typedef elements_type::iterator             it_elements;
typedef elements_type::const_iterator       cit_elements;

typedef std::vector<wxString>               notuniqs_type;
typedef notuniqs_type::const_iterator       cit_notuniqs;

/// represents a hudfile (document)
class HudFileBase
{
    friend class ListDrop;
  public:
    HudFileBase();
    virtual ~HudFileBase() { clear(); }

    void set_modified( bool modified = true );
    bool is_modified() const { return m_modified; }
    const wxString& filename() const { return m_filename; }

    /// displays open filedialog but only if supplied filename is empty, otherwise directly loads
    int OnOpen( const wxString& filename = wxT("") );


    /// sets up the document after user clicked File->New
    void OnNew();

    /// save the current hudfile (will display save filedialog if no filename is set)
    /// @return wxID_OK or wxID_CANCEL (if user pressed cancel)
    int OnSave();

    /// clears all items
    void clear(); 
    /// adds an element
    void append( ElementBase *el );
    /// returns the parent element that overwrites $specifies.
    /// parent element has flag E_PARENT
    /// @returns The parent or 0 if there is none (aka element uses default value)
    const ElementBase* get_parent( const ElementBase * const from, int specifies = E_HAS_NONE ) const;

    /// Loads a hudfile
    /// @arg wxString filename The full filename
    virtual bool load( const wxString& filename ) = 0;
    // clears all items and then loads default items
    virtual void load_default_elements() = 0;

    virtual bool save( const wxString& filename );

    void write_element( wxTextOutputStream& stream, const ElementBase& el );

    const elements_type& elements() const { return m_els; }

    virtual const notuniqs_type& notuniq_elements() const = 0;

  protected:
    ElementBase*        find_element( const wxString& name );

    /// moves an element in the list after a given other element
    /// @arg item The item to move
    /// @arg after Item will be removed and inserted after this (if it's 0 we insert at beginning)
    /// @return True if successfully moved otherwise false;
    bool                move_element_after( ElementBase *item, ElementBase *after );

  protected:
    elements_type       m_els;
    bool                m_modified;
    wxString            m_filename;
};


#endif // HUDFILEBASE_H

