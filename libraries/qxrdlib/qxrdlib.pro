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
    qxrdacquisition.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrdexperimentpreferencesdialog.cpp \
    qxrdresultserializer.cpp \
    qxrdhistogramdata.cpp \
    qxrdexposuretimespinner.cpp \
    qxrdgeneratetestimage.cpp \
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
    qxrddebug.cpp \
    qxrdhelpbrowser.cpp \
    qxrduserscriptdialog.cpp \
    qxrdimagedataformatcbf.cpp \
    qxrdplotvectorfieldcurve.cpp \
    qxrdpoint4d.cpp \
    qxrdplotvectorfielddata.cpp \
    qxrdscripteditwidget.cpp \
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
    qxrdoutputfileformatter.cpp \
    qxrdoutputfileformattersettings.cpp \
    qxrdoutputfileformattertiff.cpp \
    qxrdoutputfileformatterhdf.cpp \
    qxrdoutputfileformattertiff-bzip2.cpp \
    qxrdoutputfileformattertiff-zip.cpp \
    qxrdoutputfileformattertiff-gzip.cpp

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
    qxrdprocessor-ptr.h \
    qxrdprocessorstep.h \
    qxrdprocessorstep-ptr.h \
    qxrdzingerfinder.h \
    qxrdzingerfinder-ptr.h \
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
    qxrdoutputfileformatter.h \
    qxrdoutputfileformatter-ptr.h \
    qxrdoutputfileformattersettings.h \
    qxrdoutputfileformattersettings-ptr.h \
    qxrdoutputfileformattertiff.h \
    qxrdoutputfileformatterhdf.h

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
    qxrddebug.h \
    qxrdhelpbrowser.h \
    qxrduserscriptdialog.h \
    qxrdimagedataformatcbf.h \
    qxrdplotvectorfieldcurve.h \
    qxrdpoint4d.h \
    qxrdplotvectorfielddata.h \
    qxrdscripteditwidget.h \
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
    qxrdcorrectiondialogpage.ui \
    qxrdexposurepreferencesdialog.ui \
    qxrdacquisitioneventlogwindow.ui

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

include(../qceplib/qceplib/qceplib-base-include.pri)
include(../qceplib/qceplib/qceplib-qwt-include.pri)
include(../qceplib/qceplib/qceplib-mar345-include.pri)
include(../qceplib/qceplib/qceplib-cbf-include.pri)
include(../qceplib/qceplib/qceplib-tiff-include.pri)
include(../qceplib/qceplib/qceplib-levmar-include.pri)
include(../qceplib/qceplib/qceplib-szip-include.pri)
include(../qceplib/qceplib/qceplib-zlib-include.pri)
include(../qceplib/qceplib/qceplib-hdf5-include.pri)
include(../qceplib/qceplib/qceplib-bzip2-include.pri)
include(../qceplib/qceplib/qceplib-specserver-include.pri)

INCLUDEPATH += $${OUT_PWD}/../qceplib/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib
