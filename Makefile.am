# vim: noexpandtab
SUBDIRS = src

DOCS = AUTHORS ChangeLog COPYING INSTALL NEWS README KNOWNBUGS TODO


# Data that needs to be distributed and won't be packed up.
# Note that webupdater is not supported on non-win32 so we 
# don't ship it!
nobase_dist_pkgdata_DATA = data/pak-$(VERSION).pke data/tips.txt



# EXTRA_DIST

# those go into pak-VERSION.pke but ship it into tarball nevertheless
# this list should be equal to data/pak-VERSION.include
apppak = \
					data/cpma/hudspecs.dat \
					data/cpma/docs/README.hudspecs \
					data/cpma/docs/README.superhud \
					data/cpma/texture/background_16x10.jpg \
					data/cpma/texture/background_4x3.jpg \
					data/model/dfegg.mde \
					data/model/README \
					data/texture/default.tga \
					data/texture/model.tga

# we aren't using the webupdater on linux..
# but provide it in distributed file nevertheless
webupdater = \
										data/webupdater/local.xml \
										data/webupdater/webupdatedlg.xrc \
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

dist_noinst_DATA =  \
										data/pak-$(VERSION).include \
										$(apppak) \
										create-pak.sh \
                    superhudeditor.sln \
                    superhudeditor.vcproj \
                    superhudeditor.cbp \
										superhudeditor.nsi \
										Microsoft.VC90.CRT/Microsoft.VC90.CRT.manifest \
										Microsoft.VC90.CRT/msvcm90.dll \
										Microsoft.VC90.CRT/msvcp90.dll \
										Microsoft.VC90.CRT/msvcr90.dll \
										$(webupdater) \
										www/webupdater/remote.xml

