#include "wx/wx.h"

// -----------------------------------------------------------------------


#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>

// You will *NOT* need to include this in your application.
#include <wx/propgrid/extras.h>

#include "pgfontsizeproperty.h"


bool operator == (const FontSize&, const FontSize&)
{
    return false;
}



// -----------------------------------------------------------------------
// PGFontSizeProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(FontSize,PGFontSizeProperty,FontSize())

class PGFontSizePropertyClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    PGFontSizePropertyClass( const wxString& label, const wxString& name, const FontSize& value );
    virtual ~PGFontSizePropertyClass ();

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

protected:

    // I stands for internal
    void SetValueI( const FontSize& value )
    {
        m_value = value;
        RefreshChildren();
    }

    FontSize                  m_value;
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(PGFontSizeProperty,wxBaseParentProperty,
                               FontSize,const FontSize&,TextCtrl)

PGFontSizePropertyClass::PGFontSizePropertyClass ( const wxString& label, const wxString& name,
    const FontSize& value) : wxPGPropertyWithChildren(label,name)
{
    wxPG_INIT_REQUIRED_TYPE(FontSize)
    SetValueI(value);
    static const wxChar* size_type[] = {wxT("Pointsize"), wxT("Pixelsize"),  (const wxChar*)0};
    AddChild( wxEnumProperty(_("Type"), wxPG_LABEL, size_type, 0, value.type) );
    AddChild( wxIntProperty(_("Pointsize"),wxPG_LABEL,value.pt) );
    AddChild( wxIntProperty(_("Pixel width"),wxPG_LABEL,value.x) );
    AddChild( wxIntProperty(_("Pixel height"),wxPG_LABEL,value.y) );
}

PGFontSizePropertyClass::~PGFontSizePropertyClass() { }

void PGFontSizePropertyClass::DoSetValue( wxPGVariant value )
{
    SetValueI( *((FontSize*)wxPGVariantToVoidPtr(value)) );
}

wxPGVariant PGFontSizePropertyClass::DoGetValue() const
{
    return wxPGVariantCreator((void*)&m_value);
}

void PGFontSizePropertyClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    Item(0)->DoSetValue( (long)m_value.type );
    Item(1)->DoSetValue( (long)m_value.pt );
    Item(2)->DoSetValue( (long)m_value.x );
    Item(3)->DoSetValue( (long)m_value.y );
}

void PGFontSizePropertyClass::ChildChanged ( wxPGProperty* p )
{
    switch ( p->GetIndexInParent() )
    {
        case 0: m_value.type = p->DoGetValue().GetLong(); break;
        case 1: m_value.pt = p->DoGetValue().GetLong(); break;
        case 2: m_value.x = p->DoGetValue().GetLong(); break;
        case 3: m_value.y = p->DoGetValue().GetLong(); break;
    }
}

