SUBDIRS = src

DOCS = AUTHORS ChangeLog COPYING INSTALL NEWS README

# note that this is only for devel... :x we should have everything in a pak file for retail
nobase_dist_pkgdata_DATA = data/cpma/hudspecs.dat data/cpma/texture/background.jpg \
                           data/model/dfegg.mde \
                           data/texture/*.tga \
                           data/tips.txt

# we aren't using the webupdater on linux..
dist_noinst_DATA =  \
                    superhudeditor.sln \
                    superhudeditor.vcproj \
                    superhudeditor.cbp \
                    webupdater/include/wx/checked_dis.xpm \
                    webupdater/include/wx/checkedlistctrl.h \
                    webupdater/include/wx/checked.xpm \
                    webupdater/include/wx/download.h \
                    webupdater/include/wx/installer.h \
                    webupdater/include/wx/md5.h \
                    webupdater/include/wx/stdactions.h \
                    webupdater/include/wx/unchecked_dis.xpm \
                    webupdater/include/wx/unchecked.xpm \
                    webupdater/include/wx/webupdatectrl.h \
                    webupdater/include/wx/webupdatedef.h \
                    webupdater/include/wx/webupdatedlg.h \
                    webupdater/include/wx/webupdate.h \
                    webupdater/src/app.ico \
                    webupdater/src/app.rc \
                    webupdater/src/app.xpm \
                    webupdater/src/checkedlistctrl.cpp \
                    webupdater/src/download.cpp \
                    webupdater/src/installer.cpp \
                    webupdater/src/md5.cpp \
                    webupdater/src/stdactions.cpp \
                    webupdater/src/webapp.cpp \
                    webupdater/src/webupdate.cpp \
                    webupdater/src/webupdatectrl.cpp \
                    webupdater/src/webupdatedlg.cpp \
                    webupdater/src/webupdatedlg.xrc \
                    webupdater/src/www.xpm




#DISTCHECK_CONFIGURE_FLAGS = --with-wx-config=`which wx-config-2.6` --disable-opengl
