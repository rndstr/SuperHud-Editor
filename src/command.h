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
#ifndef COMMAND_H
#define COMMAND_H

#include <wx/cmdproc.h>
#include "hudfilebase.h"

/// stores all the list elements.. actually only the pointers, so only
/// usefull for reordering
class ReorderCommand : public wxCommand
{
  public:
    ReorderCommand( const wxString& action ) : wxCommand(true, action) { takepre(); }
    virtual ~ReorderCommand() {}

    /// this is supposed to be called before any changes are done to the elements
    void takepre();
    /// call this after you changed things
    void takepost();
    virtual bool Do();
    virtual bool Undo();

  protected:
    elements_type m_doels;
    elements_type m_undoels;
};

/*
/// changed extended properties (those stored in GAMEElement::Properties)
class PropertyExtCommand : public wxCommand
{
  public:
    PropertyCommand( ElementBase *from, ElementBase *to

*/

#endif
