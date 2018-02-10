message("qceplib-base PWD = $${PWD} QCEPLIB_VERSION = $${QCEPLIB_VERSION}")

greaterThan(QT_MAJOR_VERSION,4) {
  QT += widgets concurrent
}

CONFIG += qt

QT += script

include(qceplib-base-include-$${CEPLIB_VERSION}.pri)

contains (DEFINES,HAVE_QWT) {
 message(qceplib-base wants qwt)
 include(../qceplib-qwt-include.pri)
} else {
 message(qceplib-base no qwt)
}

include(../qceplib-tiff-include.pri)

SOURCES += \
  $${PWD}/qcepmacros.cpp \
  $${PWD}/qcepdebug.cpp \
  $${PWD}/qcepproperty.cpp \
  $${PWD}/qcepvector3dproperty.cpp \
  $${PWD}/qcepmatrix3x3property.cpp \
  $${PWD}/qcepplotmarker.cpp \
  $${PWD}/qcepplotmarkerlist.cpp \
  $${PWD}/qcepplotmarkerlistproperty.cpp \
  $${PWD}/qcepmutexlocker.cpp \
  $${PWD}/qcepdocumentationdictionary.cpp \
  $${PWD}/qcepimagedata.cpp \
  $${PWD}/qcepimagequeue.cpp \
  $${PWD}/qcepimagedataformatfactory.cpp \
  $${PWD}/qcepimagedataformat.cpp \
  $${PWD}/qcepobjectnamer.cpp \
  $${PWD}/qcepexperiment.cpp \
  $${PWD}/qceppropertyvalue.cpp \
  $${PWD}/qcepmatrix3x3.cpp \
  $${PWD}/qcepvector3d.cpp \
  $${PWD}/qcepobject.cpp \
  $${PWD}/qcepdataobject.cpp \
  $${PWD}/qcepdatagroup.cpp \
  $${PWD}/qcepdataarray.cpp \
  $${PWD}/qcepdatacolumnscan.cpp \
  $${PWD}/qcepdatacolumn.cpp \
  $${PWD}/qcepdataset.cpp \
  $${PWD}/qcepdatasetmodel.cpp \
  $${PWD}/qcepdataarrayspreadsheetmodel.cpp \
  $${PWD}/qcepdatacolumnspreadsheetmodel.cpp \
  $${PWD}/qcepdatacolumnscanspreadsheetmodel.cpp \
  $${PWD}/qcepdatagroupspreadsheetmodel.cpp \
  $${PWD}/qcepdataobjectpropertiesmodel.cpp \
  $${PWD}/qcepspreadsheetmodel.cpp \
  $${PWD}/qceppropertiesmodel.cpp  \
  $${PWD}/qcepintegrateddata.cpp \
  $${PWD}/qcepintegrateddataqueue.cpp \
  $${PWD}/qcepintegrateddataspreadsheetmodel.cpp \
  $${PWD}/qcepmaskdata.cpp \
  $${PWD}/qcepallocator.cpp \
  $${PWD}/qcepapplication.cpp \
  $${PWD}/qcepthread.cpp \
  $${PWD}/qcepimagedataspreadsheetmodel.cpp \
  $${PWD}/qcepnewdatagroupdialog.cpp \
  $${PWD}/qcepnewdatacolumndialog.cpp \
  $${PWD}/qcepnewcolumnscandialog.cpp \
  $${PWD}/qcepnewimagedialog.cpp \
  $${PWD}/qcepnewdataarraydialog.cpp \
  $${PWD}/qcepdataobjectpropertieswindow.cpp \
  $${PWD}/qcepdataobjectspreadsheetwindow.cpp \
  $${PWD}/qcepscriptengine.cpp \
  $${PWD}/qcepdataobjectwindow.cpp \
  $${PWD}/qcepmainwindow.cpp \
  $${PWD}/qcepscatterplotgraphmodel.cpp \
  $${PWD}/qcepdatasetselectiondialog.cpp \
  $${PWD}/qcepdataimportcommand.cpp \
  $${PWD}/qcepdataexportcommand.cpp \
  $${PWD}/qcepdataimportdialog.cpp \
  $${PWD}/qcepdataexportdialog.cpp \
  $${PWD}/qcepdataimportparameters.cpp \
  $${PWD}/qcepdataexportparameters.cpp \
  $${PWD}/qcepfileimporter.cpp \
  $${PWD}/qcepfileimportertext.cpp \
  $${PWD}/qcepfileimportertiff.cpp \
  $${PWD}/qcepfileimporterhdf.cpp \
  $${PWD}/qcepfileimportermultiple.cpp \
  $${PWD}/qcepapplicationsettings.cpp \
    $${PWD}/qcepfileformatter.cpp \
    $${PWD}/qcepfileformattertext.cpp \
    $${PWD}/qcepfileformatterhdf.cpp \
    $${PWD}/qcepfileformatternexus.cpp \
    $$PWD/qcepsetrangeofimagedialog.cpp \
    $$PWD/qcepsetdatavaluerangedialog.cpp \
    $$PWD/qcepfixupgainmapdialog.cpp \
    $$PWD/qcepfixupgainmapcommand.cpp \
    $$PWD/qcepsetdatavaluerangecommand.cpp \
    $$PWD/qcepsetrangeofimagecommand.cpp \
    $$PWD/qcepfixupdetectorpanelgains.cpp \
    $$PWD/qcepscandetectorpanelgains.cpp \
    $$PWD/qcepmainwindowsettings.cpp \
    $$PWD/qcepobjecttreemodel.cpp \
    $$PWD/qcepobjecttreewindow.cpp \
    $$PWD/qcepobjectpropertiesmodel.cpp \
    $$PWD/qnewthread.cpp \
    $$PWD/qnewapplication.cpp

contains (DEFINES, HAVE_DATAVIS) {
 SOURCES += \
    $$PWD/qcepdataobjectsurfaceplotwindow.cpp \
    $$PWD/qcepdataobjectsurfaceplotwidget.cpp
}

contains (DEFINES, HAVE_QWT) {
 SOURCES += \
  $$PWD/qcepdatasetbrowserview.cpp \
  $${PWD}/qcepdatasetbrowserdialog.cpp \
  $${PWD}/qcepdataobjectgraphcontroller.cpp \
  $${PWD}/qcepdataobjectgraphwindow.cpp \
  $${PWD}/qcepscatterplotgraphcontrols.cpp \
  $${PWD}/qcepscatterplotgraphcontroller.cpp \
  $${PWD}/qcepimagedatagraphcontroller.cpp \
  $${PWD}/qcepimagedatagraphcontrols.cpp \
  $${PWD}/qcepimageslicegraphcontroller.cpp \
  $${PWD}/qcepimageslicegraphcontrols.cpp \
  $${PWD}/qcepimagehistogramgraphcontroller.cpp \
  $${PWD}/qcepimagehistogramgraphcontrols.cpp \
  $${PWD}/qcepgraphcontrolswidget.cpp \
  $${PWD}/qcepplot.cpp \
  $${PWD}/qcepplotsettings.cpp \
  $${PWD}/qcepplotmeasurer.cpp \
  $${PWD}/qcepplotzoomer.cpp \
  $${PWD}/qwt_plot_piecewise_curve.cpp
}


HEADERS += \
  $${PWD}/qceplib_global.h \
  $${PWD}/qcepmacros.h \
  $${PWD}/qcepsharedpointer.h \
  $${PWD}/qcepdebug.h \
  $${PWD}/qcepproperty.h \
  $${PWD}/qcepproperty-ptr.h \
  $${PWD}/qcepvector3dproperty.h \
  $${PWD}/qcepmatrix3x3property.h \
  $${PWD}/qcepplotmarker.h \
  $${PWD}/qcepplotmarkerlist.h \
  $${PWD}/qcepplotmarkerlistproperty.h \
  $${PWD}/qcepmutexlocker.h \
  $${PWD}/qcepdocumentationdictionary.h \
  $${PWD}/qcepimagedata.h \
  $${PWD}/qcepimagedata-ptr.h \
  $${PWD}/qcepimagequeue.h \
  $${PWD}/qcepimagedataformatfactory.h \
  $${PWD}/qcepimagedataformatfactory-ptr.h \
  $${PWD}/qcepimagedataformat.h \
  $${PWD}/qcepobjectnamer.h \
  $${PWD}/qcepexperiment.h \
  $${PWD}/qcepexperiment-ptr.h \
  $${PWD}/qceppropertyvalue.h \
  $${PWD}/qcepmatrix3x3.h \
  $${PWD}/qcepvector3d.h \
  $${PWD}/qcepobject.h \
  $${PWD}/qcepobject-ptr.h \
  $${PWD}/qcepdataobject.h \
  $${PWD}/qcepdataobject-ptr.h \
  $${PWD}/qcepdatagroup.h \
  $${PWD}/qcepdatagroup-ptr.h \
  $${PWD}/qcepdataarray.h \
  $${PWD}/qcepdataarray-ptr.h \
  $${PWD}/qcepdatacolumn.h \
  $${PWD}/qcepdatacolumn-ptr.h \
  $${PWD}/qcepdatacolumnscan.h \
  $${PWD}/qcepdatacolumnscan-ptr.h \
  $${PWD}/qcepdataset.h \
  $${PWD}/qcepdataset-ptr.h \
  $${PWD}/qcepdatasetmodel.h \
  $${PWD}/qcepdatasetmodel-ptr.h \
  $${PWD}/qcepdataarrayspreadsheetmodel.h \
  $${PWD}/qcepdatacolumnspreadsheetmodel.h \
  $${PWD}/qcepdatacolumnscanspreadsheetmodel.h \
  $${PWD}/qcepdatagroupspreadsheetmodel.h \
  $${PWD}/qcepintegrateddataspreadsheetmodel.h \
  $${PWD}/qcepdataobjectpropertiesmodel.h \
  $${PWD}/qcepspreadsheetmodel.h \
  $${PWD}/qceppropertiesmodel.h \
  $${PWD}/qcepmaskdata.h \
  $${PWD}/qcepmaskdata-ptr.h \
  $${PWD}/qcepallocator.h \
  $${PWD}/qcepallocator-ptr.h \
  $${PWD}/qcepintegrateddata.h \
  $${PWD}/qcepintegrateddata-ptr.h \
  $${PWD}/qcepapplication.h \
  $${PWD}/qcepapplication-ptr.h \
  $${PWD}/qcepthread.h \
  $${PWD}/qcepintegrateddataqueue.h \
  $${PWD}/qcepimagedataspreadsheetmodel.h \
  $${PWD}/qcepnewdatagroupdialog.h \
  $${PWD}/qcepnewdatacolumndialog.h \
  $${PWD}/qcepnewcolumnscandialog.h \
  $${PWD}/qcepnewimagedialog.h \
  $${PWD}/qcepnewdataarraydialog.h \
  $${PWD}/qcepdataobjectpropertieswindow.h \
  $${PWD}/qcepdataobjectspreadsheetwindow.h \
  $${PWD}/qcepscriptengine.h \
  $${PWD}/qcepscriptengine-ptr.h \
  $${PWD}/qcepdataobjectwindow.h \
  $${PWD}/qcepmainwindow.h \
  $${PWD}/qcepmainwindow-ptr.h \
  $${PWD}/qcepscatterplotgraphmodel.h \
  $${PWD}/qcepscatterplotgraphmodel-ptr.h \
  $${PWD}/qcepdatasetselectiondialog.h \
  $${PWD}/qcepdataimportcommand.h \
  $${PWD}/qcepdataexportcommand.h \
  $${PWD}/qcepdataimportdialog.h \
  $${PWD}/qcepdataexportdialog.h \
  $${PWD}/qcepdataimportparameters.h \
  $${PWD}/qcepdataexportparameters.h \
  $${PWD}/qcepdataexportparameters-ptr.h \
  $${PWD}/qcepdataexportcommand-ptr.h \
  $${PWD}/qcepdataimportparameters-ptr.h \
  $${PWD}/qcepdataimportcommand-ptr.h \
  $${PWD}/qcepfileimporter.h \
  $${PWD}/qcepfileimportertext.h \
  $${PWD}/qcepfileimportertiff.h \
  $${PWD}/qcepfileimporterhdf.h \
  $${PWD}/qcepfileimporter-ptr.h \
  $${PWD}/qcepfileimportermultiple.h \
  $${PWD}/qcepapplicationsettings.h \
  $${PWD}/qcepapplicationsettings-ptr.h \
    $${PWD}/qcepfileformatter.h \
    $${PWD}/qcepfileformatter-ptr.h \
    $${PWD}/qcepfileformattertext.h \
    $${PWD}/qcepfileformatterhdf.h \
    $${PWD}/qcepfileformatternexus.h \
    $$PWD/qcepsetrangeofimagedialog.h \
    $$PWD/qcepsetdatavaluerangedialog.h \
    $$PWD/qcepfixupgainmapdialog.h \
    $$PWD/qcepfixupgainmapcommand.h \
    $$PWD/qcepsetdatavaluerangecommand.h \
    $$PWD/qcepsetrangeofimagecommand.h \
    $$PWD/qcepsetrangeofimagecommand-ptr.h \
    $$PWD/qcepsetdatavaluerangecommand-ptr.h \
    $$PWD/qcepfixupgainmapcommand-ptr.h \
    $$PWD/qcepfixupdetectorpanelgains.h \
    $$PWD/qcepscandetectorpanelgains.h \
    $$PWD/qcepmainwindowsettings.h \
    $$PWD/qcepmainwindowsettings-ptr.h \
    $$PWD/qobject-ptr.h \
    $$PWD/qcepobjecttreemodel.h \
    $$PWD/qcepobjecttreewindow.h \
    $$PWD/qcepobjectpropertiesmodel.h \
    $$PWD/qthread-ptr.h \
    $$PWD/qnewthread.h \
    $$PWD/qnewthread-ptr.h \
    $$PWD/qcepthread-ptr.h \
    $$PWD/qapplication-ptr.h \
    $$PWD/qnewapplication.h \
    $$PWD/qnewapplication-ptr.h

contains (DEFINES, HAVE_DATAVIS) {
 HEADERS += \
    $$PWD/qcepdataobjectsurfaceplotwindow.h \
    $$PWD/qcepdataobjectsurfaceplotwidget.h
}

contains (DEFINES, HAVE_QWT) {
 HEADERS += \
  $$PWD/qcepdatasetbrowserview.h \
  $${PWD}/qcepdatasetbrowserdialog.h \
  $${PWD}/qcepdataobjectgraphcontroller-ptr.h \
  $${PWD}/qcepdataobjectgraphcontroller.h \
  $${PWD}/qcepdataobjectgraphwindow.h \
  $${PWD}/qcepscatterplotgraphcontrols.h \
  $${PWD}/qcepscatterplotgraphcontroller.h \
  $${PWD}/qcepimagedatagraphcontroller.h \
  $${PWD}/qcepimagedatagraphcontrols.h \
  $${PWD}/qcepimagehistogramgraphcontroller.h \
  $${PWD}/qcepimagehistogramgraphcontrols.h \
  $${PWD}/qcepimageslicegraphcontroller.h \
  $${PWD}/qcepimageslicegraphcontrols.h \
  $${PWD}/qcepgraphcontrolswidget.h \
  $${PWD}/qcepplot-ptr.h \
  $${PWD}/qcepplot.h \
  $${PWD}/qcepplotsettings-ptr.h \
  $${PWD}/qcepplotsettings.h \
  $${PWD}/qcepplotmeasurer-ptr.h \
  $${PWD}/qcepplotmeasurer.h \
  $${PWD}/qcepplotzoomer.h \
  $${PWD}/qwt_plot_piecewise_curve-ptr.h \
  $${PWD}/qwt_plot_piecewise_curve.h
}

FORMS += \
  $${PWD}/qcepnewdatagroupdialog.ui \
  $${PWD}/qcepnewdatacolumndialog.ui \
  $${PWD}/qcepnewcolumnscandialog.ui \
  $${PWD}/qcepnewimagedialog.ui \
  $${PWD}/qcepnewdataarraydialog.ui \
  $${PWD}/qcepdataobjectpropertieswindow.ui \
  $${PWD}/qcepdataobjectspreadsheetwindow.ui \
  $${PWD}/qcepdatasetselectiondialog.ui \
  $${PWD}/qcepdataimportdialog.ui \
  $${PWD}/qcepdataexportdialog.ui \
    $$PWD/qcepsetrangeofimagedialog.ui \
    $$PWD/qcepsetdatavaluerangedialog.ui \
    $$PWD/qcepfixupgainmapdialog.ui \
    $$PWD/qcepobjecttreewindow.ui

contains(DEFINES, HAVE_QWT) {
 FORMS += \
  $${PWD}/qcepdatasetbrowserdialog.ui \
  $${PWD}/qcepdataobjectgraphwindow.ui \
  $${PWD}/qcepscatterplotgraphcontrols.ui \
  $${PWD}/qcepimagedatagraphcontrols.ui \
  $${PWD}/qcepimagehistogramgraphcontrols.ui \
  $${PWD}/qcepimageslicegraphcontrols.ui
}

contains(DEFINES, HAVE_DATAVIS) {
  FORMS += \
    $$PWD/qcepdataobjectsurfaceplotwindow.ui \
    $$PWD/qcepdataobjectsurfaceplotwidget.ui
}

RESOURCES += \
  $${PWD}/qcepresources.qrc
