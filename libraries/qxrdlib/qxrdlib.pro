#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T10:50:00
#
#-------------------------------------------------

include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

QT       += widgets qml network script scripttools concurrent datavisualization charts

win32:CONFIG(debug, debug|release) {
  TARGET = qxrdlibd
} else {
  TARGET = qxrdlib
}

TEMPLATE = lib
DESTDIR = ../../

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
    qxrdtestthread.cpp

HEADERS += qxrdlib.h\
        qxrdlib_global.h

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
    qxrdroieditordialog.ui \
    qxrdtestimagegeneratorwindow.ui \
    qxrdtestscangeneratorwindow.ui \
    qxrdtestscanwindowdatavis.ui \
    qxrdtestimagewindowqwt.ui \
    qxrdtestscanwindowqwt.ui \
    qxrdtestimagewindowdatavis.ui

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
