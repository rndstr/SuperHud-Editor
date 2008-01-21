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
#ifndef HUDFILEBASE_H
#define HUDFILEBASE_H

#include "common.h"

#include "elementbase.h"
#include "hudspecs.h"

#include <vector>

/// Used for algos trying to iterate through all PostDecorate elements.
const wxString HF_POSTDECORATE_PREFIX = wxT("PostDecorate");
/// Used for algos trying to iterate through all PreDecorate elements.
const wxString HF_PREDECORATE_PREFIX = wxT("PreDecorate");

#ifdef CPMA_BACKWARD_COMPATIBILITY_142
  const wxString CPMA_HF_POSTDECORATE_PREFIX_142 = wxT("_PostDecorate");
  const wxString CPMA_HF_PREDECORATE_PREFIX_142 = wxT("!PreDecorate");
#endif

/// available property(command) delimiters: newline or semicolon.
/// NOTE that for q4max we "fix" the properties first by inserting semicolons
/// between multiple elements on same line.. as q4max allows several nonseparated
/// eleemnts on one line (although arguments have to be surrounded by doublequotes)
const wxString HF_PROPERTY_DELIM = wxT("\n;");
/// what we trim for.
const wxString HF_PROPERTY_TRIM = wxT(" \n\t\r");


typedef std::vector<ElementBase*>           elements_type;
typedef elements_type::iterator             it_elements;
typedef elements_type::const_iterator       cit_elements;



/// represents a hudfile (document)
class HudFileBase
{
    friend class ReorderCommand;
    friend class ListDrop;
  public:
    HudFileBase();
    virtual ~HudFileBase() { cleanup(); }

    void set_modified( bool modified = true );
    bool is_modified() const { return m_modified; }
    const wxString& filename() const { return m_filename; }

    /// displays open filedialog but only if supplied filename is empty, otherwise directly loads
    /// @arg force_convert This overrules the load_autoconvert setting (thus if a user clicks File->New we can open it in widescreen)
    int                   OnOpen( const wxString& filename = wxT(""), bool force_convert = false );


    /// sets up the document after user clicked File->New
    void                  OnNew();

    /// save the current hudfile (will display save filedialog if no filename is set)
    /// @return wxID_OK or wxID_CANCEL (if user pressed cancel)
    int                   OnSave( bool saveas = false );

    /// clears all items (and sets modified flag)
    void                  clear(); 
    /// adds an element
    void                  append( ElementBase *el );
    /// inserts an element at given position
    void                  insert( size_t pos, ElementBase *el );
    /// inserts an element after given element
    void                  insert( const ElementBase * const after, ElementBase *el );
    /// removes an element
    /// @return True if element was removed(found) otherwise false
    bool                  remove( ElementBase *el );


    /// returns the parent element that overwrites $specifies.
    /// parent element has flag E_PARENT
    /// @returns The parent or 0 if there is none (aka element uses default value)
    const ElementBase*    get_parent( const ElementBase * const from, int specifies = 0 ) const;

    /// Loads a hudfile
    /// NOTE make sure that you don't forget to read the (hud)options like m_opt_version&co
    /// @arg wxString filename The filename
    virtual bool          load( const wxString& filename );

    // clears all items and then loads default items
    virtual void          load_default_elements() = 0;

    virtual bool          save( const wxString& filename );

    /// the default hud filename, return empty if none
    virtual wxString      default_hudfilename() const = 0;

    

    /// writes the header that preceeds every file we write
    void                  write_header( wxTextOutputStream& stream );
    /// writes the element to the given stream
    void                  write_element( wxTextOutputStream& stream, const ElementBase& el );

    const elements_type&  elements() const { return m_els; }

    void                  convert_all( double from, double to, bool size, bool stretchposition, bool fontsize);

    /// returns a list of names of notuniq elements
    const notuniqs_type& notuniq_elements() const;

    wxString              opt_version() const { return m_opt_version; }
    wxString              opt_aspectratio() const { return m_opt_aspectratio; }

#ifndef NDEBUG
    /// prints some information
    void                  debug() const;
#endif
    /// looks for an element in the element list (m_els)
    ElementBase*          find_element( const wxString& name );

    /// creates a default element from given name
    /// @returns The newly created element, if name was not found 0
    ElementBase*          create_element_from_default( const wxString& name ) const
    {
      const hsitem_s *item = HudSpecs::get().find_item(name);
      if( !item ) return 0;
      return create_element(*item);
    }

  protected:

    /// moves an element in the list after a given other element
    /// @arg item The item to move
    /// @arg after Item will be removed and inserted after this (if it's 0 we insert at beginning)
    /// @return True if successfully moved otherwise false;
    bool                  move_element_after( ElementBase *item, ElementBase *after );

    void                  cleanup();

    virtual bool          parse_item( wxString s );

    virtual bool          read_properties( ElementBase *hi, const wxString& props );

    /// create an unknown element by name
    virtual ElementBase*  create_element( const wxString& name ) const = 0;
    /// create a known element from a hudspecs item
    virtual ElementBase*  create_element( const hsitem_s& defit ) const = 0;

    

  protected:
    elements_type         m_els;
    wxString              m_filename;
    bool                  m_modified;
    ElementBase           *m_load_prevel;

    // options that have been set in the comments of the hudfile
    // like `version' `view_aspectratio'
    wxString              m_opt_version;
    wxString              m_opt_aspectratio;

    
};


#endif // HUDFILEBASE_H

