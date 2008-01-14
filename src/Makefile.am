bin_PROGRAMS = superhudeditor

#dist_noinst_DATA = \
#                   src/wxpropgrid-1.3/README.txt \
#                   src/wxpropgrid-1.3/CHANGES.txt


xpm = \
                         xpm/bitmaps/setupwizard.xpm \
                         xpm/icons/*.xpm \
                         xpm/icons/superhudeditor.ico

wxpropgrid = \
                         wxpropgrid-1.3/src/props.cpp wxpropgrid-1.3/include/wx/propgrid/props.h \
                         wxpropgrid-1.3/src/propgrid.cpp wxpropgrid-1.3/include/wx/propgrid/propgrid.h \
                         wxpropgrid-1.3/src/advprops.cpp wxpropgrid-1.3/include/wx/propgrid/advprops.h \
                         wxpropgrid-1.3/src/editors.cpp wxpropgrid-1.3/include/wx/propgrid/editors.h \
                         wxpropgrid-1.3/src/manager.cpp wxpropgrid-1.3/include/wx/propgrid/manager.h \
                         wxpropgrid-1.3/src/odcombo.cpp wxpropgrid-1.3/include/wx/propgrid/odcombo.h \
                         wxpropgrid-1.3/include/wx/propgrid/propdev.h
cpma = \
                         cpma/colorpropertiesctrl.cpp cpma/colorpropertiesctrl.h \
                         cpma/displayctrl.cpp cpma/displayctrl.h \
                         cpma/element.cpp cpma/element.h \
                         cpma/elementsctrl.cpp cpma/elementsctrl.h \
                         cpma/factory.cpp cpma/factory.h \
                         cpma/fontpropertiesctrl.cpp cpma/fontpropertiesctrl.h \
                         cpma/hudfile.cpp cpma/hudfile.h \
                         cpma/hudspecs.cpp cpma/hudspecs.h \
                         cpma/imagepropertiesctrl.cpp cpma/imagepropertiesctrl.h \
                         cpma/miscpropertiesctrl.cpp cpma/miscpropertiesctrl.h \
                         cpma/propertiesnotebook.cpp cpma/propertiesnotebook.h \
                         cpma/propertygridbase.h \
                         cpma/visibilitypropertiesctrl.cpp cpma/visibilitypropertiesctrl.h


superhudeditor_SOURCES = \
                         $(xpm) \
                         artprovider.h \
                         color4.cpp color4.h \
                         common.h \
                         command.cpp command.h \
                         convertdialog.cpp convertdialog.h \
                         displayctrlbase.cpp displayctrlbase.h \
                         elementbase.cpp elementbase.h \
                         elementsctrlbase.cpp elementsctrlbase.h \
                         elementslistctrl.cpp elementslistctrl.h \
                         eventids.h \
                         exceptions.h \
                         factorybase.h \
                         font.cpp font.h \
                         hudfilebase.cpp hudfilebase.h \
                         mainframe.cpp mainframe.h \
                         misc.cpp misc.h \
                         model.cpp model.h \
                         optionalmessagedialog.cpp optionalmessagedialog.h \
                         pakfiledialog.cpp pakfiledialog.h \
                         pakfileproperty.cpp pakfileproperty.h \
                         pakmanager.cpp pakmanager.h \
                         prefs.cpp prefs.h \
                         prefsdialog.cpp prefsdialog.h \
                         propertiesnotebookbase.h \
                         setupwizard.cpp setupwizard.h \
                         superhudeditor.cpp superhudeditor.h \
                         texture.cpp texture.h \
                         GameSelectionDialog.cpp GameSelectionDialog.h \
                         $(cpma) \
                         $(wxpropgrid)



if ENABLE_DEBUG
DEB =
else
DEB = -DNDEBUG
endif

AM_CXXFLAGS = -Iwxpropgrid-1.3/include/ -DPACKAGE_PREFIX="\"$(prefix)\"" $(DEB)
