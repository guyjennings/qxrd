# message(Qt version: $$[QT_VERSION])
# message(Qt is installed in $$[QT_INSTALL_PREFIX])
# message(Qt resources can be found in the following locations:)
# message(Documentation: $$[QT_INSTALL_DOCS])
# message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])

# message(Plugins: $$[QT_INSTALL_PLUGINS])
# message(Data files: $$[QT_INSTALL_DATA])
# message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
# message(Settings: $$[QT_INSTALL_SETTINGS])
# message(Examples: $$[QT_INSTALL_EXAMPLES])
# message(Demonstrations: $$[QT_INSTALL_DEMOS])
message(Destdir = $${DESTDIR})
TEMPLATE = app

DESTDIR = ../app/

# POST_TARGETDEPS += install
target.path = ../app
INSTALLS += target
CONFIG += qt
include("../qxrd.version.pri")
include("../qxrd.platform.pri")
QT += network \
    script \
    scripttools
macx:CONFIG += x86 \
    ppc
macx:
else:unix { 
    message(Version = $${VERSION})
    message(Plugin path = $${QXRD_PLUGIN_PATH})
}
DEFINES += QXRD_VERSION=\"\\\"$$VERSION\\\"\"
!isEmpty(QXRD_PLUGIN_PATH):DEFINES += QXRD_PLUGIN_PATH=\"$$QXRD_PLUGIN_PATH\"
*g++* { 
    QMAKE_CXXFLAGS += -g
    QMAKE_CFLAGS += -g
    QMAKE_LFLAGS += -g
}
vectorize { 
    QMAKE_CXXFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
    QMAKE_CFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
}
profile { 
    QMAKE_CXXFLAGS += -pg
    QMAKE_CFLAGS += -pg
    QMAKE_LFLAGS += -pg
}
CONFIG(debug, debug|release):QXRDSUFFIX = $${QXRDSUFFIX}-dbg
else:QXRDSUFFIX = $${QXRDSUFFIX}-rls

# win32 {
# TARGET_EXT = -$${VERSION}$${QXRDSUFFIX}.exe
# } else {
# TARGET_EXT = -$${VERSION}$${QXRDSUFFIX}
# }
# INCLUDEPATH += /usr/local/lib/spec.d/include/
# DESTDIR = app/app$${QXRDSUFFIX}
# DESTDIR = .
MOC_DIR = moc # /moc$${QXRDSUFFIX}
UI_DIR = ui # /ui$${QXRDSUFFIX}
OBJECTS_DIR = obj # /obj$${QXRDSUFFIX}
RCC_DIR = rcc # /rcc$${QXRDSUFFIX}
TARGET = qxrd
message(Building: $${TARGET})
RC_FILE = qxrd.rc
win32 { 
    contains(QMAKE_HOST.arch,x86_64) { 
        CONFIG(debug, debug|release):CONFIG += console
        else:
    }
    else { 
        CONFIG(debug, debug|release):CONFIG += console
        else:
    }
}
RESOURCES += qxrdresources.qrc
#RESOURCES.depends += help/*.html
OTHER_FILES += help/qxrdhelptext.html \
    help/qxrdhelpscript.html \
    help/qxrdhelpscriptfunctions.html \
    help/qxrdhelpscriptobjects.html \
    help/qxrdhelpquick.html \
    help/qxrdhelpindex.html \
    help/qxrdreleasenotes.html \
    help/qxrdpowderfitidentifyhelp.html \
    help/qxrdpowderfitrefinehelp.html \
    help/qxrdpowderfitresultshelp.html
DISTFILES += qxrd.dox \
    download/index.php \
    images/*.* \
    help/*.html \
    help/images/*.* \
    tiffconfig/win32/tif_config.h \
    tiffconfig/win32/tiffconf.h \
    tiffconfig/macx/tif_config.h \
    tiffconfig/macx/tiffconf.h
win32:include("../qt-libtiff-win32.pri")
macx:include("../qt-libtiff-macx.pri")
include("../qwt-5.2.pri")
include("../qceplib.pri")
include("qxrd.levmar.pri")

# include("qxrd-cuda.pri")
HEADERS += TODO.h \
    qxrdapplication.h \
    qxrdsettings.h \
    qxrdimagedata.h \
    qxrdimagequeue.h \
    qxrdimagedataformathis.h \
    qxrdimagedataformattiff.h \
    qxrdimagedataformatmar345.h \
    qxrdrasterdata.h \
    qxrdmaskrasterdata.h \
    qxrdmaskcolormap.h \
    qxrdwindow.h \
    qxrddataprocessorthread.h \
    qxrddataprocessor.h \
    qxrddataprocessorbase.h \
    qxrddataprocessorsimple.h \
    qxrddataprocessorthreaded.h \
    qxrddataprocessorcuda.h \
    qxrdallocatorthread.h \
    qxrdallocator.h \
    qxrdfilesaverthread.h \
    qxrdfilesaver.h \
    qxrdcenterfinder.h \
    qxrdcenterfinderplot.h \
    qxrdcenterfinderdialog.h \
    qxrdcenterfinderpicker.h \
    qxrdintegrator.h \
    qxrdintegratorplot.h \
    qxrdintegratordialog.h \
    qxrdimageplot.h \
    qxrdplotzoomer.h \
    qxrdplottracker.h \
    qxrdserver.h \
    qxrdserverthread.h \
    qxrdacquisitionthread.h \
    qxrdacquisition.h \
    qxrdacquisitionparameters.h \
    qxrdacquisitionscripting.h \
    qxrdacquisitionoperations.h \
    qxrdacquisitionpilatus.h \
    qxrdacquisitionareadetector.h \
    qxrdplot.h \
    qxrdplotslicer.h \
    qxrdplotmeasurer.h \
    qxrdscriptengine.h \
    qxrdscriptenginethread.h \
    qxrdacquisitionsimulated.h \
    qxrdmaskpicker.h \
    qxrdmaskdata.h \
    qxrdimageplotmeasurer.h \
    qxrdfilebrowser.h \
    qxrdimagecalculator.h \
    qxrdmutexlocker.h \
    qxrdpreferencesdialog.h \
    qxrdforwardtypes.h \
    qxrdsharedpointer.h \
    qxrdintegrateddata.h \
    qxrdintegrateddataqueue.h \
    qxrdresultserializer.h \
    qxrdroidata.h \
    qxrdhistogramdata.h \
    qxrddoubleimagedata.h \
    qxrdexposuretimespinner.h \
    qxrddetectorplugininterface.h \
    qxrdprocessorinterface.h \
    qxrdnamedplugininterface.h \
    qxrddetectorgeometry.h \
    qxrdgeneratetestimage.h \
    qxrdfitparameter.h \
    qxrdringfitparameters.h \
    qxrdringsetfitparameters.h \
    qxrdringsampleddata.h \
    qxrdringsetsampleddata.h \
    qxrdringsetresampler.h \
    qxrdringsetfitrefiner.h \
    qxrdringfitrefiner.h \
    qxrdringresampler.h \
    qxrdresampler.h \
    qxrdfitrefiner.h \
    qxrdsimpleserver.h \
    qxrdsimpleserverthread.h \
    qxrdpowderfitwidget.h \
    qxrdpowderfitimageplot.h \
    qxrdpowderfitidentifypage.h \
    qxrdpowderfitrefinepage.h \
    qxrdpowderfitresultspage.h \
    qxrdpowderfitdialog.h \
    qxrdacquiredialog.h \
    qxrdcenterstepspinner.h
unix:HEADERS += AcqLinuxTypes.h
SOURCES += qxrd.cpp \
    qxrdapplication.cpp \
    qxrdsettings.cpp \
    qxrdimagedata.cpp \
    qxrdimagequeue.cpp \
    qxrdimagedataformathis.cpp \
    qxrdimagedataformattiff.cpp \
    qxrdimagedataformatmar345.cpp \
    qxrdrasterdata.cpp \
    qxrdmaskrasterdata.cpp \
    qxrdmaskcolormap.cpp \
    qxrdwindow.cpp \
    qxrddataprocessor.cpp \
    qxrddataprocessorbase.cpp \
    qxrddataprocessorsimple.cpp \
    qxrddataprocessorthreaded.cpp \
    qxrddataprocessorcuda.cpp \
    qxrdallocatorthread.cpp \
    qxrdallocator.cpp \
    qxrdfilesaverthread.cpp \
    qxrdfilesaver.cpp \
    qxrdcenterfinder.cpp \
    qxrdcenterfinderplot.cpp \
    qxrdcenterfinderdialog.cpp \
    qxrdcenterfinderpicker.cpp \
    qxrdintegrator.cpp \
    qxrdintegratorplot.cpp \
    qxrdintegratordialog.cpp \
    qxrdimageplot.cpp \
    qxrdplotzoomer.cpp \
    qxrdplottracker.cpp \
    qxrdserver.cpp \
    qxrdserverthread.cpp \
    qxrdacquisitionthread.cpp \
    qxrdacquisition.cpp \
    qxrdacquisitionparameters.cpp \
    qxrdacquisitionscripting.cpp \
    qxrdacquisitionoperations.cpp \
    qxrdacquisitionpilatus.cpp \
    qxrdacquisitionareadetector.cpp \
    qxrdplot.cpp \
    qxrdplotslicer.cpp \
    qxrdplotmeasurer.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrddataprocessorthread.cpp \
    qxrdacquisitionsimulated.cpp \
    qxrdmaskpicker.cpp \
    qxrdmaskdata.cpp \
    qxrdimageplotmeasurer.cpp \
    qxrdfilebrowser.cpp \
    qxrdimagecalculator.cpp \
    qxrdmutexlocker.cpp \
    qxrdpreferencesdialog.cpp \
    qxrdsharedpointer.cpp \
    qxrdintegrateddata.cpp \
    qxrdintegrateddataqueue.cpp \
    qxrdresultserializer.cpp \
    qxrdroidata.cpp \
    qxrdhistogramdata.cpp \
    qxrddoubleimagedata.cpp \
    qxrdexposuretimespinner.cpp \
    qxrddetectorplugininterface.cpp \
    qxrdprocessorinterface.cpp \
    qxrdnamedplugininterface.cpp \
    qxrddetectorgeometry.cpp \
    qxrdgeneratetestimage.cpp \
    qxrdfitparameter.cpp \
    qxrdringfitparameters.cpp \
    qxrdringsetfitparameters.cpp \
    qxrdringsampleddata.cpp \
    qxrdringsetsampleddata.cpp \
    qxrdringsetresampler.cpp \
    qxrdringsetfitrefiner.cpp \
    qxrdringfitrefiner.cpp \
    qxrdringresampler.cpp \
    qxrdresampler.cpp \
    qxrdfitrefiner.cpp \
    qxrdsimpleserver.cpp \
    qxrdsimpleserverthread.cpp \
    qxrdpowderfitwidget.cpp \
    qxrdpowderfitimageplot.cpp \
    qxrdpowderfitidentifypage.cpp \
    qxrdpowderfitrefinepage.cpp \
    qxrdpowderfitresultspage.cpp \
    qxrdpowderfitdialog.cpp \
    qxrdacquiredialog.cpp \
    qxrdcenterstepspinner.cpp
FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui \
    qxrdimagecalculator.ui \
    qxrdpreferencesdialog.ui \
    qxrdpowderfitwidget.ui \
    qxrdacquiredialog.ui
macx:
else:unix:LIBS += -ltiff
else:win32 { 
    contains(QMAKE_HOST.arch,x86_64) {
      WIN64 = 1
      PE_SDK="c:/XIS/SDK64/"
    } else {
      WIN64 = 0
      PE_SDK="c:/XIS/SDK32/"
    }
    DEFINES += HAVE_PERKIN_ELMER

    INCLUDEPATH += $${PE_SDK}

    SOURCES += \
      qxrdacquisitionperkinelmer.cpp \
      qxrdperkinelmerplugininterface.cpp

    HEADERS += \
      qxrdacquisitionperkinelmer.h \
      qxrdperkinelmerplugininterface.h
}

# QMAKE_LFLAGS += -Wl,--script,nordata.lscript
# QMAKE_LFLAGS += -Wl,--disable-auto-import
# QMAKE_CFLAGS += -g
# QMAKE_CXXFLAGS += -g
# LIBS += XISL.lib
OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-cuda.pri \
    HeaderTemplates.txt
