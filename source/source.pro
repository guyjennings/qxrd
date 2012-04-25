include("../qxrd.version.pri")
include("../qxrd.platform.pri")

message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Destdir = $${DESTDIR})

TEMPLATE = app
TARGET   = qxrd
DESTDIR  = ../
#DESTDIR  = ../app/

macx {
  ICON = images/qxrd-icon.icns
}

# POST_TARGETDEPS += install
#target.path = ../app
INSTALLS += target
CONFIG += qt #profile
QT += network \
    script \
    scripttools
macx:#CONFIG += x86
else:unix {
    message(Version = $${VERSION})
    message(Plugin path = $${QXRD_PLUGIN_PATH})
}
DEFINES += QXRD_VERSION=\"$$VERSION\"
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

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
message(Building: $${TARGET})
win32 { 
    RC_FILE = qxrd.rc
    CONFIG(debug, debug|release):CONFIG += console
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
    help/README.html \
    simpleServerTest.mac \
    testScripts.js \
    testScriptPlot.js \
    testImage.js \
    tilt_test.js \
    testscript.js \
    unsubtractDark.js
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
#include("../qceplib.pri")
include("qxrd.levmar.pri")

# include("qxrd-cuda.pri")
HEADERS += spec_server.h \
    qspecserver.h \
    qcepmacros.h \
    qcepproperty.h \
    qcepproperty-ptr.h \
    qcepsettingssaver.h \
    qcepsettingssaver-ptr.h \
    qcepmutexlocker.h \
    qcepimagedata.h \
    qcepimagedata-ptr.h \
    qcepimagedataformat.h \
    qcepimagedataformat-ptr.h \
    qcepimagedataformattiff.h \
    qcepimagedataformatmar345.h \
    qcepimagedataformatfactory.h \
    qcepimagedataformatfactory-ptr.h \
    qcepdocumentationdictionary.h \
    qcepdebug.h \
    qxrddebug.h \
    qxrdacquisitionextrainputsplot.h \
    qxrddetector.h \
    qxrddetectorpilatus.h \
    qxrddetectorsimulated.h \
    qxrddetectorthread.h \
    qxrddetector-ptr.h \
    qxrddetectorthread-ptr.h \
    qxrddetectorfilewatcher.h \
    qxrdacquisitiondialog-ptr.h \
    qxrdacquisitionextrainputsdialogsettings.h \
    qxrdhelpbrowser.h \
    qxrdobjectnamer.h

SOURCES += qspecserver.cpp \
    qcepproperty.cpp \
    qcepsettingssaver.cpp \
    qcepmutexlocker.cpp \
    qcepimagedata.cpp \
    qcepimagedataformat.cpp \
    qcepimagedataformattiff.cpp \
    qcepimagedataformatmar345.cpp \
    qcepimagedataformatfactory.cpp \
    qcepdocumentationdictionary.cpp \
    qcepdebug.cpp \
    qxrddebug.cpp \
    qxrdacquisitionextrainputsplot.cpp \
    qxrddetector.cpp \
    qxrddetectorpilatus.cpp \
    qxrddetectorsimulated.cpp \
    qxrddetectorthread.cpp \
    qxrddetectorfilewatcher.cpp \
    qxrdacquisitionextrainputsdialogsettings.cpp \
    qxrdhelpbrowser.cpp \
    qxrdobjectnamer.cpp

OTHER_FILES += testImage.js

HEADERS += TODO.h \
    qxrdapplication.h \
    qxrdapplication-ptr.h \
    qxrdglobalsettings.h \
    qxrdexperimentsettings.h \
    qxrdimagedata.h \
    qxrdimagedata-ptr.h \
    qxrdimagequeue.h \
    qxrdimagequeue-ptr.h \
    qxrdimagedataformathis.h \
    qxrdimagedataformattiff.h \
    qxrdimagedataformatmar345.h \
    qxrdrasterdata.h \
    qxrdrasterdata-ptr.h \
    qxrdmaskrasterdata.h \
    qxrdmaskcolormap.h \
    qxrdwindow.h \
    qxrdwindow-ptr.h \
    qxrddataprocessorthread.h \
    qxrddataprocessorthread-ptr.h \
    qxrddataprocessor.h \
    qxrddataprocessor-ptr.h \
    qxrddataprocessorbase.h \
    qxrddataprocessorthreaded.h \
    qxrdallocatorthread.h \
    qxrdallocator.h \
    qxrdallocator-ptr.h \
    qxrdfilesaverthread.h \
    qxrdfilesaverthread-ptr.h \
    qxrdfilesaver.h \
    qxrdfilesaver-ptr.h \
    qxrdcenterfinder.h \
    qxrdcenterfinder-ptr.h \
    qxrdcenterfinderplot.h \
    qxrdcenterfinderdialog.h \
    qxrdcenterfinderpicker.h \
    qxrdpowderpointpicker.h \
    qxrdintegrator.h \
    qxrdintegrator-ptr.h \
    qxrdintegratorplot.h \
    qxrdintegratordialog.h \
    qxrdimageplot.h \
    qxrdplotzoomer.h \
    qxrdserver.h \
    qxrdserver-ptr.h \
    qxrdserverthread.h \
    qxrdserverthread-ptr.h \
    qxrdacquisitionthread.h \
    qxrdacquisitionthread-ptr.h \
    qxrdacquisition.h \
    qxrdacquisition-ptr.h \
    qxrdacquisitiondialog.h \
    qxrdplot.h \
    qxrdplot-ptr.h \
    qxrdplotslicer.h \
    qxrdplotslicer-ptr.h \
    qxrdplotmeasurer.h \
    qxrdplotmeasurer-ptr.h \
    qxrdscriptengine.h \
    qxrdscriptengine-ptr.h \
    qxrdscriptenginethread.h \
    qxrdscriptenginethread-ptr.h \
    qxrdmaskpicker.h \
    qxrdmaskpicker-ptr.h \
    qxrdmaskdata.h \
    qxrdmaskdata-ptr.h \
    qxrdimageplotmeasurer.h \
    qxrdimageplotmeasurer-ptr.h \
    qxrdfilebrowser.h \
    qxrdimagecalculator.h \
    qxrdmutexlocker.h \
    qxrdexperimentpreferencesdialog.h \
    qxrdsharedpointer.h \
    qxrdintegrateddata.h \
    qxrdintegrateddata-ptr.h \
    qxrdintegrateddataqueue.h \
    qxrdresultserializer.h \
    qxrdroidata.h \
    qxrdroidata-ptr.h \
    qxrdhistogramdata.h \
    qxrdhistogramdata-ptr.h \
    qxrdexposuretimespinner.h \
    qxrddetectorplugininterface.h \
    qxrdnidaqplugininterface.h \
    qxrdnidaqplugininterface-ptr.h \
    qxrdprocessorinterface.h \
    qxrdnamedplugininterface.h \
    qxrddetectorgeometry.h \
    qxrddetectorgeometry-ptr.h \
    qxrdgeneratetestimage.h \
    qxrdgeneratetestimage-ptr.h \
    qxrdfitparameter.h \
    qxrdfitparameter-ptr.h \
    qxrdringfitparameters.h \
    qxrdringfitparameters-ptr.h \
    qxrdringsetfitparameters.h \
    qxrdringsetfitparameters-ptr.h \
    qxrdringsampleddata.h \
    qxrdringsampleddata-ptr.h \
    qxrdringsetsampleddata.h \
    qxrdringsetsampleddata-ptr.h \
    qxrdringsetresampler.h \
    qxrdringsetfitrefiner.h \
    qxrdringfitrefiner.h \
    qxrdringresampler.h \
    qxrdresampler.h \
    qxrdfitrefiner.h \
    qxrdsimpleserver.h \
    qxrdsimpleserver-ptr.h \
    qxrdsimpleserverthread.h \
    qxrdsimpleserverthread-ptr.h \
    qxrdpowderfitwidget.h \
    qxrdpowderfitimageplot.h \
    qxrdpowderfitidentifypage.h \
    qxrdpowderfitrefinepage.h \
    qxrdpowderfitresultspage.h \
    qxrdpowderfitdialog.h \
    qxrdcenterstepspinner.h \
    qxrdsettingssaver.h \
    qxrdsettingssaver-ptr.h \
    qxrdplotimage.h \
    qxrdplotimage-ptr.h \
    qxrdimagedisplaywidget.h \
    qwt_plot_piecewise_curve.h \
    qxrdmaskdialog.h \
    qxrdmaskdialog-ptr.h \
    qxrdmaskstackmodel.h \
    qxrdmaskstackmodel-ptr.h \
    qxrdmaskstack.h \
    qxrdmaskstack-ptr.h \
    qxrdmaskstackview.h \
    qxrdsynchronizedacquisition.h \
    qxrdsynchronizedacquisition-ptr.h \
    qxrdsynchronizedacquisitiondialog.h \
    qxrdsynchronizedacquisitiondialog-ptr.h \
    qxrddisplaydialog.h \
    qxrdcorrectiondialog.h \
    qxrdfilebrowsermodel.h \
    qxrdfilebrowsermodel-ptr.h \
    qxrdfilebrowserview.h \
    qxrdslicedialog.h \
    qxrdhistogramdialog.h \
    qxrdinfodialog.h \
    qxrdhistogramplot.h \
    qxrdsliceplot.h \
    qxrdhistogramselector.h \
    qxrdresponsetimer.h \
    qxrdimagedataobjectcounter.h \
    qxrdthread.h \
    qxrdsynchronizedacquisitionplot.h \
    qxrdhighlighter.h \
    qxrdfilebrowsermodelupdater.h \
    qxrdfilebrowsermodelupdater-ptr.h \
    qxrdfilebrowsermodelupdaterthread.h \
    qxrdfilebrowsermodelupdaterthread-ptr.h \
    qxrdintegratorcache.h \
    qxrdintegratorcache-ptr.h \
    qxrdexperimentthread.h \
    qxrdexperimentthread-ptr.h \
    qxrdexperiment.h \
    qxrdexperiment-ptr.h \
    qxrdexperimentpilatusacquisition.h \
    qxrdexperimentperkinelmeracquisition.h \
    qxrdexperimentsimulatedacquisition.h \
    qxrdexperimentperkinelmeranalysis.h \
    qxrdexperimentpilatusanalysis.h \
    qxrdexperimentgenericanalysis.h \
    qxrdwelcomewindow.h \
    qxrdwelcomewindow-ptr.h \
    qxrdglobalpreferencesdialog.h \
    qxrdwelcomerecentitem.h \
    qxrdsplashscreen.h \
    qxrdsplashscreen-ptr.h \
    qxrdacquisitiontrigger.h \
    qxrdacquisitiontrigger-ptr.h \
    qxrdacquisitiontriggerthread.h \
    qxrdacquisitiontriggerthread-ptr.h \
    qxrdacquisitiontriggerdialog.h \
    qxrdacquisitiontriggerdialog-ptr.h \
    qxrdacquisitionextrainputs.h \
    qxrdacquisitionextrainputs-ptr.h \
    qxrdacquisitionextrainputsdialog.h \
    qxrdacquisitionextrainputsdialog-ptr.h \
    qxrdacquisitionextrainputschannel.h \
    qxrdacquisitionextrainputschannel-ptr.h \
    qxrdwindowsettings.h \
    qxrdslicedialogsettings.h \
    qxrdplotsettings.h \
    qxrdimageplotsettings.h \
    qxrdfilebrowsersettings.h \
    qxrdhistogramdialogsettings.h \
    qxrdhistogramplotsettings.h \
    qxrdinfodialogsettings.h \
    qxrdcenterfinderplotsettings.h \
    qxrdsliceplotsettings.h
unix:HEADERS += AcqLinuxTypes.h
SOURCES += qxrd.cpp \
    qxrdapplication.cpp \
    qxrdglobalsettings.cpp \
    qxrdexperimentsettings.cpp \
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
    qxrdpowderpointpicker.cpp \
    qxrdintegrator.cpp \
    qxrdintegratorplot.cpp \
    qxrdintegratordialog.cpp \
    qxrdimageplot.cpp \
    qxrdplotzoomer.cpp \
    qxrdserver.cpp \
    qxrdserverthread.cpp \
    qxrdacquisitionthread.cpp \
    qxrdacquisition.cpp \
    qxrdacquisitiondialog.cpp \
    qxrdplot.cpp \
    qxrdplotslicer.cpp \
    qxrdplotmeasurer.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrddataprocessorthread.cpp \
    qxrdmaskpicker.cpp \
    qxrdmaskdata.cpp \
    qxrdimageplotmeasurer.cpp \
    qxrdfilebrowser.cpp \
    qxrdimagecalculator.cpp \
    qxrdmutexlocker.cpp \
    qxrdexperimentpreferencesdialog.cpp \
    qxrdsharedpointer.cpp \
    qxrdintegrateddata.cpp \
    qxrdintegrateddataqueue.cpp \
    qxrdresultserializer.cpp \
    qxrdroidata.cpp \
    qxrdhistogramdata.cpp \
    qxrdexposuretimespinner.cpp \
    qxrddetectorplugininterface.cpp \
    qxrdnidaqplugininterface.cpp \
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
    qxrdcenterstepspinner.cpp \
    qxrdsettingssaver.cpp \
    qxrdplotimage.cpp \
    qxrdimagedisplaywidget.cpp \
    qwt_plot_piecewise_curve.cpp \
    qxrdmaskdialog.cpp \
    qxrdmaskstackmodel.cpp \
    qxrdmaskstack.cpp \
    qxrdmaskstackview.cpp \
    qxrdsynchronizedacquisition.cpp \
    qxrdsynchronizedacquisitiondialog.cpp \
    qxrddisplaydialog.cpp \
    qxrdcorrectiondialog.cpp \
    qxrdfilebrowsermodel.cpp \
    qxrdfilebrowserview.cpp \
    qxrdslicedialog.cpp \
    qxrdhistogramdialog.cpp \
    qxrdinfodialog.cpp \
    qxrdhistogramplot.cpp \
    qxrdsliceplot.cpp \
    qxrdhistogramselector.cpp \
    qxrdresponsetimer.cpp \
    qxrdimagedataobjectcounter.cpp \
    qxrdthread.cpp \
    qxrdsynchronizedacquisitionplot.cpp \
    qxrdhighlighter.cpp \
    qxrdfilebrowsermodelupdater.cpp \
    qxrdfilebrowsermodelupdaterthread.cpp \
    qxrdintegratorcache.cpp \
    qxrdexperimentthread.cpp \
    qxrdexperiment.cpp \
    qxrdexperimentpilatusacquisition.cpp \
    qxrdexperimentperkinelmeracquisition.cpp \
    qxrdexperimentsimulatedacquisition.cpp \
    qxrdexperimentperkinelmeranalysis.cpp \
    qxrdexperimentpilatusanalysis.cpp \
    qxrdexperimentgenericanalysis.cpp \
    qxrdwelcomewindow.cpp \
    qxrdglobalpreferencesdialog.cpp \
    qxrdwelcomerecentitem.cpp \
    qxrdsplashscreen.cpp \
    qxrdacquisitiontrigger.cpp \
    qxrdacquisitiontriggerthread.cpp \
    qxrdacquisitiontriggerdialog.cpp \
    qxrdacquisitionextrainputs.cpp \
    qxrdacquisitionextrainputsdialog.cpp \
    qxrdacquisitionextrainputschannel.cpp \
    qxrdwindowsettings.cpp \
    qxrdslicedialogsettings.cpp \
    qxrdplotsettings.cpp \
    qxrdimageplotsettings.cpp \
    qxrdfilebrowsersettings.cpp \
    qxrdhistogramdialogsettings.cpp \
    qxrdhistogramplotsettings.cpp \
    qxrdinfodialogsettings.cpp \
    qxrdcenterfinderplotsettings.cpp \
    qxrdsliceplotsettings.cpp
FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui \
    qxrdimagecalculator.ui \
    qxrdexperimentpreferencesdialog.ui \
    qxrdpowderfitwidget.ui \
    qxrdmaskdialog.ui \
    qxrdsynchronizedacquisitiondialog.ui \
    qxrddisplaydialog.ui \
    qxrdcorrectiondialog.ui \
    qxrdslicedialog.ui \
    qxrdhistogramdialog.ui \
    qxrdinfodialog.ui \
    qxrdwelcomewindow.ui \
    qxrdglobalpreferencesdialog.ui \
    qxrdwelcomerecentitem.ui \
    qxrdacquisitiondialog.ui \
    qxrdacquisitiontriggerdialog.ui \
    qxrdacquisitionextrainputsdialog.ui
macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

contains(DEFINES,HAVE_PERKIN_ELMER) {
  SOURCES += qxrdacquisitionperkinelmer.cpp \
             qxrddetectorperkinelmer.cpp \
             qxrdperkinelmerplugininterface.cpp
  HEADERS += qxrdacquisitionperkinelmer.h \
             qxrddetectorperkinelmer.h \
             qxrdperkinelmerplugininterface.h \
             qxrdperkinelmerplugininterface-ptr.h
}

OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-cuda.pri \
    HeaderTemplates.txt

win32 { # Copy QT Libraries into app directory
#  PRE_TARGETDEPS += app
#  QMAKE_EXTRA_TARGETS += app
  LIBDIR = $$[QT_INSTALL_BINS]
  LIBDIR_WIN = $${replace(LIBDIR, /, \\)}

  win32-g++ {
    exists($${LIBDIR}/libgcc_s_dw2-1.dll) {
      message("MINGW found in $${LIBDIR}/libgcc_s_dw2-1.dll")
      QMAKE_EXTRA_TARGETS += libgcc
      PRE_TARGETDEPS   += ../libgcc_s_dw2-1.dll
      libgcc.target   = ../libgcc_s_dw2-1.dll
      libgcc.depends  = $${LIBDIR}/libgcc_s_dw2-1.dll
      libgcc.commands = $(COPY_FILE) $${LIBDIR_WIN}\\libgcc_s_dw2-1.dll ..\\libgcc_s_dw2-1.dll
    }

    exists($${LIBDIR}/mingwm10.dll) {
      message("MINGW found in $${LIBDIR}/mingwm10.dll")
      QMAKE_EXTRA_TARGETS += mingwm10
      PRE_TARGETDEPS   += ../mingwm10.dll
      mingwm10.target   = ../mingwm10.dll
      mingwm10.depends  = $${LIBDIR}/QtCored4.dll
      mingwm10.commands = $(COPY_FILE) $${LIBDIR_WIN}\\mingwm10.dll ..\\mingwm10.dll
    }
  }

  CONFIG(debug, debug|release) {
    QMAKE_EXTRA_TARGETS += QtCored4
    PRE_TARGETDEPS      += ../QtCored4.dll
    QtCored4.target      = ../QtCored4.dll
    QtCored4.depends     = $${LIBDIR}/QtCored4.dll
    QtCored4.commands    = $(COPY_FILE) $${LIBDIR_WIN}\\QtCored4.dll ..\\QtCored4.dll

    QMAKE_EXTRA_TARGETS += QtNetworkd4
    PRE_TARGETDEPS   += ../QtNetworkd4.dll
    QtNetworkd4.target   = ../QtNetworkd4.dll
    QtNetworkd4.depends  = $${LIBDIR}/QtNetworkd4.dll
    QtNetworkd4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtNetworkd4.dll ..\\QtNetworkd4.dll

    QMAKE_EXTRA_TARGETS += QtGuid4
    PRE_TARGETDEPS   += ../QtGuid4.dll
    QtGuid4.target   = ../QtGuid4.dll
    QtGuid4.depends  = $${LIBDIR}/QtGuid4.dll
    QtGuid4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtGuid4.dll ..\\QtGuid4.dll

    QMAKE_EXTRA_TARGETS += QtScriptd4
    PRE_TARGETDEPS   += ../QtScriptd4.dll
    QtScriptd4.target   = ../QtScriptd4.dll
    QtScriptd4.depends  = $${LIBDIR}/QtScriptd4.dll
    QtScriptd4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtScriptd4.dll ..\\QtScriptd4.dll
  } else {
    QMAKE_EXTRA_TARGETS += QtCore4
    PRE_TARGETDEPS   += ../QtCore4.dll
    QtCore4.target   = ../QtCore4.dll
    QtCore4.depends  = $${LIBDIR}/QtCore4.dll
    QtCore4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtCore4.dll ..\\QtCore4.dll

    QMAKE_EXTRA_TARGETS += QtNetwork4
    PRE_TARGETDEPS   += ../QtNetwork4.dll
    QtNetwork4.target   = ../QtNetwork4.dll
    QtNetwork4.depends  = $${LIBDIR}/QtNetwork4.dll
    QtNetwork4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtNetwork4.dll ..\\QtNetwork4.dll

    QMAKE_EXTRA_TARGETS += QtGui4
    PRE_TARGETDEPS   += ../QtGui4.dll
    QtGui4.target   = ../QtGui4.dll
    QtGui4.depends  = $${LIBDIR}/QtGui4.dll
    QtGui4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtGui4.dll ..\\QtGui4.dll

    QMAKE_EXTRA_TARGETS += QtScript4
    PRE_TARGETDEPS   += ../QtScript4.dll
    QtScript4.target   = ../QtScript4.dll
    QtScript4.depends  = $${LIBDIR}/QtScript4.dll
    QtScript4.commands = $(COPY_FILE) $${LIBDIR_WIN}\\QtScript4.dll ..\\QtScript4.dll
  }

  QMAKE_DISTCLEAN += /Q ..\\*.dll
  QMAKE_DISTCLEAN += ..\\plugins\\*
  QMAKE_DISTCLEAN += ..\\plugins
}

win32 { # Make NSIS installer...
#  CONFIG(release, debug|release) {
    OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
    PWD_WIN = $${replace(PWD, /, \\)}

    exists("c:/Program Files/NSIS/makensis.exe") {
      QMAKE_POST_LINK = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
      message("NSIS found in Program Files")
    }

    exists("c:/Program Files (x86)/NSIS/makensis.exe") {
      QMAKE_POST_LINK = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
      message("NSIS found in Program Files (x86)")
    }

    !isEmpty(QMAKE_POST_LINK) {
      contains(QMAKE_HOST.arch,x86_64) {
        QMAKE_POST_LINK += /DWIN64
      }

      QMAKE_POST_LINK += /DVERSION=$${VERSION}
      CONFIG(release, debug|release) {
        QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}\"
        QMAKE_POST_LINK += /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
      } else {
        QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}-dbg\"
        QMAKE_POST_LINK += /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
      }
      QMAKE_POST_LINK += /DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"

      QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd.nsi\"
    }
#  }
}

#for(m, QT) {
#  message("In source.pro QT contains $${m}")
#}

































