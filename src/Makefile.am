bin_PROGRAMS = superhudeditor

superhudeditor_SOURCES = \
                         color4.cpp colo4.h \
                         artprovider.h \
                         common.h \
                         exceptions.h \
                         factorybase.h \
                         displayctrlbase.h \
                         misc.cpp misc.h \
                         prefs.cpp prefs.h \
                         elementslistctrl.cpp elementslistctrl.h \
                         elementsctrlbase.cpp elementsctrlbase.h \
                         hudfilebase.cpp hudfilebase.h \
                         elementbase.cpp elementbase.h \
                         cpma/factory.cpp cpma/factory.h \
                         cpma/displayctrl.cpp cpma/displayctrl.h \
                         cpma/elementsctrl.cpp cpma/elementsctrl.h \
                         cpma/hudfile.cpp cpma/hudfile.h \
                         cpma/element.cpp cpma/element.h \
                         cpma/hudspecs.cpp cpma/hudspecs.h \
                         GameSelectionDialog.cpp GameSelectionDialog.h \
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
AM_CXXFLAGS = -DPACKAGE_PREFIX="\"$(prefix)\"" $(DEB)
