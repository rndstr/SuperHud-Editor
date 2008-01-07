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
