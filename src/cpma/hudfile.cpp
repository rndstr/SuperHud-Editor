
#include "hudfile.h"
#include "../common.h"
#include "../elementsctrlbase.h"


#include "hudspecs.h"
#include "element.h"

#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/tokenzr.h>
//#include <wx/stdpaths.h>
//#include <wx/dir.h>
//#include <algorithm>


CPMAHudFile::CPMAHudFile() : 
  HudFileBase()
{
}



void CPMAHudFile::load_default_elements()
{
  clear();
  ElementBase *el = 0;
  const CPMAHudSpecs::hsitems_type& items = CPMAHudSpecs::get().items();
  for( CPMAHudSpecs::cit_hsitems cit = items.begin(); cit != items.end(); ++cit )
  {
    if( cit->flags & E_NODEFAULT )
      continue;
    el = new CPMAElement(*cit);// cit->name, cit->desc, cit->enable, cit->text, cit->icon, 0, E_RECT_DEFAULT, cit->type, cit->flags, cit->has );    
    append( el );

    //if( force_disable ) 
      //hi->set_enable( false );
  }
  m_modified = false;
}

bool CPMAHudFile::load( const wxString& filename )
{
  wxLogDebug(wxT("Loading hudfile: ") + filename);
  wxString content;
  wxFileInputStream fis( filename );

  m_load_prevel = 0;

  if( !fis.Ok() )
    return false;

  load_default_elements();
  wxTextInputStream tis( fis );
  wxString line;
  size_t pos;
  // read file line by line and remove `#' comments
  while(!fis.Eof())
  {
    line = tis.ReadLine();
    she::wxTrim(line);
    if( 0 == line.length() || line[0] == '#' )
      continue;
    if( (pos = line.find( wxT("#") )) != wxString::npos )
      line = line.substr( 0, pos );
    content += line;
    content += wxT("\n");
  }

  she::wxTrim( content ); 
  try
  {
    wxStringTokenizer tok( content, wxT("}") );
    while(tok.HasMoreTokens())
      parse_item( tok.GetNextToken() );
  }
  catch( hudfile_parse_error& err )
  {
    wxString str = wxT("ERROR while parsing `") + filename + wxT("':\n ");
    str += wxString(err.what(), wxConvUTF8);
    wxLogError( str );
    load_default_elements();
    return false;
  }
  // removeo all non-unique elements that aren't enabled
  for( it_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( ((*it)->flags() & E_NOTUNIQ) && !(*it)->is_enabled() )
      m_els.erase( it );
  }

  // remove E_DRAWNEVER and E_PARENT from end
  ElementBase *el;
  for( int i = m_els.size()-1; i >= 0; --i )
  {
    el = m_els[i];
    if( el->flags() & E_DRAWNEVER && el->flags() & E_PARENT )
    {
      m_els.erase(m_els.begin() + i);
      delete el;
    }
    else
      break;
  }
  
  //if( m_had_unknown_items )
//    wxLogWarning( wxT("You had unknown elements in your hudfile. This could be due to\nmisspellings or an outdated hudspecs file. \nTry updating your hudspecs file by visiting the website: 'Help->Visit website'") );
  m_filename = filename;
  wxGetApp().elementsctrl()->list_refresh(m_els);
  m_modified = false;
  return true;
}

bool CPMAHudFile::parse_item( wxString s )
{
  she::wxLTrim(s);

  // fish name of item.
  size_t pos = s.find_first_of( wxT(" {\n\t\r") );
  wxString name = s.substr( 0, pos );

  // extract properties of this item
  pos = s.find_first_of('{');
  wxString props = s.substr( pos + 1, s.length() - pos - 1 );
  she::wxTrim( props );
  if( props.find('{') != wxString::npos )
  {
    wxString e = wxString::Format(_("Missing closing brace in element `%s'"), name.c_str());
    throw hudfile_parse_error( e );
  }
#ifdef BACKWARD_COMPATIBILITY_142
  if( name == HF_PREDECORATE_PREFIX || name.StartsWith(HF_PREDECORATE_PREFIX_142) )
#else
  if( name == HF_PREDECORATE_PREFIX )
#endif
    name = HF_PREDECORATE_PREFIX;
#ifdef BACKWARD_COMPATIBILITY_142
  else if( name == HF_POSTDECORATE_PREFIX || name.StartsWith(HF_POSTDECORATE_PREFIX_142) )
#else
  else if( name == HF_POSTDECORATE_PREFIX )
#endif
    name = HF_POSTDECORATE_PREFIX;

  // check if item exists (it should!).
  const hsitem_s *defit = CPMAHudSpecs::get().find_item( name );
  ElementBase *exel = find_element( name ); // the existing item.
  ElementBase *el = 0; // the item we are gonna create
  if (defit == 0)
  { // warning, unknown item!
    //m_had_unknown_items = true;
    wxLogWarning( wxT("Unknown elementname `") + name + wxT("', adding it anyway."));
    el = new CPMAElement(name);
    append(el);
  }
  else // valid item-name, verry gooood...
  { 
    if( exel ) 
    { // and it already exists in hud    
      if( defit->flags & E_NOTUNIQ )
      { // as we can have several of it, check if the one in the hud is already in use.
        if( exel->is_enabled() )
        { // already in use add another one and keep flags alive.
          el = new CPMAElement(*defit);
          append(el);
        }
        else
          el = exel;
      }
      else
      { // it's an unique item. so if it's already enabled (=already been read), issue a warning
        el = exel;
        if( exel->is_enabled() )
          wxLogWarning( _("You have more than one instance of `%s'' in your hudfile\nwhich might doesn't have the effect you desire."), name.c_str() );
      }
    }
    else
    {
      el = new CPMAElement( *defit );
      append(el);
    }
  }

  wxASSERT( el != 0 );
  // all items inside the hudfile are drawn (enabled).
  el->set_enabled( true );
  // make sure the order is the same as in hudfile.
  if( el != m_load_prevel && !move_element_after( el, m_load_prevel ) )
      wxLogError( BUG_MSG + wxT("Failed moving item `") + el->name() + wxT("'."));
  m_load_prevel = el;
  
  // read properties
  return read_properties( el, props );
}


bool CPMAHudFile::read_properties( ElementBase *hi, const wxString& props )
{
  size_t pos;
  wxString prop, cmd, args;
  wxStringTokenizer linetok( props, HF_PROPERTY_DELIM );
//  wxLogMessage( wxT("[%d] '%s'"), linetok.CountTokens(), props );
  while( linetok.HasMoreTokens() )
  {
    prop = linetok.GetNextToken();
    she::wxTrim( prop );
    if (prop.empty())
      continue;

    // extract property name
    pos = prop.find_first_of( HF_PROPERTY_TRIM );
    if( pos != wxString::npos )
      cmd = prop.substr( 0, pos );
    else
      cmd = prop;
    
    if (cmd.empty())
    { // FIXME who cares?
      wxLogWarning( wxT("Found empty element command.") );
      continue;
    }
    
    // extract arguments
    args = wxT("");
    if( pos != wxString::npos )
    {
      args = prop.substr(pos+1, prop.length() - pos - 1);
      she::wxTrim( args );
      // remove commas and multi-spaces, we only care about ONE single space as delimiter.
      args.Replace( wxT(","), wxT(" ") );
      while( args.Replace( wxT("  "), wxT(" ") ) > 0 );
    }

    if( !hi->parse_property( cmd, args ) )
    {
      wxLogWarning( _("Invalid command `%s' found in element `%s'."), cmd.c_str(), hi->name().c_str() );
      //wxString e = wxString::Format( wxT("Invalid element command `%s' found in element `%s'."), cmd,  hi->get_name() );
      //throw hudfilereader_parse_error( e.c_str() );
    }
  }

  return true;
}


bool CPMAHudFile::save( const wxString& filename )
{
  int decoratecount = 0;
  wxFFileOutputStream file( filename.c_str() );
  if( !file.Ok() )
    return false;
  wxTextOutputStream stream( file );
  wxDateTime dt(wxDateTime::Now());
  
  stream << wxT("# written by ") << APP_NAME << wxT(" v") << APP_VERSION << wxT(" on ") << dt.FormatISODate() << wxT(" ") << dt.FormatISOTime() << wxT("\n");
  stream << wxT("# ") << APP_URL << wxT("\n");

  for( cit_elements it = m_els.begin(); it != m_els.end(); ++it )
  {
    if( (*it)->name() == HF_POSTDECORATE_PREFIX || (*it)->name() == HF_PREDECORATE_PREFIX )
      ++decoratecount;
    write_element( stream, *(*it) );
  }

  if( decoratecount > HF_MAX_PREPOSTDECORATE )
    wxLogWarning( _T("You have more than %d combined PreDecorate/PostDecorate elements which CPMA does not support"), HF_MAX_PREPOSTDECORATE );

  m_filename = filename;
  m_modified = false;

  return true;
}
