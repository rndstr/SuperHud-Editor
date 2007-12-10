bin_PROGRAMS = superhudeditor

superhudeditor_SOURCES = \
                         wxpropgrid/src/propgrid/advprops.cpp wxpropgrid/include/propgrid/advprops.h \
                         wxpropgrid/src/propgrid/extras.cpp wxpropgrid/src/propgrid/extras.h \
                         wxpropgrid/src/propgrid/manager.cpp wxpropgrid/src/propgrid/manager.h \
                         wxpropgrid/src/propgrid/odcombo.cpp wxpropgrid/src/propgrid/odcombo.h \
                         wxpropgrid/src/propgrid/propgrid.cpp wxpropgrid/src/propgrid/propgrid.h \
                         wxpropgrid/src/propgrid/props.cpp wxpropgrid/src/propgrid/props.h \
                         color4.cpp color4.h \
                         artprovider.h \
                         common.h \
                         exceptions.h \
                         factorybase.h \
                         displayctrlbase.h \
                         misc.cpp misc.h \
                         prefs.cpp prefs.h \
                         elementbase.cpp elementbase.h \
                         elementsctrlbase.cpp elementsctrlbase.h \
                         elementslistctrl.cpp elementslistctrl.h \
                         fontpropertiesctrl.cpp fontpropertiesctrl.h \
                         GameSelectionDialog.cpp GameSelectionDialog.h \
                         pgfontsizeproperty.cpp pgfontsizeproperty.h \
                         positionpropertiesctrl.cpp positionpropertiesctrl.h \
                         propertiesctrlbase.cpp propertiesctrlbase.h \
                         hudfilebase.cpp hudfilebase.h \
                         cpma/displayctrl.cpp cpma/displayctrl.h \
                         cpma/element.cpp cpma/element.h \
                         cpma/elementsctrl.cpp cpma/elementsctrl.h \
                         cpma/factory.cpp cpma/factory.h \
                         cpma/hudfile.cpp cpma/hudfile.h \
                         cpma/hudspecs.cpp cpma/hudspecs.h \
                         cpma/propertiesctrl.cpp cpma/propertiesctrl.h \
                         mainframe.cpp mainframe.h \
                         superhudeditor.cpp superhudeditor.h



if ENABLE_DEBUG
DEB = -O0 -g
else
DEB = -DNDEBUG
endif

# FIXME this should go into configure.ac:AM_PATH_WXCONFIG
# but somehow it doesn't include aui?
superhudeditor_LDFLAGS = `wx-config --libs`
AM_CXXFLAGS = -Iwxpropgrid/include/ -DPACKAGE_PREFIX="\"$(prefix)\"" $(DEB)
