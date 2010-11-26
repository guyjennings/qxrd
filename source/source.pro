include("../qxrd.version.pri")
include("../qxrd.platform.pri")

message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Destdir = $${DESTDIR})

TEMPLATE = app
TARGET   = qxrd
DESTDIR  = ../app/

# POST_TARGETDEPS += install
target.path = ../app
INSTALLS += target
CONFIG += qt
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

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
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

# RESOURCES.depends += help/*.html
OTHER_FILES += help/qxrdhelptext.html \
    help/qxrdhelpscript.html \
    help/qxrdhelpscriptfunctions.html \
    help/qxrdhelpscriptobjects.html \
    help/qxrdhelpquick.html \
    help/qxrdhelpindex.html \
    help/qxrdreleasenotes.html \
    help/qxrdpowderfitidentifyhelp.html \
    help/qxrdpowderfitrefinehelp.html \
    help/qxrdpowderfitresultshelp.html \
    simpleServerTest.mac
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
    qxrddataprocessorthreaded.h \
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
    qxrdcenterstepspinner.h \
    qxrdsettingssaver.h \
    qxrdsettingssaverthread.h \
    qxrdplotimage.h \
    qxrdimagedisplaywidget.h \
    qxrddataprocessoroptionsdialog.h \
    qwt_plot_piecewise_curve.h
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
    qxrddataprocessorthreaded.cpp \
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
    qxrdcenterstepspinner.cpp \
    qxrdsettingssaver.cpp \
    qxrdsettingssaverthread.cpp \
    qxrdplotimage.cpp \
    qxrdimagedisplaywidget.cpp \
    qxrddataprocessoroptionsdialog.cpp \
    qwt_plot_piecewise_curve.cpp
FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui \
    qxrdimagecalculator.ui \
    qxrdpreferencesdialog.ui \
    qxrdpowderfitwidget.ui \
    qxrdacquiredialog.ui \
    qxrddataprocessoroptionsdialog.ui
macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

defined(HAVE_PERKIN_ELMER) {
    SOURCES += qxrdacquisitionperkinelmer.cpp \
               qxrdperkinelmerplugininterface.cpp
    HEADERS += qxrdacquisitionperkinelmer.h \
               qxrdperkinelmerplugininterface.h
}

OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-cuda.pri \
    HeaderTemplates.txt

win32 { # Copy QT Libraries into app directory
#  PRE_TARGETDEPS += app
#  QMAKE_EXTRA_TARGETS += app
  LIBDIR = $$[QT_INSTALL_BINS]

  win32-g++ {
    exists($${LIBDIR}/libgcc_s_dw2-1.dll) {
      message("MINGW found in $${LIBDIR}/libgcc_s_dw2-1.dll")
      QMAKE_EXTRA_TARGETS += libgcc
      PRE_TARGETDEPS   += ../app/libgcc_s_dw2-1.dll
      libgcc.target   = ../app/libgcc_s_dw2-1.dll
      libgcc.depends  = $${LIBDIR}/libgcc_s_dw2-1.dll
      libgcc.commands = $(COPY_FILE) $${LIBDIR}\\libgcc_s_dw2-1.dll ..\\app\\libgcc_s_dw2-1.dll
    }

    exists($${LIBDIR}/mingwm10.dll) {
      message("MINGW found in $${LIBDIR}/mingwm10.dll")
      QMAKE_EXTRA_TARGETS += mingwm10
      PRE_TARGETDEPS   += ../app/mingwm10.dll
      mingwm10.target   = ../app/mingwm10.dll
      mingwm10.depends  = $${LIBDIR}/QtCored4.dll
      mingwm10.commands = $(COPY_FILE) $${LIBDIR}\\mingwm10.dll ..\\app\\mingwm10.dll
    }
  }

  CONFIG(debug, debug|release) {
    QMAKE_EXTRA_TARGETS += QtCored4
    PRE_TARGETDEPS   += ../app/QtCored4.dll
    QtCored4.target   = ../app/QtCored4.dll
    QtCored4.depends  = $${LIBDIR}/QtCored4.dll
    QtCored4.commands = $(COPY_FILE) $${LIBDIR}\\QtCored4.dll ..\\app\\QtCored4.dll

    QMAKE_EXTRA_TARGETS += QtNetworkd4
    PRE_TARGETDEPS   += ../app/QtNetworkd4.dll
    QtNetworkd4.target   = ../app/QtNetworkd4.dll
    QtNetworkd4.depends  = $${LIBDIR}/QtNetworkd4.dll
    QtNetworkd4.commands = $(COPY_FILE) $${LIBDIR}\\QtNetworkd4.dll ..\\app\\QtNetworkd4.dll

    QMAKE_EXTRA_TARGETS += QtGuid4
    PRE_TARGETDEPS   += ../app/QtGuid4.dll
    QtGuid4.target   = ../app/QtGuid4.dll
    QtGuid4.depends  = $${LIBDIR}/QtGuid4.dll
    QtGuid4.commands = $(COPY_FILE) $${LIBDIR}\\QtGuid4.dll ..\\app\\QtGuid4.dll

    QMAKE_EXTRA_TARGETS += QtScriptd4
    PRE_TARGETDEPS   += ../app/QtScriptd4.dll
    QtScriptd4.target   = ../app/QtScriptd4.dll
    QtScriptd4.depends  = $${LIBDIR}/QtScriptd4.dll
    QtScriptd4.commands = $(COPY_FILE) $${LIBDIR}\\QtScriptd4.dll ..\\app\\QtScriptd4.dll
  } else {
    QMAKE_EXTRA_TARGETS += QtCore4
    PRE_TARGETDEPS   += ../app/QtCore4.dll
    QtCore4.target   = ../app/QtCore4.dll
    QtCore4.depends  = $${LIBDIR}/QtCore4.dll
    QtCore4.commands = $(COPY_FILE) $${LIBDIR}\\QtCore4.dll ..\\app\\QtCore4.dll

    QMAKE_EXTRA_TARGETS += QtNetwork4
    PRE_TARGETDEPS   += ../app/QtNetwork4.dll
    QtNetwork4.target   = ../app/QtNetwork4.dll
    QtNetwork4.depends  = $${LIBDIR}/QtNetwork4.dll
    QtNetwork4.commands = $(COPY_FILE) $${LIBDIR}\\QtNetwork4.dll ..\\app\\QtNetwork4.dll

    QMAKE_EXTRA_TARGETS += QtGui4
    PRE_TARGETDEPS   += ../app/QtGui4.dll
    QtGui4.target   = ../app/QtGui4.dll
    QtGui4.depends  = $${LIBDIR}/QtGui4.dll
    QtGui4.commands = $(COPY_FILE) $${LIBDIR}\\QtGui4.dll ..\\app\\QtGui4.dll

    QMAKE_EXTRA_TARGETS += QtScript4
    PRE_TARGETDEPS   += ../app/QtScript4.dll
    QtScript4.target   = ../app/QtScript4.dll
    QtScript4.depends  = $${LIBDIR}/QtScript4.dll
    QtScript4.commands = $(COPY_FILE) $${LIBDIR}\\QtScript4.dll ..\\app\\QtScript4.dll
  }
}

win32 { # Make NSIS installer...
  CONFIG(release, debug|release) {
    OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
    PWD_WIN = $${replace(PWD, /, \\)}

    QMAKE_POST_LINK = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4

    contains(QMAKE_HOST.arch,x86_64) {
      QMAKE_POST_LINK += /DWIN64
    }

    QMAKE_POST_LINK += /DVERSION=$${VERSION}
    QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}\"
    QMAKE_POST_LINK += /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
    QMAKE_POST_LINK += /DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
    QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd.nsi\"
  }
}
