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
    qml \
    script \
    scripttools \
    testlib

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
    qxrd_macrocounter.mac \
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
    tiffconfig/macx/tiffconf.h \
    help/qxrdspecmacro.html \
    qxrd_tests.mac

CONFIG += qceplib-qwt qceplib-hdf5

#CONFIG += qceplib-nexus

include(submodules/qceplib/qceplib-qwt.pri)
include(submodules/qceplib/qceplib-base.pri)
include(submodules/qceplib/qceplib-mar345.pri)
include(submodules/qceplib/qceplib-cbf.pri)
include(submodules/qceplib/qceplib-tiff.pri)
include(submodules/qceplib/qceplib-levmar.pri)
include(submodules/qceplib/qceplib-szip.pri)
include(submodules/qceplib/qceplib-zlib.pri)
include(submodules/qceplib/qceplib-hdf5.pri)
#include(submodules/qceplib/qceplib-nexus.pri)

macx {
}

include(submodules/qceplib/qceplib-specserver.pri)

HEADERS += TODO.h \
    qxrdapplication.h \
    qxrdapplication-ptr.h \
    qxrdglobalsettings.h \
    qxrdexperimentsettings.h \
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
    qxrdserver.h \
    qxrdserver-ptr.h \
    qxrdserverthread.h \
    qxrdserverthread-ptr.h \
    qxrdacquisition.h \
    qxrdacquisition-ptr.h \
    qxrdacquisitiondialog.h \
    qxrdplotslicer.h \
    qxrdplotslicer-ptr.h \
    qxrdscriptengine.h \
    qxrdscriptengine-ptr.h \
    qxrdscriptenginethread.h \
    qxrdscriptenginethread-ptr.h \
    qxrdmaskpicker.h \
    qxrdmaskpicker-ptr.h \
    qxrdimageplotmeasurer.h \
    qxrdimageplotmeasurer-ptr.h \
    qxrdfilebrowser.h \
    qxrdimagecalculator.h \
    qxrdexperimentpreferencesdialog.h \
    qxrdsharedpointer.h \
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
    qxrdimagedisplaywidget.h \
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
    qxrdthread.h \
    qxrdsynchronizedacquisitionplot.h \
    qxrdhighlighter.h \
    qxrdfilebrowsermodelupdater.h \
    qxrdfilebrowsermodelupdater-ptr.h \
    qxrdfilebrowsermodelupdaterthread.h \
    qxrdfilebrowsermodelupdaterthread-ptr.h \
    qxrdintegratorcache.h \
    qxrdintegratorcache-ptr.h \
    qxrdexperiment.h \
    qxrdexperiment-ptr.h \
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
    qxrdimageplotsettings.h \
    qxrdfilebrowsersettings.h \
    qxrdhistogramdialogsettings.h \
    qxrdhistogramplotsettings.h \
    qxrdinfodialogsettings.h \
    qxrdcenterfinderplotsettings.h \
    qxrdsliceplotsettings.h \
    triangulate.h \
    qxrddebug.h \
    qxrdacquisitionextrainputsplot.h \
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
    qxrdcalibrant-ptr.h \
    qxrdcalibrantlibrary.h \
    qxrdcalibrantlibrary-ptr.h \
    qxrdcalibrantdialog.h \
    qxrdcalibrantlibrarymodel.h \
    qxrdcalibrantlibrarymodel-ptr.h \
    qxrdcalibrantdspacingsmodel.h \
    qxrdcalibrantdspacingsmodel-ptr.h \
    qxrdimageplotzoomer.h \
    qxrdmainwindow.h \
    qxrdacquisitionparameterpack.h \
    qxrdacquisitionparameterpack-ptr.h \
    qxrddarkacquisitionparameterpack.h \
    qxrddarkacquisitionparameterpack-ptr.h \
    qxrdprocessargs.h \
    qxrdprocessargs-ptr.h \
    qxrddetectorproxy.h \
    qxrddetectorproxy-ptr.h \
    qxrddetectorproxywidget.h \
    qxrddetectorproxylistmodel.h \
    qxrddetectorproxylistmodel-ptr.h \
    qxrddetectorconfigurationdialog.h \
    qxrddetectorcontrolwindow.h \
    qxrddetectorcontrolwindow-ptr.h \
    qxrddetectorprocessor.h \
    qxrddetectorprocessor-ptr.h \
    qxrdroicalculator.h \
    qxrdroicalculator-ptr.h \
    qxrdacquisitioninterface.h \
    qxrdacquisitioninterface-ptr.h \
    qxrdacquisitionexecutionthread.h \
    qxrdacquisitionexecutionthread-ptr.h \
    qxrdacquisitionexecution.h \
    qxrdacquisitionexecution-ptr.h \
    qxrdtodolist.h \
    qxrdtodolistitem.h \
    qxrdtodolistitem-ptr.h \
    qxrdroicoordinates.h \
    qxrdroicoordinates-ptr.h \
    qxrddetectorpilatusremote.h \
    qxrddetectorpilatusremote-ptr.h \
    qxrdroicoordinateslistmodel.h \
    qxrdroicoordinateslistmodel-ptr.h \
    qxrdimageplotsettings-ptr.h \
    qxrddetectorimageplot.h \
    qxrdroitypedelegate.h \
    qxrdacquisitionscalerdialog.h \
    qxrdacquisitionscalermodel.h \
    qxrdacquisitionscalermodel-ptr.h \
    qxrdacquisitionscalerdialog-ptr.h \
    qxrdzingerdialog.h \
    qxrdpolartransform.h \
    qxrdpolartransformdialog.h \
    qxrdpolartransform-ptr.h \
    qxrdpolarintensityfitter.h \
    qxrdpolarnormalization.h \
    qxrdpolarnormalizationdialog.h \
    qxrdpolarnormalization-ptr.h \
    qxrdcalibrantpropertiesdialog.h \
    qxrdcalibrantdspacings.h \
    qxrdcalibrantdspacing.h \
    qxrdcalibrantdspacing-ptr.h \
    qxrdcalibrantdspacings-ptr.h \
    qxrdintegratorparmsdialog.h \
    qxrdapplicationsettings.h \
    qxrdapplicationsettings-ptr.h \
    qxrdintegratorplotsettings.h \
    qxrdintegratorplotsettings-ptr.h \
    qxrddistortioncorrectionplotsettings-ptr.h \
    qxrddistortioncorrectiondialogsettings-ptr.h \
    qxrdacquisitionextrainputsdialogsettings-ptr.h \
    qxrdacquisitionextrainputsplotsettings.h \
    qxrdacquisitionextrainputsplotsettings-ptr.h \
    qxrdhistogramplotsettings-ptr.h \
    qxrdhistogramdialogsettings-ptr.h \
    qxrdcenterfinderplotsettings-ptr.h \
    qxrdfilebrowsersettings-ptr.h \
    qxrdinfodialogsettings-ptr.h \
    qxrdscriptdialogsettings-ptr.h \
    qxrdsynchronizedacquisitiondialogsettings-ptr.h \
    qxrdsynchronizedacquisitionplotsettings-ptr.h \
    qxrdslicedialogsettings-ptr.h \
    qxrdsliceplotsettings-ptr.h \
    qxrdwindowsettings-ptr.h \
    qxrdroicoordinateslist.h \
    qxrdroicoordinateslist-ptr.h \
    qxrddetectorsettingsfilewatcher.h \
    qxrddetectorsettingssimulated.h \
    qxrddetectorsettingspilatus.h \
    qxrddetectorsettingsperkinelmer.h \
    qxrddetectorsettings.h \
    qxrddetectorsettings-ptr.h \
    qxrddetectordriver.h \
    qxrddetectordriversimulated.h \
    qxrddetectordriverperkinelmer.h \
    qxrddetectordriverpilatus.h \
    qxrddetectordriverfilewatcher.h \
    qxrddetectordriverepicsarea.h \
    qxrddetectorsettingsepicsarea.h \
    qxrddetectordriver-ptr.h \
    qxrddetectordriverthread.h \
    qxrddetectordriverthread-ptr.h \
    qxrddetectorsettingspilatus-ptr.h \
    qxrddetectorsettingsperkinelmer-ptr.h \
    qxrddetectorsettingssimulated-ptr.h \
    qxrddetectorsettingsfilewatcher-ptr.h \
    qxrddetectorsettingsepicsarea-ptr.h \
    qxrdexperimentthread.h \
    qxrdexperimentthread-ptr.h \
    qxrdroishape.h \
    qxrdroirectangle.h \
    qxrdroiellipse.h \
    qxrdroipolygon.h \
    qxrdroishape-ptr.h \
    qxrdroirasterdata.h \
    qxrdroipicker.h \
    qxrdroipicker-ptr.h \
    qxrddetectorimageplot-ptr.h \
    qxrdroicenteredshape.h \
    qxrdroicache.h \
    qxrdroicache-ptr.h \
    qxrddetectorcontrolwindowsettings.h \
    qxrddetectorcontrolwindowsettings-ptr.h \
    qxrdexperimentsettings-ptr.h \
    qxrdjsengine.h \
    qxrdjsengine-ptr.h \
    qxrdroieditordialog.h \
    qxrdpolygonpointsmodel.h \
    qxrdroipolygon-ptr.h \
    qxrdroicenteredshape-ptr.h \
    modeltest.h \
    qxrdfittedrings.h \
    qxrdfittedrings-ptr.h

SOURCES += qxrd.cpp \
    qxrdapplication.cpp \
    qxrdglobalsettings.cpp \
    qxrdexperimentsettings.cpp \
    qxrdimagedataformathis.cpp \
    qxrdimagedataformattiff.cpp \
    qxrdimagedataformatmar345.cpp \
    qxrdrasterdata.cpp \
    qxrdmaskrasterdata.cpp \
    qxrdmaskcolormap.cpp \
    qxrdwindow.cpp \
    qxrddataprocessor.cpp \
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
    qxrdserver.cpp \
    qxrdserverthread.cpp \
    qxrdacquisition.cpp \
    qxrdacquisitiondialog.cpp \
    qxrdplotslicer.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrddataprocessorthread.cpp \
    qxrdmaskpicker.cpp \
    qxrdimageplotmeasurer.cpp \
    qxrdfilebrowser.cpp \
    qxrdimagecalculator.cpp \
    qxrdmutexlocker.cpp \
    qxrdexperimentpreferencesdialog.cpp \
    qxrdsharedpointer.cpp \
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
    qxrdimagedisplaywidget.cpp \
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
    qxrdthread.cpp \
    qxrdsynchronizedacquisitionplot.cpp \
    qxrdhighlighter.cpp \
    qxrdfilebrowsermodelupdater.cpp \
    qxrdfilebrowsermodelupdaterthread.cpp \
    qxrdintegratorcache.cpp \
    qxrdexperiment.cpp \
    qxrdwelcomewindow.cpp \
    qxrdglobalpreferencesdialog.cpp \
    qxrdwelcomerecentitem.cpp \
    qxrdsplashscreen.cpp \
    qxrdacquisitionextrainputs.cpp \
    qxrdacquisitionextrainputsdialog.cpp \
    qxrdacquisitionextrainputschannel.cpp \
    qxrdwindowsettings.cpp \
    qxrdslicedialogsettings.cpp \
    qxrdimageplotsettings.cpp \
    qxrdfilebrowsersettings.cpp \
    qxrdhistogramdialogsettings.cpp \
    qxrdhistogramplotsettings.cpp \
    qxrdinfodialogsettings.cpp \
    qxrdcenterfinderplotsettings.cpp \
    qxrdsliceplotsettings.cpp \
    triangulate.c \
    qxrddebug.cpp \
    qxrdacquisitionextrainputsplot.cpp \
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
    qxrdimageplotzoomer.cpp \
    qxrdmainwindow.cpp \
    qxrdacquisitionparameterpack.cpp \
    qxrddarkacquisitionparameterpack.cpp \
    qxrdprocessargs.cpp \
    qxrddetectorproxy.cpp \
    qxrddetectorproxywidget.cpp \
    qxrddetectorproxylistmodel.cpp \
    qxrddetectorconfigurationdialog.cpp \
    qxrddetectorcontrolwindow.cpp \
    qxrddetectorprocessor.cpp \
    qxrdroicalculator.cpp \
    qxrdacquisitioninterface.cpp \
    qxrdacquisitionexecutionthread.cpp \
    qxrdacquisitionexecution.cpp \
    qxrdtodolist.cpp \
    qxrdtodolistitem.cpp \
    qxrdroicoordinates.cpp \
    qxrddetectorpilatusremote.cpp \
    qxrdroicoordinateslistmodel.cpp \
    qxrddetectorimageplot.cpp \
    qxrdroitypedelegate.cpp \
    qxrdacquisitionscalerdialog.cpp \
    qxrdacquisitionscalermodel.cpp \
    qxrdzingerdialog.cpp \
    qxrdpolartransform.cpp \
    qxrdpolartransformdialog.cpp \
    qxrdpolarintensityfitter.cpp \
    qxrdpolarnormalization.cpp \
    qxrdpolarnormalizationdialog.cpp \
    qxrdcalibrantpropertiesdialog.cpp \
    qxrdcalibrantdspacings.cpp \
    qxrdcalibrantdspacing.cpp \
    qxrdintegratorparmsdialog.cpp \
    qxrdapplicationsettings.cpp \
    qxrdintegratorplotsettings.cpp \
    qxrdacquisitionextrainputsplotsettings.cpp \
    qxrdroicoordinateslist.cpp \
    qxrddetectorsettings.cpp \
    qxrddetectorsettingspilatus.cpp \
    qxrddetectorsettingsperkinelmer.cpp \
    qxrddetectorsettingssimulated.cpp \
    qxrddetectorsettingsfilewatcher.cpp \
    qxrddetectordriver.cpp \
    qxrddetectordriversimulated.cpp \
    qxrddetectordriverperkinelmer.cpp \
    qxrddetectordriverpilatus.cpp \
    qxrddetectordriverfilewatcher.cpp \
    qxrddetectordriverepicsarea.cpp \
    qxrddetectorsettingsepicsarea.cpp \
    qxrddetectordriverthread.cpp \
    qxrdexperimentthread.cpp \
    qxrdroishape.cpp \
    qxrdroirectangle.cpp \
    qxrdroiellipse.cpp \
    qxrdroipolygon.cpp \
    qxrdroirasterdata.cpp \
    qxrdroipicker.cpp \
    qxrdroicenteredshape.cpp \
    qxrdroicache.cpp \
    qxrddetectorcontrolwindowsettings.cpp \
    qxrdjsengine.cpp \
    qxrdroieditordialog.cpp \
    qxrdpolygonpointsmodel.cpp \
    modeltest.cpp \
    qxrdfittedrings.cpp

OTHER_FILES += testImage.js

FORMS += qxrdwindow.ui \
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
    qxrddetectorproxywidget.ui \
    qxrddetectorconfigurationdialog.ui \
    qxrddetectorcontrolwindow.ui \
    qxrdacquisitionscalerdialog.ui \
    qxrdzingerdialog.ui \
    qxrdpolartransformdialog.ui \
    qxrdpolarnormalizationdialog.ui \
    qxrdcalibrantpropertiesdialog.ui \
    qxrdintegratorparmsdialog.ui \
    qxrdroieditordialog.ui

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

contains(DEFINES,HAVE_PERKIN_ELMER) {
  SOURCES += \
             qxrdperkinelmerplugininterface.cpp
  HEADERS += \
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
      greaterThan(QT_MINOR_VERSION, 6) {
      } else {
        greaterThan(QT_MINOR_VERSION, 4) {
          libs += icudt54 icuin54 icuuc54
        } else {
          libs += icudt53 icuin53 icuuc53
        }
      }
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





























