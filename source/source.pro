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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

CONFIG += qt

QT += network \
    script \
    scripttools

unix {
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

INCLUDEPATH += qceplib

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
    racetest.js \
    racetest.mac \
    unsubtractDark.js \
    qxrdexampleusergeometry.js \
    qxrdexampleuserabsorption.js \
    help/qxrdhelpuserfunctions.html \
    calgrid.js \
    findring.js \
    dumpRings.js
DISTFILES += qxrd.dox \
    download/index.php \
    images/*.* \
    help/*.html \
    help/images/*.* \
    tiffconfig/win32/tif_config.h \
    tiffconfig/win32/tiffconf.h \
    tiffconfig/macx/tif_config.h \
    tiffconfig/macx/tiffconf.h

include(submodules/qceplib/qceplib-base.pri)
include(submodules/qceplib/qceplib-qwt.pri)
include(submodules/qceplib/qceplib-mar345.pri)
include(submodules/qceplib/qceplib-cbf.pri)
include(submodules/qceplib/qceplib-tiff.pri)
include(submodules/qceplib/qceplib-levmar.pri)

macx {
include(submodules/qceplib/qceplib-szip.pri)
include(submodules/qceplib/qceplib-zlib.pri)
include(submodules/qceplib/qceplib-hdf5.pri)
}

include(submodules/qceplib/qceplib-specserver.pri)

HEADERS += \
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
    qxrduserscriptdialog.h \
    qxrdsynchronizedacquisitiondialogsettings.h \
    qxrdsynchronizedacquisitionplotsettings.h \
    qxrdimagedataformatcbf.h \
    qxrddistortioncorrection.h \
    qxrddistortioncorrection-ptr.h \
    qxrddistortioncorrectiondialog.h \
    qxrddistortioncorrectiondialogsettings.h \
    qxrddistortioncorrectionplot.h \
    qxrddistortioncorrectionplotsettings.h \
    qxrdplotvectorfieldcurve.h \
    qxrdpoint4d.h \
    qxrdplotvectorfielddata.h \
    qxrdscriptdialog.h \
    qxrdscriptdialogsettings.h \
    qxrdscripteditwidget.h \
    qxrdpowderpoint.h \
    qxrdpowderpointproperty.h \
    qxrdfitter.h \
    qxrdfitterpeakpoint.h \
    qxrdfitterringpoint.h \
    qxrdfitterpeakorring.h \
    qxrdfitterringcircle.h \
    qxrdfitterringellipse.h \
    triangulate.h \
    qxrdplanefitter.h \
    qxrdcalibrant.h \
    qxrdcalibrantlibrary.h \
    qxrdcalibrant-ptr.h \
    qxrdcalibrantlibrary-ptr.h \
    qxrdcalibrantdialog.h \
    qxrdcalibrantlibrarymodel.h \
    qxrdcalibrantdspacingsmodel.h \
    qxrdcalibrantdspacingsmodel-ptr.h \
    qxrdcalibrantlibrarymodel-ptr.h \
    qcepdatasetbrowserdialog.h \
    qxrddataset.h \
    qxrddataset-ptr.h \
    qxrddataobjectgraphwindow.h \
    qxrddataobjectspreadsheetwindow.h \
    qxrddataobjectpropertieswindow.h \
    qxrdimagedataspreadsheetmodel.h \
    qxrdintegrateddataspreadsheetmodel.h \
    qcepdataarrayspreadsheetmodel.h \
    qcepdatacolumnspreadsheetmodel.h \
    qcepdatacolumnscanspreadsheetmodel.h \
    qcepdatagroupspreadsheetmodel.h

SOURCES += \
    qxrddebug.cpp \
    qxrdacquisitionextrainputsplot.cpp \
    qxrddetector.cpp \
    qxrddetectorpilatus.cpp \
    qxrddetectorsimulated.cpp \
    qxrddetectorthread.cpp \
    qxrddetectorfilewatcher.cpp \
    qxrdacquisitionextrainputsdialogsettings.cpp \
    qxrdhelpbrowser.cpp \
    qxrduserscriptdialog.cpp \
    qxrdsynchronizedacquisitiondialogsettings.cpp \
    qxrdsynchronizedacquisitionplotsettings.cpp \
    qxrdimagedataformatcbf.cpp \
    qxrddistortioncorrection.cpp \
    qxrddistortioncorrectiondialog.cpp \
    qxrddistortioncorrectiondialogsettings.cpp \
    qxrddistortioncorrectionplot.cpp \
    qxrddistortioncorrectionplotsettings.cpp \
    qxrdplotvectorfieldcurve.cpp \
    qxrdpoint4d.cpp \
    qxrdplotvectorfielddata.cpp \
    qxrdscriptdialog.cpp \
    qxrdscriptdialogsettings.cpp \
    qxrdscripteditwidget.cpp \
    qxrdpowderpoint.cpp \
    qxrdpowderpointproperty.cpp \
    qxrdfitter.cpp \
    qxrdfitterpeakpoint.cpp \
    qxrdfitterringpoint.cpp \
    qxrdfitterpeakorring.cpp \
    qxrdfitterringcircle.cpp \
    qxrdfitterringellipse.cpp \
    qxrdplanefitter.cpp \
    qxrdcalibrant.cpp \
    qxrdcalibrantlibrary.cpp \
    qxrdcalibrantdialog.cpp \
    qxrdcalibrantlibrarymodel.cpp \
    qxrdcalibrantdspacingsmodel.cpp \
    qcepdatasetbrowserdialog.cpp \
    qxrddataset.cpp \
    qxrddataobjectgraphwindow.cpp \
    qxrddataobjectspreadsheetwindow.cpp \
    qxrddataobjectpropertieswindow.cpp \
    qxrdimagedataspreadsheetmodel.cpp \
    qxrdintegrateddataspreadsheetmodel.cpp \
    qcepdataarrayspreadsheetmodel.cpp \
    qcepdatacolumnspreadsheetmodel.cpp \
    qcepdatacolumnscanspreadsheetmodel.cpp \
    qcepdatagroupspreadsheetmodel.cpp

OTHER_FILES += testImage.js

HEADERS += TODO.h \
    qxrdapplication.h \
    qxrdapplication-ptr.h \
    qxrdglobalsettings.h \
    qxrdexperimentsettings.h \
    qxrdimagedata.h \
    qxrdimagedata-ptr.h \
    qxrdimagequeue.h \
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
    qxrdsimpleserver.h \
    qxrdsimpleserver-ptr.h \
    qxrdsimpleserverthread.h \
    qxrdsimpleserverthread-ptr.h \
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
    qxrdsliceplotsettings.h \
    triangulate.h

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
    qxrdsimpleserver.cpp \
    qxrdsimpleserverthread.cpp \
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
    qxrdsliceplotsettings.cpp \
    triangulate.c

FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui \
    qxrdimagecalculator.ui \
    qxrdexperimentpreferencesdialog.ui \
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
    qxrdacquisitionextrainputsdialog.ui \
    qxrduserscriptdialog.ui \
    qxrddistortioncorrectiondialog.ui \
    qxrdscriptdialog.ui \
    qxrdcalibrantdialog.ui \
    qcepdatasetbrowserdialog.ui \
    qxrddataobjectgraphwindow.ui \
    qxrddataobjectspreadsheetwindow.ui \
    qxrddataobjectpropertieswindow.ui

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

contains(DEFINES,HAVE_PERKIN_ELMER) {
  SOURCES += qxrddetectorperkinelmer.cpp \
             qxrdperkinelmerplugininterface.cpp
  HEADERS += qxrddetectorperkinelmer.h \
             qxrdperkinelmerplugininterface.h \
             qxrdperkinelmerplugininterface-ptr.h
}

OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-qt5.nsi \
    qxrd-cuda.pri \
    HeaderTemplates.txt

win32 {
# Copy QT Libraries into app directory
  LIBDIR = $$[QT_INSTALL_BINS]
  LIBDIR_WIN = $${replace(LIBDIR, /, \\)}
  LIBDIR_MINGW = $${replace(LIBDIR, \\\\, /)}

  message(Libdir: $${LIBDIR})
  message(Libdir_win: $${LIBDIR_WIN})
  message(Libdir_mingw: $${LIBDIR_MINGW})
  message($${replace(LIBDIR, \\\\, /)})

  QMAKE_EXTRA_TARGETS += qtlibs

  isEqual(QT_MAJOR_VERSION, 5) {
    CONFIG(debug, debug|release) {
      libs =  Qt5Cored \
              Qt5Networkd \
              Qt5Guid \
              Qt5Scriptd \
              Qt5Widgetsd \
              Qt5Svgd \
              Qt5OpenGLd \
              Qt5PrintSupportd \
              Qt5Concurrentd

      platform = qwindowsd

      win32-msvc* {
        libs += libEGLd libGLESv2d
      }
    } else {
      libs =  Qt5Core \
              Qt5Network \
              Qt5Gui \
              Qt5Script \
              Qt5Widgets \
              Qt5Svg \
              Qt5OpenGL \
              Qt5PrintSupport \
              Qt5Concurrent

      platform = qwindows

      win32-msvc* {
        libs += libEGL libGLESv2
      }
    }

    win32-msvc2013 {
      libs += icudt53 icuin53 icuuc53
    } else:win32-msvc* {
      libs += icudt51 icuin51 icuuc51
    } else:win32-g++ {
      libs += libgcc_s_dw2-1 libwinpthread-1 libstdc++-6 icudt52 icuin52 icuuc52
    }

    QMAKE_EXTRA_TARGETS += qtplatformdir
    qtplatformdir.target = ../platforms

    win32-msvc* {
      qtplatformdir.commands = if not exist ..\\platforms $(MKDIR) ..\\platforms
    } else:win32-g++ {
      qtplatformdir.commands = $(MKDIR) -p ../platforms
    }

    QMAKE_EXTRA_TARGETS += qtplatform
    qtplatform.target   = ../platforms/$${platform}.dll
    qtplatform.depends  = qtplatformdir $${LIBDIR}/../plugins/platforms/$${platform}.dll
    win32-g++ {
      qtplatform.commands += $(COPY_FILE) $${LIBDIR_MINGW}/../plugins/platforms/$${platform}.dll ../platforms/$${platform}.dll
    } else {
      qtplatform.commands += $(COPY_FILE) /Y $${LIBDIR_WIN}\\..\\plugins\\platforms\\$${platform}.dll ..\\platforms\\$${platform}.dll
    }

    qtlibs.depends += qtplatform

    for(lib, libs) {
      !build_pass:message(Target $${lib})
      qtlibs.depends     += $${LIBDIR}/$${lib}.dll
      win32-g++ {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_MINGW}/$${lib}.dll ../$${lib}.dll &
      } else {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll ..\\$${lib}.dll &
      }
    }

    QMAKE_CLEAN += ../platforms/*
    QMAKE_CLEAN += ../platforms
  }

  isEqual(QT_MAJOR_VERSION, 4) {
    CONFIG(debug, debug|release) {
      libs = QtCored4 QtNetworkd4 QtGuid4 QtScriptd4 QtOpenGLd4 QtSvgd4
    } else {
      libs = QtCore4 QtNetwork4 QtGui4 QtScript4 QtOpenGL4 QtSvg4
    }

    win32-g++ {
      libs += libgcc_s_dw2-1 libwinpthread-1 libstdc++-6
    }

    for(lib, libs) {
      !build_pass:message(Target $${lib})

      qtlibs.depends     += $${LIBDIR}/$${lib}.dll
      win32-g++ {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_MINGW}/$${lib}.dll ../$${lib}.dll &
      } else {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll ..\\$${lib}.dll &
      }
    }
  }

  QMAKE_CLEAN += ../*.dll
  QMAKE_CLEAN += ../plugins/*
  QMAKE_CLEAN += ../plugins

  PRE_TARGETDEPS  += qtlibs
}

TARGET.depends += qtlibs

never {
# win32 { # Make NSIS installer...
    OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
    PWD_WIN = $${replace(PWD, /, \\)}

    exists("c:/Program Files/NSIS/makensis.exe") {
      win32-g++ {
        QMAKE_POST_LINK = "\"c:/Program Files/NSIS/makensis.exe\"" //V4
      } else {
        QMAKE_POST_LINK = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
      }
      message("NSIS found in Program Files")
    }

    exists("c:/Program Files (x86)/NSIS/makensis.exe") {
      win32-g++ {
        QMAKE_POST_LINK = "\"c:/Program Files (x86)/NSIS/makensis.exe\"" //V4
      } else {
        QMAKE_POST_LINK = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
      }
      message("NSIS found in Program Files (x86)")
    }

    !isEmpty(QMAKE_POST_LINK) {
      contains(QMAKE_HOST.arch,x86_64) {
        win32-g++ {
          QMAKE_POST_LINK += //DWIN64
        } else {
          QMAKE_POST_LINK += /DWIN64
        }
      }

      message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

      win32-g++ {
        QMAKE_POST_LINK += //DVERSION=$${VERSION} //DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
      } else {
        QMAKE_POST_LINK += /DVERSION=$${VERSION} /DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
      }

      CONFIG(release, debug|release) {
        win32-g++ {
          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
        } else {
          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
        }
      } else {
        win32-g++ {
          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}-dbg\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
        } else {
          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
        }
      }

      isEqual(QT_MAJOR_VERSION, 4) {
        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd.nsi\"
      } else {
        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd-qt5.nsi\"
      }
   }
}





























