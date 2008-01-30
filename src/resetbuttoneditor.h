#ifndef RESETBUTTONEDITOR_H 
#define RESETBUTTONEDITOR_H 

 class wxResetButtonEditor : public wxPGTextCtrlEditor
    {
        WX_PG_DECLARE_EDITOR_CLASS(wxResetButtonEditor)
    public:
        wxResetButtonEditor() {}
        virtual ~wxResetButtonEditor() {}

        wxPG_DECLARE_CREATECONTROLS
        virtual bool OnEvent( wxPropertyGrid* propGrid,
                              wxPGProperty* property,
                              wxWindow* ctrl,
                              wxEvent& event ) const;

    };

    WX_PG_IMPLEMENT_EDITOR_CLASS(ResetButtonEditor, wxResetButtonEditor,
                                 wxPGTextCtrlEditor)

    wxPGWindowList wxResetButtonEditor::CreateControls( wxPropertyGrid* propGrid,
                                                                wxPGProperty* property,
                                                                const wxPoint& pos,
                                                                const wxSize& sz ) const
    {
        // Create and populate buttons-subwindow
        wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

        // Add two regular buttons
        buttons->Add( wxT("...") );
        buttons->Add( wxT("X") );

        // Create the 'primary' editor control (textctrl in this case)
        wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls
                                 ( propGrid, property, pos, buttons->GetPrimarySize() );

        // Finally, move buttons-subwindow to correct position and make sure
        // returned wxPGWindowList contains our custom button list.
        buttons->FinalizePosition(pos);

        wndList.SetSecondary( buttons );
        return wndList;
    }

    bool wxResetButtonEditor::OnEvent( wxPropertyGrid* propGrid,
                                               wxPGProperty* property,
                                               wxWindow* ctrl,
                                               wxEvent& event ) const
    {
        if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
        {
            wxPGMultiButton* buttons = (wxPGMultiButton*) propGrid->GetEditorControlSecondary();

            /*
            if ( event.GetId() == buttons->GetButtonId(0) )
            {
                // Do something when first button is pressed
                return true;
            }
            */
            if ( event.GetId() == buttons->GetButtonId(1) )
            {
              wxLogWarning(wxT("fu"));
              propGrid->SetPropertyValue(wxT("visible"), wxT(""));
                // Do something when first button is pressed
                return true;
            }
        }
        return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
    }

#endif // MULTIBUTTONMULTICHOICEEDITOR_H

