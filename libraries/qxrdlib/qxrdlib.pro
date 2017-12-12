#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T10:50:00
#
#-------------------------------------------------

include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

QT       += widgets qml network script scripttools concurrent
QT       += testlib

win32:CONFIG   += skip_target_version_ext

win32:CONFIG(debug, debug|release) {
  TARGET = qxrdlibd
} else {
  TARGET = qxrdlib
}

TEMPLATE = lib
DESTDIR = ../../

qtHaveModule(datavisualization):qtHaveModule(charts) {
  message("QtDataVisualization and QtCharts available")

    QT += datavisualization charts
    DEFINES += HAVE_DATAVIS
}

packagesExist(QtNetwork) {
  message("QtNetwork exists")
}

message("QT_MAJOR_VERSION == $${QT_MAJOR_VERSION}")
message("QT_MINOR_VERSION == $${QT_MINOR_VERSION}")

contains(DEFINES, HAVE_DATAVIS) {
  message("HAVE_DATAVIS defined")
}

DEFINES += QXRDLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


DEFINES += QXRD_VERSION=\"$$VERSION\"

!isEmpty(QXRD_PLUGIN_PATH):DEFINES += QXRD_PLUGIN_PATH=\"$$QXRD_PLUGIN_PATH\"

SOURCES += qxrdlib.cpp \
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
    qxrdplotslicer.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrddataprocessorthread.cpp \
    qxrdmaskpicker.cpp \
    qxrdimageplotmeasurer.cpp \
    qxrdfilebrowser.cpp \
    qxrdimagecalculator.cpp \
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
    qxrdcalibrantlibrarymodel.cpp \
    qxrdcalibrantdspacingsmodel.cpp \
    qxrdimageplotzoomer.cpp \
    qxrdmainwindow.cpp \
    qxrdacquisitionparameterpack.cpp \
    qxrddarkacquisitionparameterpack.cpp \
    qxrdprocessargs.cpp \
    qxrddetectorproxy.cpp \
    qxrddetectorproxywidget.cpp \
    qxrddetectorconfigurationdialog.cpp \
    qxrddetectorcontrolwindow.cpp \
    qxrddetectorprocessor.cpp \
    qxrdroicalculator.cpp \
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
    qxrddetectorsettingsperkinelmer.cpp \
    qxrddetectordriver.cpp \
    qxrddetectordriversimulated.cpp \
    qxrddetectordriverperkinelmer.cpp \
    qxrddetectordriverpilatus.cpp \
    qxrddetectordriverfilewatcher.cpp \
    qxrddetectordriverepicsarea.cpp \
    qxrddetectordriverdexela.cpp \
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
    qxrdfittedrings.cpp \
    qxrdtestimagegenerator.cpp \
    qxrdtestimagegeneratorwindow.cpp \
    qxrdtestscangeneratorwindow.cpp \
    qxrdtestimagegeneratorthread.cpp \
    qxrdtestscangenerator.cpp \
    qxrdtestscangeneratorthread.cpp \
    qxrdtestscanwindowqwt.cpp \
    qxrdtestscanwindow.cpp \
    qxrdtestscanwindowdatavis.cpp \
    qxrdtestimagewindow.cpp \
    qxrdtestimagewindowdatavis.cpp \
    qxrdtestimagewindowqwt.cpp \
    qxrdtestgenerator.cpp \
    qxrdtestimageplotqwt.cpp \
    qxrdtestimageplotdatavis.cpp \
    qxrdtestscanplotdatavis.cpp \
    qxrdtestscanplotqwt.cpp \
    qxrdtestscanplotdatavishelper.cpp \
    qxrdtestscanplotqwthelper.cpp \
    qxrdtestimageplotdatavishelper.cpp \
    qxrdtestimageplotqwthelper.cpp \
    qxrdtestthread.cpp \
    qxrdplotcurvevector.cpp \
    commands/00generic/qxrdcommand.cpp \
    commands/00generic/qxrdapplicationcommand.cpp \
    commands/00generic/qxrdexperimentcommand.cpp \
    commands/00generic/qxrdgraphcommand.cpp \
    commands/00generic/qxrddatasetcommand.cpp \
    commands/specremote/qxrdspecremotecommand.cpp \
    qxrddetectorinterface.cpp \
    qxrdanalysiswindow.cpp \
    qxrdcenteringwindow.cpp \
    qxrdintegrationwindow.cpp \
    qxrdmaskingwindow.cpp \
    qxrdscriptingwindow.cpp \
    qxrdanalysiswindowsettings.cpp \
    qxrdacquisitionwindowsettings.cpp \
    qxrdacquisitionwindow.cpp \
    qxrdcalibrantwindow.cpp \
    qxrdcalibrantwindowsettings.cpp \
    qxrdcenteringwindowsettings.cpp \
    qxrdhelpwindow.cpp \
    qxrdhelpwindowsettings.cpp \
    qxrdcalculatorwindow.cpp \
    qxrdcalculatorwindowsettings.cpp \
    qxrdinfowindowsettings.cpp \
    qxrdinfowindow.cpp \
    qxrdintegrationwindowsettings.cpp \
    qxrdmaskingwindowsettings.cpp \
    qxrdscriptingwindowsettings.cpp \
    qxrdmainwindowsettings.cpp \
    qxrdextraiowindowsettings.cpp \
    qxrdextraiowindow.cpp \
    qxrdcenteringplotwidget.cpp \
    qxrdimageplotwidget.cpp \
    qxrdintegratedplotwidget.cpp \
    qxrdfilebrowserwidget.cpp \
    qxrdplotwidget.cpp \
    qxrdplotcommand.cpp \
    qxrdzoomincommand.cpp \
    qxrdzoomoutcommand.cpp \
    qxrdzoomallcommand.cpp \
    qxrdmaskcirclescommand.cpp \
    qxrdmaskpolygonscommand.cpp \
    qxrdsetcentercommand.cpp \
    qxrdpowderpointscommand.cpp \
    qxrdslicecommand.cpp \
    qxrdmeasurecommand.cpp \
    qxrdhistogramcommand.cpp \
    qxrdfitcirclecommand.cpp \
    qxrdfitellipsecommand.cpp \
    qxrdfitellipsescommand.cpp \
    qxrdadjustenergycommand.cpp \
    qxrdadjustdistancecommand.cpp \
    qxrdadjustfitcommand.cpp \
    qxrdaddpointcommand.cpp \
    qxrddelpointcommand.cpp \
    qxrddelringcommand.cpp \
    qxrddelallpointscommand.cpp \
    qxrddisableringcommand.cpp \
    qxrdenableringcommand.cpp \
    qxrdnormalizeringscommand.cpp \
    qxrdfitpeakcommand.cpp \
    qxrdfitringpointcommand.cpp \
    qxrdtraceringcommand.cpp \
    qxrdmissingringcommand.cpp \
    qxrdzappixelcommand.cpp \
    qxrdplotbuttoncommand.cpp \
    qxrdplotcontextmenucommand.cpp \
    qxrdcontextseparatorcommand.cpp \
    qxrdautoscalecommand.cpp \
    qxrdprintplotcommand.cpp \
    qxrddisplaysubmenucommand.cpp \
    qxrdplotpreferencescommand.cpp \
    qxrdprocessor.cpp \
    qxrddistortionplotwidget.cpp \
    qxrdextrainputsplotwidget.cpp \
    qxrdextraoutputsplotwidget.cpp \
    qxrdhistogramplotwidget.cpp \
    qxrdsliceplotwidget.cpp \
    qxrddetectorplotwidget.cpp \
    qxrdplugininfomodel.cpp \
    qxrddexelasettings.cpp \
    qxrddetectorlistmodel.cpp \
    qxrdfilewatchersettings.cpp \
    qxrdpilatussettings.cpp \
    qxrdsimulatedsettings.cpp \
    qxrddetectordialog.cpp \
    qxrdsimulateddialog.cpp \
    qxrdperkinelmerdialog.cpp \
    qxrdpilatusdialog.cpp \
    qxrdareadetectordialog.cpp \
    qxrdfilewatcherdialog.cpp \
    qxrddexeladialog.cpp \
    qxrdareadetectorsettings.cpp

HEADERS += qxrdlib.h\
        qxrdlib_global.h \
    commands/00generic/qxrdcommand.h \
    commands/00generic/qxrdapplicationcommand.h \
    commands/00generic/qxrdexperimentcommand.h \
    commands/00generic/qxrdgraphcommand.h \
    commands/00generic/qxrddatasetcommand.h \
    commands/specremote/qxrdspecremotecommand.h \
    qxrddetectorinterface.h \
    qxrddetectorinterface-ptr.h \
    qxrddetectorplugininterface-ptr.h \
    qxrdanalysiswindow.h \
    qxrdcenteringwindow.h \
    qxrdintegrationwindow.h \
    qxrdmaskingwindow.h \
    qxrdscriptingwindow.h \
    qxrdanalysiswindowsettings.h \
    qxrdacquisitionwindowsettings.h \
    qxrdacquisitionwindow.h \
    qxrdcalibrantwindow.h \
    qxrdcalibrantwindowsettings.h \
    qxrdcenteringwindowsettings.h \
    qxrdhelpwindow.h \
    qxrdhelpwindowsettings.h \
    qxrdcalculatorwindow.h \
    qxrdcalculatorwindowsettings.h \
    qxrdinfowindowsettings.h \
    qxrdinfowindow.h \
    qxrdintegrationwindowsettings.h \
    qxrdmaskingwindowsettings.h \
    qxrdscriptingwindowsettings.h \
    qxrdmainwindowsettings.h \
    qxrdmainwindowsettings-ptr.h \
    qxrdmainwindow-ptr.h \
    qxrdextraiowindowsettings.h \
    qxrdextraiowindow.h \
    qxrdcenteringplotwidget.h \
    qxrdimageplotwidget.h \
    qxrdintegratedplotwidget.h \
    qxrdfilebrowserwidget.h \
    qxrdacquisitionwindowsettings-ptr.h \
    qxrdanalysiswindowsettings-ptr.h \
    qxrdcalculatorwindowsettings-ptr.h \
    qxrdcalibrantwindowsettings-ptr.h \
    qxrdcenteringwindowsettings-ptr.h \
    qxrdextraiowindowsettings-ptr.h \
    qxrdhelpwindowsettings-ptr.h \
    qxrdintegrationwindowsettings-ptr.h \
    qxrdmaskingwindowsettings-ptr.h \
    qxrdscriptingwindowsettings-ptr.h \
    qxrdplotwidget.h \
    qxrdplotcommand.h \
    qxrdzoomincommand.h \
    qxrdzoomoutcommand.h \
    qxrdzoomallcommand.h \
    qxrdmaskcirclescommand.h \
    qxrdmaskpolygonscommand.h \
    qxrdsetcentercommand.h \
    qxrdpowderpointscommand.h \
    qxrdslicecommand.h \
    qxrdmeasurecommand.h \
    qxrdhistogramcommand.h \
    qxrdplotcommand-ptr.h \
    qxrdfitcirclecommand.h \
    qxrdfitellipsecommand.h \
    qxrdfitellipsescommand.h \
    qxrdadjustenergycommand.h \
    qxrdadjustdistancecommand.h \
    qxrdadjustfitcommand.h \
    qxrdaddpointcommand.h \
    qxrddelpointcommand.h \
    qxrddelringcommand.h \
    qxrddelallpointscommand.h \
    qxrddisableringcommand.h \
    qxrdenableringcommand.h \
    qxrdnormalizeringscommand.h \
    qxrdfitpeakcommand.h \
    qxrdfitringpointcommand.h \
    qxrdtraceringcommand.h \
    qxrdmissingringcommand.h \
    qxrdzappixelcommand.h \
    qxrdplotbuttoncommand.h \
    qxrdplotcontextmenucommand.h \
    qxrdcontextseparatorcommand.h \
    qxrdautoscalecommand.h \
    qxrdprintplotcommand.h \
    qxrdimageplotwidget-ptr.h \
    qxrdplotwidget-ptr.h \
    qxrddisplaysubmenucommand.h \
    qxrdplotpreferencescommand.h \
    qxrdprocessor.h \
    qxrddisplaydialog-ptr.h \
    qxrddistortionplotwidget.h \
    qxrdextrainputsplotwidget.h \
    qxrdextraoutputsplotwidget.h \
    qxrdhistogramplotwidget.h \
    qxrdsliceplotwidget.h \
    qxrddetectorplotwidget.h \
    qxrdplugininfomodel.h \
    qxrdplugininfomodel-ptr.h \
    qxrdprocessorinterface-ptr.h \
    qxrddexelasettings.h \
    qxrddexelasettings-ptr.h \
    qxrddetectorlistmodel-ptr.h \
    qxrddetectorlistmodel.h \
    qxrdfilewatchersettings.h \
    qxrdfilewatchersettings-ptr.h \
    qxrdpilatussettings.h \
    qxrdpilatussettings-ptr.h \
    qxrdsimulatedsettings.h \
    qxrdsimulatedsettings-ptr.h \
    qxrddetectordialog.h \
    qxrdsimulateddialog.h \
    qxrdperkinelmerdialog.h \
    qxrdpilatusdialog.h \
    qxrdareadetectordialog.h \
    qxrdfilewatcherdialog.h \
    qxrddexeladialog.h \
    qxrdareadetectordialog-ptr.h \
    qxrddexeladialog-ptr.h \
    qxrdfilewatcherdialog-ptr.h \
    qxrdperkinelmerdialog-ptr.h \
    qxrdpilatusdialog-ptr.h \
    qxrdareadetectorsettings.h \
    qxrdareadetectorsettings-ptr.h

INCLUDEPATH += commands/00generic/
INCLUDEPATH += commands/specremote/

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
    qxrdacquisitionextrainputsdialogsettings.h \
    qxrdhelpbrowser.h \
    qxrduserscriptdialog.h \
    qxrdsynchronizedacquisitiondialogsettings.h \
    qxrdsynchronizedacquisitionplotsettings.h \
    qxrdimagedataformatcbf.h \
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
    qxrddetectorconfigurationdialog.h \
    qxrddetectorcontrolwindow.h \
    qxrddetectorcontrolwindow-ptr.h \
    qxrddetectorprocessor.h \
    qxrddetectorprocessor-ptr.h \
    qxrdroicalculator.h \
    qxrdroicalculator-ptr.h \
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
    qxrddetectorsettingsperkinelmer.h \
    qxrddetectorsettings.h \
    qxrddetectorsettings-ptr.h \
    qxrddetectordriver.h \
    qxrddetectordriversimulated.h \
    qxrddetectordriverperkinelmer.h \
    qxrddetectordriverdexela.h \
    qxrddetectordriverpilatus.h \
    qxrddetectordriverfilewatcher.h \
    qxrddetectordriverepicsarea.h \
    qxrddetectordriver-ptr.h \
    qxrddetectordriverthread.h \
    qxrddetectordriverthread-ptr.h \
    qxrddetectorsettingsperkinelmer-ptr.h \
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
    qxrdfittedrings.h \
    qxrdfittedrings-ptr.h \
    qxrdtestimagegenerator.h \
    qxrdtestimagegeneratorwindow.h \
    qxrdtestscangeneratorwindow.h \
    qxrdtestimagegeneratorthread.h \
    qxrdtestimagegenerator-ptr.h \
    qxrdtestscangenerator-ptr.h \
    qxrdtestscangeneratorthread-ptr.h \
    qxrdtestimagegeneratorthread-ptr.h \
    qxrdtestscangenerator.h \
    qxrdtestscangeneratorthread.h \
    qxrdtestscanwindow.h \
    qxrdtestscanwindowdatavis.h \
    qxrdtestimagewindow.h \
    qxrdtestimagewindowqwt.h \
    qxrdtestimagewindowdatavis.h \
    qxrdtestscanwindowqwt.h \
    qxrdtestgenerator.h \
    qxrdtestimageplotqwt.h \
    qxrdtestimageplotdatavis.h \
    qxrdtestscanplotdatavis.h \
    qxrdtestscanplotqwt.h \
    qxrdtestscanplotdatavishelper.h \
    qxrdtestscanplotqwthelper.h \
    qxrdtestimageplotdatavishelper.h \
    qxrdtestimageplotqwthelper.h \
    qxrdtestscanplotdatavishelper-ptr.h \
    qxrdtestscanplotqwthelper-ptr.h \
    qxrdtestimageplotdatavishelper-ptr.h \
    qxrdtestimageplotqwthelper-ptr.h \
    qxrdplotcurvevector-ptr.h \
    qxrdplotcurvevector.h \
    qxrdtestthread.h


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
    qxrdacquisitionextrainputsdialog.ui \
    qxrduserscriptdialog.ui \
    qxrdscriptdialog.ui \
    qxrddetectorproxywidget.ui \
    qxrddetectorconfigurationdialog.ui \
    qxrddetectorcontrolwindow.ui \
    qxrdacquisitionscalerdialog.ui \
    qxrdzingerdialog.ui \
    qxrdpolartransformdialog.ui \
    qxrdpolarnormalizationdialog.ui \
    qxrdcalibrantpropertiesdialog.ui \
    qxrdintegratorparmsdialog.ui \
    qxrdroieditordialog.ui \
    qxrdtestimagegeneratorwindow.ui \
    qxrdtestscangeneratorwindow.ui \
    qxrdtestimagewindowqwt.ui \
    qxrdtestscanwindowqwt.ui \
    qxrdanalysiswindow.ui \
    qxrdcenteringwindow.ui \
    qxrdintegrationwindow.ui \
    qxrdmaskingwindow.ui \
    qxrdscriptingwindow.ui \
    qxrdacquisitionwindow.ui \
    qxrdcalibrantwindow.ui \
    qxrdhelpwindow.ui \
    qxrdcalculatorwindow.ui \
    qxrdinfowindow.ui \
    qxrdextraiowindow.ui \
    qxrdfilebrowserwidget.ui \
    qxrdplotwidget.ui \
    qxrdsimulateddialog.ui \
    qxrdperkinelmerdialog.ui \
    qxrdpilatusdialog.ui \
    qxrdareadetectordialog.ui \
    qxrdfilewatcherdialog.ui \
    qxrddexeladialog.ui

contains(DEFINES, HAVE_DATAVIS) {
  FORMS += \
    qxrdtestscanwindowdatavis.ui \
    qxrdtestimagewindowdatavis.ui
}

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
    help/qxrdhelpuserfunctions.html \
    help/README.html \

RESOURCES += qxrdresources.qrc

DISTFILES += \
    images/*.* \
    help/*.html \
    help/images/*.*

contains(DEFINES,HAVE_PERKIN_ELMER) {
  SOURCES += \
             qxrdperkinelmerplugininterface.cpp
  HEADERS += \
             qxrdperkinelmerplugininterface.h \
             qxrdperkinelmerplugininterface-ptr.h
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../../source/submodules/qceplib/qceplib-base-include.pri)
include(../../source/submodules/qceplib/qceplib-qwt-include.pri)
include(../../source/submodules/qceplib/qceplib-mar345-include.pri)
include(../../source/submodules/qceplib/qceplib-cbf-include.pri)
include(../../source/submodules/qceplib/qceplib-tiff-include.pri)
include(../../source/submodules/qceplib/qceplib-levmar-include.pri)
include(../../source/submodules/qceplib/qceplib-szip-include.pri)
include(../../source/submodules/qceplib/qceplib-zlib-include.pri)
include(../../source/submodules/qceplib/qceplib-hdf5-include.pri)
include(../../source/submodules/qceplib/qceplib-specserver-include.pri)

INCLUDEPATH += $${OUT_PWD}/../qceplib/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib
