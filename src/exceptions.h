#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <wx/string.h>

class she_exception : public std::runtime_error
{
  public:
    she_exception( const wxString& what ) : std::runtime_error( std::string(what.mb_str()) ) {}
};

class file_not_found_error : public she_exception
{
  public:
    file_not_found_error( const wxString& what ) : she_exception(what) {}
};

class hudfile_parse_error : public she_exception
{
  public:
    hudfile_parse_error( const wxString& what ) : she_exception(what) {}
};


#endif // EXCEPTIONS_H

