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

