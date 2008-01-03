bin_PROGRAMS = superhudeditor

superhudeditor_SOURCES = \
                         artprovider.h \
                         color4.cpp color4.h \
                         common.h \
                         displayctrlbase.cpp displayctrlbase.h \
                         elementbase.cpp elementbase.h \
                         elementsctrlbase.cpp elementsctrlbase.h \
                         elementslistctrl.cpp elementslistctrl.h \
                         eventids.h \
                         exceptions.h \
                         factorybase.h \
                         font.cpp font.h \
                         glcanvasbase.cpp glcanvasbase.h \
                         hudfilebase.cpp hudfilebase.h \
                         mainframe.cpp mainframe.h \
                         misc.cpp misc.h \
                         model.cpp model.h \
                         pakfiledialog.cpp src/pakfiledialog.h \
                         pakimageproperty.cpp pakimageproperty.h \
                         pakmanager.cpp pakmanager.h \
                         prefs.cpp prefs.h \
                         propertiesnotebookbase.h \
                         setupwizard.cpp setupwizard.h \
                         superhudeditor.cpp superhudeditor.h \
                         texture.cpp texture.h \
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
                         cpma/visibilitypropertiesctrl.cpp cpma/visibilitypropertiesctrl.h \
                         wxpropgrid-1.3/src/props.cpp wxpropgrid-1.3/include/wx/props.h \
                         wxpropgrid-1.3/src/propgrid.cpp wxpropgrid-1.3/include/wx/propgrid.h \
                         wxpropgrid-1.3/src/advprops.cpp wxpropgrid-1.3/include/wx/advprops.h \
                         wxpropgrid-1.3/src/editors.cpp wxpropgrid-1.3/include/wx/editors.h \
                         wxpropgrid-1.3/src/manager.cpp wxpropgrid-1.3/include/wx/manager.h \
                         wxpropgrid-1.3/src/odcombo.cpp wxpropgrid-1.3/include/wx/odcombo.h \
                         GameSelectionDialog.cpp GameSelectionDialog.h



if ENABLE_DEBUG
DEB = -O0 -g
else
DEB = -DNDEBUG
endif

# FIXME this should go into configure.ac:AM_PATH_WXCONFIG
# but somehow it doesn't include aui?
superhudeditor_LDFLAGS = `wx-config --libs base,core,gl,net,aui`
AM_CXXFLAGS = -Iwxpropgrid-1.3/include/ -DPACKAGE_PREFIX="\"$(prefix)\"" $(DEB)
