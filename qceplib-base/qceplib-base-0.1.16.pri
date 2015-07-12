message(qceplib PWD = $${PWD} QCEPLIB_VERSION = $${QCEPLIB_VERSION})

greaterThan(QT_MAJOR_VERSION,4): QT += widgets concurrent

CONFIG += qt

QT += script

INCLUDEPATH += $${PWD}

SOURCES += \
  $${PWD}/qcepdebug.cpp \
  $${PWD}/qcepproperty.cpp \
  $${PWD}/qcepvector3dproperty.cpp \
  $${PWD}/qcepmatrix3x3property.cpp \
  $${PWD}/qcepplotmarker.cpp \
  $${PWD}/qcepplotmarkerlist.cpp \
  $${PWD}/qcepplotmarkerlistproperty.cpp \
  $${PWD}/qcepmutexlocker.cpp \
  $${PWD}/qcepdocumentationdictionary.cpp \
  $${PWD}/qcepsettingssaver.cpp \
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
  $${PWD}/qcepdatasetbrowserdialog.cpp \
  $${PWD}/qcepdataarrayspreadsheetmodel.cpp \
  $${PWD}/qcepdatacolumnspreadsheetmodel.cpp \
  $${PWD}/qcepdatacolumnscanspreadsheetmodel.cpp \
  $${PWD}/qcepdatagroupspreadsheetmodel.cpp \
  $${PWD}/qcepdataobjectpropertiesmodel.cpp \
  $${PWD}/qcepspreadsheetmodel.cpp \
  $${PWD}/qceppropertiesmodel.cpp  \
  $${PWD}/qcepnewdatagroupdialog.cpp \
  $${PWD}/qcepnewdatacolumndialog.cpp \
  $${PWD}/qcepnewcolumnscandialog.cpp \
  $${PWD}/qcepnewimagedialog.cpp \
  $${PWD}/qcepnewdataarraydialog.cpp \
  $${PWD}/qcepintegrateddata.cpp \
  $${PWD}/qcepintegrateddataqueue.cpp \
  $${PWD}/qcepintegrateddataspreadsheetmodel.cpp \
  $${PWD}/qcepmaskdata.cpp \
  $${PWD}/qcepallocator.cpp \
  $${PWD}/qcepapplication.cpp \
  $${PWD}/qcepthread.cpp \
  $${PWD}/qcepallocatorthread.cpp \
  $${PWD}/qcepimagedataobjectcounter.cpp \
  $${PWD}/qcepimagedatagraphcontrols.cpp \
  $${PWD}/qcepgraphcontrolswidget.cpp \
  $${PWD}/qcepdataobjectgraphcontroller.cpp \
  $${PWD}/qcepdataobjectgraphwindow.cpp \
  $${PWD}/qcepdataobjectpropertieswindow.cpp \
  $${PWD}/qcepdataobjectspreadsheetwindow.cpp \
  $${PWD}/qcepscatterplotgraphcontrols.cpp \
  $${PWD}/qcepscatterplotgraphcontroller.cpp \
  $${PWD}/qcepimagedatagraphcontroller.cpp \
  $${PWD}/qcepimagedataspreadsheetmodel.cpp \
  $${PWD}/qcepimageslicegraphcontroller.cpp \
  $${PWD}/qcepimageslicegraphcontrols.cpp \
  $${PWD}/qcepimagehistogramgraphcontroller.cpp \
  $${PWD}/qcepimagehistogramgraphcontrols.cpp

HEADERS += \
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
  $${PWD}/qcepsettingssaver.h \
  $${PWD}/qcepsettingssaver-ptr.h \
  $${PWD}/qcepimagedata.h \
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
  $${PWD}/qcepdataobject.h \
  $${PWD}/qcepdatagroup.h \
  $${PWD}/qcepdataarray.h \
  $${PWD}/qcepdatacolumnscan.h \
  $${PWD}/qcepdatacolumn.h \
  $${PWD}/qcepdataarray-ptr.h \
  $${PWD}/qcepdatacolumn-ptr.h \
  $${PWD}/qcepdatacolumnscan-ptr.h \
  $${PWD}/qcepdatagroup-ptr.h \
  $${PWD}/qcepdataobject-ptr.h \
  $${PWD}/qcepdataset.h \
  $${PWD}/qcepdataset-ptr.h \
  $${PWD}/qcepdatasetmodel.h \
  $${PWD}/qcepdatasetmodel-ptr.h \
  $${PWD}/qcepdatasetbrowserdialog.h \
  $${PWD}/qcepdataarrayspreadsheetmodel.h \
  $${PWD}/qcepdatacolumnspreadsheetmodel.h \
  $${PWD}/qcepdatacolumnscanspreadsheetmodel.h \
  $${PWD}/qcepdatagroupspreadsheetmodel.h \
  $${PWD}/qcepintegrateddataspreadsheetmodel.h \
  $${PWD}/qcepdataobjectpropertiesmodel.h \
  $${PWD}/qcepspreadsheetmodel.h \
  $${PWD}/qceppropertiesmodel.h \
  $${PWD}/qcepnewdatagroupdialog.h \
  $${PWD}/qcepnewdatacolumndialog.h \
  $${PWD}/qcepnewcolumnscandialog.h \
  $${PWD}/qcepnewimagedialog.h \
  $${PWD}/qcepnewdataarraydialog.h \
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
  $${PWD}/qcepallocatorthread.h \
  $${PWD}/qcepallocatorthread-ptr.h \
  $${PWD}/qcepallocatorthread-ptr.h \
  $${PWD}/qcepimagedataobjectcounter.h \
  $${PWD}/qcepdataobjectgraphcontroller-ptr.h \
  $${PWD}/qcepdataobjectgraphcontroller.h \
  $${PWD}/qcepdataobjectgraphwindow.h \
  $${PWD}/qcepdataobjectpropertieswindow.h \
  $${PWD}/qcepdataobjectspreadsheetwindow.h \
  $${PWD}/qcepgraphcontrolswidget.h \
  $${PWD}/qcepscatterplotgraphcontrols.h \
  $${PWD}/qcepimagedatagraphcontrols.h \
  $${PWD}/qcepimagedatagraphcontroller.h \
  $${PWD}/qcepimagedataspreadsheetmodel.h \
  $${PWD}/qcepimagehistogramgraphcontroller.h \
  $${PWD}/qcepimagehistogramgraphcontrols.h \
  $${PWD}/qcepimageslicegraphcontroller.h \
  $${PWD}/qcepimageslicegraphcontrols.h \
  $${PWD}/qcepscatterplotgraphcontroller.h

FORMS += \
  $${PWD}/qcepdatasetbrowserdialog.ui \
  $${PWD}/qcepnewdatagroupdialog.ui \
  $${PWD}/qcepnewdatacolumndialog.ui \
  $${PWD}/qcepnewcolumnscandialog.ui \
  $${PWD}/qcepnewimagedialog.ui \
  $${PWD}/qcepnewdataarraydialog.ui \
  $${PWD}/qcepdataobjectgraphwindow.ui \
  $${PWD}/qcepdataobjectpropertieswindow.ui \
  $${PWD}/qcepdataobjectspreadsheetwindow.ui \
  $${PWD}/qcepscatterplotgraphcontrols.ui \
  $${PWD}/qcepimagedatagraphcontrols.ui \
  $${PWD}/qcepimagehistogramgraphcontrols.ui \
  $${PWD}/qcepimageslicegraphcontrols.ui

RESOURCES += \
    $$PWD/qcepresources.qrc
