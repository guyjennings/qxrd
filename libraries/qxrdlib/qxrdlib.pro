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
    qxrdwindow.cpp \
    qxrdfilesaverthread.cpp \
    qxrdfilesaver.cpp \
    qxrdintegrator.cpp \
    qxrdintegratorplot.cpp \
    qxrdserver.cpp \
    qxrdserverthread.cpp \
    qxrdacquisition.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrdexperimentpreferencesdialog.cpp \
    qxrdsharedpointer.cpp \
    qxrdresultserializer.cpp \
    qxrdhistogramdata.cpp \
    qxrdexposuretimespinner.cpp \
    qxrdgeneratetestimage.cpp \
    qxrdsimpleserver.cpp \
    qxrdsimpleserverthread.cpp \
    qxrdcenterstepspinner.cpp \
    qxrdmaskstackview.cpp \
    qxrdsynchronizedacquisition.cpp \
    qxrdfilebrowsermodel.cpp \
    qxrdfilebrowserview.cpp \
    qxrdhistogramdialog.cpp \
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
    qxrdwindowsettings.cpp \
    qxrdfilebrowsersettings.cpp \
    qxrdhistogramdialogsettings.cpp \
    triangulate.c \
    qxrddebug.cpp \
    qxrdhelpbrowser.cpp \
    qxrduserscriptdialog.cpp \
    qxrdimagedataformatcbf.cpp \
    qxrdplotvectorfieldcurve.cpp \
    qxrdpoint4d.cpp \
    qxrdplotvectorfielddata.cpp \
    qxrdscripteditwidget.cpp \
    qxrdplanefitter.cpp \
    qxrdimageplotzoomer.cpp \
    qxrdmainwindow.cpp \
    qxrdacquisitionparameterpack.cpp \
    qxrddarkacquisitionparameterpack.cpp \
    qxrddetectorcontrolwindow.cpp \
    qxrdacquisitionexecutionthread.cpp \
    qxrdacquisitionexecution.cpp \
    qxrdtodolist.cpp \
    qxrdtodolistitem.cpp \
    qxrddetectorimageplot.cpp \
    qxrdacquisitionscalermodel.cpp \
    qxrdzingerdialog.cpp \
    qxrdpolartransform.cpp \
    qxrdpolartransformdialog.cpp \
    qxrdpolarintensityfitter.cpp \
    qxrdpolarnormalization.cpp \
    qxrdpolarnormalizationdialog.cpp \
    qxrdcalibrantpropertiesdialog.cpp \
    qxrdintegratorparmsdialog.cpp \
    qxrdintegratorplotsettings.cpp \
    qxrddetectorsettings.cpp \
    qxrddetectordriver.cpp \
    qxrddetectordriverthread.cpp \
    qxrdexperimentthread.cpp \
    qxrddetectorcontrolwindowsettings.cpp \
    qxrdjsengine.cpp \
    qxrdplotcurvevector.cpp \
    commands/00generic/qxrdcommand.cpp \
    commands/00generic/qxrdapplicationcommand.cpp \
    commands/00generic/qxrdexperimentcommand.cpp \
    commands/00generic/qxrdgraphcommand.cpp \
    commands/00generic/qxrddatasetcommand.cpp \
    commands/specremote/qxrdspecremotecommand.cpp \
    qxrdanalysiswindow.cpp \
    qxrdcenteringwindow.cpp \
    qxrdintegrationwindow.cpp \
    qxrdmaskingwindow.cpp \
    qxrdscriptingwindow.cpp \
    qxrdanalysiswindowsettings.cpp \
    qxrdacquisitionwindowsettings.cpp \
    qxrdacquisitionwindow.cpp \
    qxrdcorrectiondialog.cpp \
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
    qxrdintegratedplotwidget.cpp \
    qxrdfilebrowserwidget.cpp \
    qxrdprocessor.cpp \
    qxrddistortionplotwidget.cpp \
    qxrdextrainputsplotwidget.cpp \
    qxrdextraoutputsplotwidget.cpp \
    qxrdhistogramplotwidget.cpp \
    qxrdsliceplotwidget.cpp \
    qxrdplugininfomodel.cpp \
    qxrddexelasettings.cpp \
    qxrddetectorlistmodel.cpp \
    qxrdfilewatchersettings.cpp \
    qxrdpilatussettings.cpp \
    qxrdsimulatedsettings.cpp \
    qxrdalliedvisionsettings.cpp \
    qxrddetectordialog.cpp \
    qxrdsimulateddialog.cpp \
    qxrdalliedvisiondialog.cpp \
    qxrdperkinelmerdialog.cpp \
    qxrdpilatusdialog.cpp \
    qxrdareadetectordialog.cpp \
    qxrdfilewatcherdialog.cpp \
    qxrddexeladialog.cpp \
    qxrdareadetectorsettings.cpp \
    qxrdperkinelmersettings.cpp \
    qxrdintegratedplotwidgetsettings.cpp \
    qxrdcenteringplotwidgetsettings.cpp \
    qxrdextrainputsplotwidgetsettings.cpp \
    qxrdextraoutputsplotwidgetsettings.cpp \
    qxrdhistogramplotwidgetsettings.cpp \
    qxrddistortionplotwidgetsettings.cpp \
    qxrdsliceplotwidgetsettings.cpp \
    qxrdprocessorstep.cpp \
    qxrdzingerfinder.cpp \
    qxrdwatcherwindow.cpp \
    qxrdwatcherwindowsettings.cpp \
    qxrdwatcher.cpp \
    qxrdappcommon.cpp \
    qxrdappviewer.cpp \
    qxrdviewersettings.cpp \
    qxrdacqcommon.cpp \
    qxrdacqdummy.cpp \
    qxrdstartupwindow.cpp \
    qxrdstartupwindowsettings.cpp \
    qxrddetectorplugin.cpp \
    qxrdcorrectiondialogpage.cpp \
    qxrdexposurepreferencesdialog.cpp \
    qxrdextraiodetectorsmodel.cpp \
    qxrdextraioinputsmodel.cpp \
    qxrdextraiooutputsmodel.cpp \
    qxrdsynchronizeddetectorchannel.cpp \
    qxrdsynchronizedoutputchannel.cpp \
    qxrdsynchronizedinputchannel.cpp \
    qxrdextraiodetectorsdelegate.cpp \
    qxrdextraioinputsdelegate.cpp \
    qxrdextraiooutputsdelegate.cpp \
    qxrdacquisitioneventlog.cpp \
    qxrdacquisitioneventlogwindow.cpp \
    qxrdacquisitioneventlogmodel.cpp \
    qxrdsynchronizerplugin.cpp \
    qxrdsynchronizer.cpp \
    qxrdsynchronizerthread.cpp \
    qxrdprocessorexecution.cpp \
    qxrdprocessorexecutionthread.cpp \
    qcepplotwidget.cpp \
    qcepplotwidgetsettings.cpp \
    qcepplotwidgetdialog.cpp \
    qcepplotcommand.cpp \
    qcepplotoverlay.cpp \
    qcepcolormaplibrary.cpp \
    qcepcolormap.cpp \
    qcepmaskcolormap.cpp \
    qcepplotcontextmenucommand.cpp \
    qcepautoscalecommand.cpp \
    qcepzoomincommand.cpp \
    qcepplotbuttoncommand.cpp \
    qcepzoomoutcommand.cpp \
    qcepzoomallcommand.cpp \
    qcepprintplotbutton.cpp \
    qcepplotpreferencesbutton.cpp \
    qcepprintplotcommand.cpp \
    qcepplotpreferencescommand.cpp \
    qcepaxissubmenucommand.cpp \
    qceprasterdata.cpp \
    qcepmaskrasterdata.cpp \
    qcepoverflowrasterdata.cpp \
    qcepimageplotwidgetsettings.cpp \
    qcepimageplotwidget.cpp \
    qcepimageplotwidgetdialog.cpp \
    qcepimageplot.cpp \
    qcepimageplotsettings.cpp \
    qcepaddpointcommand.cpp \
    qcepadjustdistancecommand.cpp \
    qcepadjustenergycommand.cpp \
    qcepadjustfitcommand.cpp \
    qcephistogramcommand.cpp \
    qcephistogramselector.cpp \
    qcepmaskstack.cpp \
    qcepmaskstackmodel.cpp \
    qcepmaskcirclescommand.cpp \
    qcepimageplotmeasurer.cpp \
    qcepmaskpicker.cpp \
    qcepmaskcommandbutton.cpp \
    qcepmaskpolygonscommand.cpp \
    qcepmeasurecommand.cpp \
    qceppowderpointpicker.cpp \
    qceppowderpointscommand.cpp \
    qcepdetectorgeometry.cpp \
    qcepcenterfinder.cpp \
    qcepcenterfinderpicker.cpp \
    qcepsetcentercommand.cpp \
    qcepplotslicer.cpp \
    qcepslicecommand.cpp \
    qcepcolormapsubmenucommand.cpp \
    qcepcontextseparatorcommand.cpp \
    qcepdelallpointscommand.cpp \
    qcepdelpointcommand.cpp \
    qcepdelringcommand.cpp \
    qcepdisableringcommand.cpp \
    qcepenableringcommand.cpp \
    qcepfitcirclecommand.cpp \
    qcepfitellipsecommand.cpp \
    qcepfitellipsescommand.cpp \
    qcepfitpeakcommand.cpp \
    qcepfitringpointcommand.cpp \
    qcepmissingringcommand.cpp \
    qcepnormalizeringscommand.cpp \
    qcepscalingsubmenucommand.cpp \
    qceptraceringcommand.cpp \
    qcepzappixelcommand.cpp \
    qcepfitter.cpp \
    qcepfitterpeakorring.cpp \
    qcepfitterpeakpoint.cpp \
    qcepfitterringpoint.cpp \
    qcepfitterringcircle.cpp \
    qcepfitterringellipse.cpp \
    qcepfittedrings.cpp \
    qcepcalibrantlibrarymodel.cpp \
    qcepcalibrantlibrary.cpp \
    qcepcalibrant.cpp \
    qcepcalibrantdspacingsmodel.cpp \
    qcepcalibrantdspacings.cpp \
    qcepcalibrantdspacing.cpp \
    qcepcentermarker.cpp \
    qceppowderpoint.cpp \
    qceppowderpointproperty.cpp \
    qceproimodel.cpp \
    qceproi.cpp \
    qceproicache.cpp \
    qceproicalculator.cpp \
    qceproicenteredshape.cpp \
    qceproishape.cpp \
    qceproipicker.cpp \
    qceproiellipse.cpp \
    qceproirectangle.cpp \
    qceproieditordialog.cpp \
    qceproipolygon.cpp \
    qceproioverlay.cpp \
    qceproirasterdata.cpp \
    qceproitypedelegate.cpp \
    qceproivector.cpp \
    qceppolygonpointsmodel.cpp \
    qceppowderringsmodel.cpp \
    qceppowderoverlay.cpp

HEADERS += qxrdlib.h\
        qxrdlib_global.h \
    commands/00generic/qxrdcommand.h \
    commands/00generic/qxrdapplicationcommand.h \
    commands/00generic/qxrdexperimentcommand.h \
    commands/00generic/qxrdgraphcommand.h \
    commands/00generic/qxrddatasetcommand.h \
    commands/specremote/qxrdspecremotecommand.h \
    qxrdanalysiswindow.h \
    qxrdcenteringwindow.h \
    qxrdintegrationwindow.h \
    qxrdmaskingwindow.h \
    qxrdscriptingwindow.h \
    qxrdanalysiswindowsettings.h \
    qxrdacquisitionwindowsettings.h \
    qxrdacquisitionwindow.h \
    qxrdcorrectiondialog.h \
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
    qxrdprocessor.h \
    qxrddistortionplotwidget.h \
    qxrdextrainputsplotwidget.h \
    qxrdextraoutputsplotwidget.h \
    qxrdhistogramplotwidget.h \
    qxrdsliceplotwidget.h \
    qxrdplugininfomodel.h \
    qxrdplugininfomodel-ptr.h \
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
    qxrdalliedvisionsettings.h \
    qxrdalliedvisionsettings-ptr.h \
    qxrddetectordialog.h \
    qxrdalliedvisiondialog.h \
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
    qxrdareadetectorsettings-ptr.h \
    qxrdperkinelmersettings.h \
    qxrdperkinelmersettings-ptr.h \
    qxrdalliedvisiondialog-ptr.h \
    qxrdsimulateddialog-ptr.h \
    qxrdcenteringplotwidgetsettings-ptr.h \
    qxrdintegratedplotwidgetsettings.h \
    qxrdintegratedplotwidgetsettings-ptr.h \
    qxrdcenteringplotwidgetsettings.h \
    qxrdextrainputsplotwidgetsettings.h \
    qxrdextrainputsplotwidgetsettings-ptr.h \
    qxrdextraoutputsplotwidgetsettings.h \
    qxrdextraoutputsplotwidgetsettings-ptr.h \
    qxrdhistogramplotwidgetsettings.h \
    qxrdhistogramplotwidgetsettings-ptr.h \
    qxrddistortionplotwidgetsettings.h \
    qxrddistortionplotwidgetsettings-ptr.h \
    qxrdsliceplotwidgetsettings.h \
    qxrdsliceplotwidgetsettings-ptr.h \
    qxrdzoomincommand-ptr.h \
    qwt_plot_spectrogram-ptr.h \
    qxrdprocessor-ptr.h \
    qxrdprocessorstep.h \
    qxrdprocessorstep-ptr.h \
    qxrdzingerfinder.h \
    qxrdzingerfinder-ptr.h \
    qwt_plot_picker-ptr.h \
    qxrdwatcherwindow.h \
    qxrdwatcherwindow-ptr.h \
    qxrdwatcherwindowsettings.h \
    qxrdwatcherwindowsettings-ptr.h \
    qxrdwatcher.h \
    qxrdwatcher-ptr.h \
    qxrdappcommon.h \
    qxrdappcommon-ptr.h \
    qxrdappviewer.h \
    qxrdappviewer-ptr.h \
    qxrdviewersettings.h \
    qxrdacqcommon.h \
    qxrdacqcommon-ptr.h \
    qxrdacqdummy.h \
    qxrdacqdummy-ptr.h \
    qxrdresultserializer-ptr.h \
    qfilesystemwatcher-ptr.h \
    qtimer-ptr.h \
    qxrdstartupwindow.h \
    qxrdstartupwindow-ptr.h \
    qxrdstartupwindowsettings.h \
    qxrdstartupwindowsettings-ptr.h \
    qxrddetectorplugin.h \
    qxrddetectorplugin-ptr.h \
    qxrdcorrectiondialogpage.h \
    qxrdcorrectiondialogpage-ptr.h \
    qxrdexposurepreferencesdialog.h \
    qxrdextraiodetectorsmodel.h \
    qxrdextraioinputsmodel.h \
    qxrdextraiooutputsmodel.h \
    qxrdextraiooutputsmodel-ptr.h \
    qxrdextraioinputsmodel-ptr.h \
    qxrdextraiodetectorsmodel-ptr.h \
    qxrdsynchronizeddetectorchannel.h \
    qxrdsynchronizedoutputchannel.h \
    qxrdsynchronizedinputchannel.h \
    qxrdsynchronizeddetectorchannel-ptr.h \
    qxrdsynchronizedinputchannel-ptr.h \
    qxrdsynchronizedoutputchannel-ptr.h \
    qxrdextraiodetectorsdelegate.h \
    qxrdextraiodetectorsdelegate-ptr.h \
    qxrdextraioinputsdelegate.h \
    qxrdextraioinputsdelegate-ptr.h \
    qxrdextraiooutputsdelegate.h \
    qxrdextraiooutputsdelegate-ptr.h \
    qxrdacquisitioneventlog.h \
    qxrdacquisitioneventlog-ptr.h \
    qxrdacquisitioneventlogmodel-ptr.h \
    qxrdacquisitioneventlogwindow.h \
    qxrdacquisitioneventlogmodel.h \
    qxrdacquisitioneventlogwindow-ptr.h \
    qxrdinfowindow-ptr.h \
    qxrdsynchronizerplugin.h \
    qxrdsynchronizerplugin-ptr.h \
    qxrdsynchronizer.h \
    qxrdsynchronizer-ptr.h \
    qxrdsynchronizerthread.h \
    qxrdsynchronizerthread-ptr.h \
    qxrdprocessorexecution.h \
    qxrdprocessorexecution-ptr.h \
    qxrdprocessorexecutionthread.h \
    qxrdprocessorexecutionthread-ptr.h \
    qcepplotwidget.h \
    qcepplotwidget-ptr.h \
    qcepplotwidgetsettings.h \
    qcepplotwidgetsettings-ptr.h \
    qcepplotwidgetdialog.h \
    qcepplotwidgetdialog-ptr.h \
    qcepplotcommand.h \
    qcepplotcommand-ptr.h \
    qcepplotoverlay.h \
    qcepplotoverlay-ptr.h \
    qcepcolormaplibrary.h \
    qcepcolormaplibrary-ptr.h \
    qcepcolormap.h \
    qcepcolormap-ptr.h \
    qcepmaskcolormap.h \
    qcepplotcontextmenucommand.h \
    qcepmaskcolormap-ptr.h \
    qcepautoscalecommand.h \
    qcepzoomincommand.h \
    qcepplotbuttoncommand.h \
    qcepzoomoutcommand.h \
    qcepzoomallcommand.h \
    qcepprintplotbutton.h \
    qcepplotpreferencesbutton.h \
    qcepprintplotcommand.h \
    qcepplotpreferencescommand.h \
    qcepaxissubmenucommand.h \
    qceprasterdata-ptr.h \
    qceprasterdata.h \
    qcepmaskrasterdata.h \
    qcepoverflowrasterdata.h \
    qcepimageplotwidgetsettings-ptr.h \
    qcepimageplotwidget-ptr.h \
    qcepimageplotwidgetsettings.h \
    qcepimageplotwidget.h \
    qcepoverflowrasterdata-ptr.h \
    qcepimageplotwidgetdialog.h \
    qcepimageplotwidgetdialog-ptr.h \
    qcepimageplot.h \
    qcepimageplotsettings-ptr.h \
    qcepimageplotsettings.h \
    qcepmaskrasterdata-ptr.h \
    qcepimageplot-ptr.h \
    qcepaddpointcommand.h \
    qcepadjustdistancecommand.h \
    qcepadjustenergycommand.h \
    qcepadjustfitcommand.h \
    qcephistogramcommand.h \
    qcephistogramselector.h \
    qcephistogramselector-ptr.h \
    qcepmaskstack.h \
    qcepmaskstack-ptr.h \
    qcepmaskstackmodel-ptr.h \
    qcepmaskstackmodel.h \
    qcepmaskcirclescommand.h \
    qcepimageplotmeasurer.h \
    qcepmaskpicker.h \
    qcepmaskpicker-ptr.h \
    qcepmaskcommandbutton.h \
    qcepmaskpolygonscommand.h \
    qcepimageplotmeasurer-ptr.h \
    qcepmeasurecommand.h \
    qceppowderpointpicker-ptr.h \
    qceppowderpointpicker.h \
    qceppowderpointscommand.h \
    qcepdetectorgeometry.h \
    qcepdetectorgeometry-ptr.h \
    qcepcenterfinder-ptr.h \
    qcepcenterfinder.h \
    qcepcenterfinderpicker.h \
    qcepsetcentercommand.h \
    qcepplotslicer.h \
    qcepplotslicer-ptr.h \
    qcepslicecommand.h \
    qcepcolormapsubmenucommand.h \
    qcepcontextseparatorcommand.h \
    qcepdelallpointscommand.h \
    qcepdelpointcommand.h \
    qcepdelringcommand.h \
    qcepdisableringcommand.h \
    qcepenableringcommand.h \
    qcepfitcirclecommand.h \
    qcepfitellipsecommand.h \
    qcepfitellipsescommand.h \
    qcepfitpeakcommand.h \
    qcepfitringpointcommand.h \
    qcepmissingringcommand.h \
    qcepnormalizeringscommand.h \
    qcepscalingsubmenucommand.h \
    qceptraceringcommand.h \
    qcepzappixelcommand.h \
    qcepfitter.h \
    qcepfitterpeakorring.h \
    qcepfitterpeakpoint.h \
    qcepfitterringpoint.h \
    qcepfitterringcircle.h \
    qcepfitterringellipse.h \
    qcepfittedrings.h \
    qcepfittedrings-ptr.h \
    qcepcenterfinderpicker-ptr.h \
    qcepcalibrantlibrary-ptr.h \
    qcepcalibrantlibrarymodel-ptr.h \
    qcepcalibrantlibrarymodel.h \
    qcepcalibrantlibrary.h \
    qcepcalibrant-ptr.h \
    qcepcalibrant.h \
    qcepcalibrantdspacingsmodel-ptr.h \
    qcepcalibrantdspacingsmodel.h \
    qcepcalibrantdspacings-ptr.h \
    qcepcalibrantdspacings.h \
    qcepcalibrantdspacing.h \
    qcepcalibrantdspacing-ptr.h \
    qcepcentermarker.h \
    qceppowderpoint.h \
    qceppowderpointproperty.h \
    qceproimodel-ptr.h \
    qceproimodel.h \
    qceproi-ptr.h \
    qceproi.h \
    qceproicache-ptr.h \
    qceproicache.h \
    qceproicalculator-ptr.h \
    qceproicalculator.h \
    qceproicenteredshape-ptr.h \
    qceproicenteredshape.h \
    qceproishape-ptr.h \
    qceproishape.h \
    qceproipicker-ptr.h \
    qceproipicker.h \
    qceproiellipse.h \
    qceproirectangle.h \
    qceproieditordialog.h \
    qceproipolygon-ptr.h \
    qceproipolygon.h \
    qceproioverlay-ptr.h \
    qceproioverlay.h \
    qceproirasterdata.h \
    qceproitypedelegate.h \
    qceproivector-ptr.h \
    qceproivector.h \
    qceppolygonpointsmodel.h \
    qceppowderringsmodel-ptr.h \
    qceppowderringsmodel.h \
    qceppowderoverlay-ptr.h \
    qceppowderoverlay.h

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
    qxrdwindow.h \
    qxrdwindow-ptr.h \
    qxrdfilesaverthread.h \
    qxrdfilesaverthread-ptr.h \
    qxrdfilesaver.h \
    qxrdfilesaver-ptr.h \
    qxrdintegrator.h \
    qxrdintegrator-ptr.h \
    qxrdintegratorplot.h \
    qxrdserver.h \
    qxrdserver-ptr.h \
    qxrdserverthread.h \
    qxrdserverthread-ptr.h \
    qxrdacquisition.h \
    qxrdacquisition-ptr.h \
    qxrdscriptengine.h \
    qxrdscriptengine-ptr.h \
    qxrdscriptenginethread.h \
    qxrdscriptenginethread-ptr.h \
    qxrdexperimentpreferencesdialog.h \
    qxrdsharedpointer.h \
    qxrdresultserializer.h \
    qxrdhistogramdata.h \
    qxrdhistogramdata-ptr.h \
    qxrdexposuretimespinner.h \
    qxrdgeneratetestimage.h \
    qxrdgeneratetestimage-ptr.h \
    qxrdsimpleserver.h \
    qxrdsimpleserver-ptr.h \
    qxrdsimpleserverthread.h \
    qxrdsimpleserverthread-ptr.h \
    qxrdcenterstepspinner.h \
    qxrdmaskstackview.h \
    qxrdsynchronizedacquisition.h \
    qxrdsynchronizedacquisition-ptr.h \
    qxrdfilebrowsermodel.h \
    qxrdfilebrowsermodel-ptr.h \
    qxrdfilebrowserview.h \
    qxrdhistogramdialog.h \
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
    qxrdwindowsettings.h \
    qxrdfilebrowsersettings.h \
    qxrdhistogramdialogsettings.h \
    triangulate.h \
    qxrddebug.h \
    qxrdhelpbrowser.h \
    qxrduserscriptdialog.h \
    qxrdimagedataformatcbf.h \
    qxrdplotvectorfieldcurve.h \
    qxrdpoint4d.h \
    qxrdplotvectorfielddata.h \
    qxrdscripteditwidget.h \
    triangulate.h \
    qxrdplanefitter.h \
    qxrdimageplotzoomer.h \
    qxrdmainwindow.h \
    qxrdacquisitionparameterpack.h \
    qxrdacquisitionparameterpack-ptr.h \
    qxrddarkacquisitionparameterpack.h \
    qxrddarkacquisitionparameterpack-ptr.h \
    qxrddetectorcontrolwindow.h \
    qxrddetectorcontrolwindow-ptr.h \
    qxrdacquisitionexecutionthread.h \
    qxrdacquisitionexecutionthread-ptr.h \
    qxrdacquisitionexecution.h \
    qxrdacquisitionexecution-ptr.h \
    qxrdtodolist.h \
    qxrdtodolistitem.h \
    qxrdtodolistitem-ptr.h \
    qxrddetectorimageplot.h \
    qxrdacquisitionscalermodel.h \
    qxrdacquisitionscalermodel-ptr.h \
    qxrdzingerdialog.h \
    qxrdpolartransform.h \
    qxrdpolartransformdialog.h \
    qxrdpolartransform-ptr.h \
    qxrdpolarintensityfitter.h \
    qxrdpolarnormalization.h \
    qxrdpolarnormalizationdialog.h \
    qxrdpolarnormalization-ptr.h \
    qxrdcalibrantpropertiesdialog.h \
    qxrdintegratorparmsdialog.h \
    qxrdintegratorplotsettings.h \
    qxrdintegratorplotsettings-ptr.h \
    qxrdhistogramdialogsettings-ptr.h \
    qxrdfilebrowsersettings-ptr.h \
    qxrdwindowsettings-ptr.h \
    qxrddetectorsettings.h \
    qxrddetectorsettings-ptr.h \
    qxrddetectordriver.h \
    qxrddetectordriver-ptr.h \
    qxrddetectordriverthread.h \
    qxrddetectordriverthread-ptr.h \
    qxrdexperimentthread.h \
    qxrdexperimentthread-ptr.h \
    qxrddetectorimageplot-ptr.h \
    qxrddetectorcontrolwindowsettings.h \
    qxrddetectorcontrolwindowsettings-ptr.h \
    qxrdexperimentsettings-ptr.h \
    qxrdjsengine.h \
    qxrdjsengine-ptr.h \
    qxrdplotcurvevector-ptr.h \
    qxrdplotcurvevector.h


FORMS += qxrdwindow.ui \
    qxrdexperimentpreferencesdialog.ui \
    qxrdhistogramdialog.ui \
    qxrdwelcomewindow.ui \
    qxrdglobalpreferencesdialog.ui \
    qxrdwelcomerecentitem.ui \
    qxrduserscriptdialog.ui \
    qxrddetectorcontrolwindow.ui \
    qxrdzingerdialog.ui \
    qxrdpolartransformdialog.ui \
    qxrdpolarnormalizationdialog.ui \
    qxrdcalibrantpropertiesdialog.ui \
    qxrdintegratorparmsdialog.ui \
    qxrdanalysiswindow.ui \
    qxrdcenteringwindow.ui \
    qxrdintegrationwindow.ui \
    qxrdmaskingwindow.ui \
    qxrdscriptingwindow.ui \
    qxrdacquisitionwindow.ui \
    qxrdcorrectiondialog.ui \
    qxrdcalibrantwindow.ui \
    qxrdhelpwindow.ui \
    qxrdcalculatorwindow.ui \
    qxrdinfowindow.ui \
    qxrdextraiowindow.ui \
    qxrdfilebrowserwidget.ui \
    qxrdalliedvisiondialog.ui \
    qxrdsimulateddialog.ui \
    qxrdperkinelmerdialog.ui \
    qxrdpilatusdialog.ui \
    qxrdareadetectordialog.ui \
    qxrdfilewatcherdialog.ui \
    qxrddexeladialog.ui \
    qxrdwatcherwindow.ui \
    qxrdstartupwindow.ui \
    qxrdcorrectiondialogpage.ui \
    qxrdexposurepreferencesdialog.ui \
    qxrdacquisitioneventlogwindow.ui \
    qcepplotwidget.ui \
    qcepplotwidgetdialog.ui \
    qcepimageplotwidgetdialog.ui \
    qceproieditordialog.ui

contains(DEFINES, HAVE_DATAVIS) {
  FORMS +=
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
    help/images/*.* \
    images/makeIcons.sh

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
