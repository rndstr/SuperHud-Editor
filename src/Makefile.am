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
                         cpma_colorpropertiesctrl.cpp cpma_colorpropertiesctrl.h \
                         cpma_displayctrl.cpp cpma_displayctrl.h \
                         cpma_element.cpp cpma_element.h \
                         cpma_elementsctrl.h \
                         cpma_factory.cpp cpma_factory.h \
                         cpma_fontpropertiesctrl.cpp cpma_fontpropertiesctrl.h \
                         cpma_hudfile.cpp cpma_hudfile.h \
                         cpma_imagepropertiesctrl.cpp cpma_imagepropertiesctrl.h \
                         cpma_miscpropertiesctrl.cpp cpma_miscpropertiesctrl.h \
                         cpma_propertiesnotebook.cpp cpma_propertiesnotebook.h \
                         cpma_propertygridbase.h \
                         cpma_visibilitypropertiesctrl.cpp cpma_visibilitypropertiesctrl.h

q4max = \
				q4max_displayctrl.cpp q4max_displayctrl.h \
				q4max_element.cpp q4max_element.h \
				q4max_elementsctrl.h \
				q4max_factory.cpp q4max_factory.h \
				q4max_hudfile.cpp q4max_hudfile.h \
				q4max_propertiesnotebook.cpp q4max_propertiesnotebook.h \
				q4max_propertygrid.h



superhudeditor_SOURCES = \
                         $(xpm) \
                         artprovider.h \
                         color4.cpp color4.h \
                         common.h \
                         command.cpp command.h \
                         convertdialog.cpp convertdialog.h \
                         displayctrlbase.cpp displayctrlbase.h \
                         downloadevent.cpp downloadevent.h \
                         downloadtext.cpp downloadtext.h \
                         elementbase.cpp elementbase.h \
                         elementsctrlbase.cpp elementsctrlbase.h \
                         elementslistctrl.cpp elementslistctrl.h \
                         eventids.h \
                         exceptions.h \
                         factorybase.h \
                         font.cpp font.h \
                         hudfilebase.cpp hudfilebase.h \
                         hudspecs.cpp hudspecs.h \
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
                         scrolledmessagedialog.cpp scrolledmessagedialog.h \
                         superhudeditor.cpp superhudeditor.h \
                         texture.cpp texture.h \
                         GameSelectionDialog.cpp GameSelectionDialog.h \
                         $(cpma) \
                         $(q4max) \
                         $(wxpropgrid)



if ENABLE_DEBUG
DEB =
else
DEB = -DNDEBUG
endif

AM_CXXFLAGS = -Iwxpropgrid-1.3/include/ -DPACKAGE_PREFIX="\"$(prefix)\"" $(DEB)
