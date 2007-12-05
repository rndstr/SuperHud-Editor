#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>
#include <string>
#include <wx/string.h>

class she_exception : public std::runtime_error
{
  public:
    she_exception( const wxString& what ) : std::runtime_error( std::string(what.mb_str()) ) {}
};

class file_not_found : public she_exception
{
  public:
    file_not_found( const wxString& what ) : she_exception(what) {}
};

#endif // __EXCEPTIONS_H__

