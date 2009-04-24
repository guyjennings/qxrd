TEMPLATE = app

include("qxrdconfig.pri")

CONFIG += qt

VERSION = 0.0.3

QT += network script

#INCLUDEPATH += /usr/local/lib/spec.d/include/

MOC_DIR = moc
UI_DIR  = ui
OBJECTS_DIR = obj$${SUFFIX_STR}
RCC_DIR = rcc

TARGET = qxrd$${SUFFIX_STR}

RESOURCES += qxrdhelptext.qrc \
	qxrdresources.qrc

DISTFILES += qxrdhelptext.html \
	images/*.png

win32 {
  include("../qxrd/qt-libtiff-win32.pri")
}

win32 {
  INCLUDEPATH += ../qceplib/
  HEADERS += \
	../qceplib/spec_server.h \
	../qceplib/qspecserver.h \
        ../qceplib/qcepimagedata.h \
        ../qceplib/qcepimagedataformat.h \
        ../qceplib/qcepimagedataformattiff.h \
        ../qceplib/qcepimagedataformatmar345.h \
        ../qceplib/qcepimagedataformatfactory.h
}

HEADERS += \
        qwt_plot_rescaler.h \
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
        qxrddataprocessor.h \
	qxrdcenterfinder.h \
	qxrdcenterfinderplot.h \
	qxrdcenterfinderdialog.h \
	qxrdintegrator.h \
	qxrdintegratorplot.h \
	qxrdintegratordialog.h \
	qxrdimageplot.h \
        qxrdplotzoomer.h \
        qxrdplottracker.h \
	qxrdserver.h \
	qxrdserverthread.h \
	qxrdspecserver.h \
	qxrdacquisitionthread.h \
	qxrdacquisition.h \
	Acq.h

unix {
  HEADERS += \
	AcqLinuxTypes.h
}

win32 {
SOURCES += \
	../qceplib/qspecserver.cpp \
        ../qceplib/qcepimagedata.cpp \
        ../qceplib/qcepimagedataformat.cpp \
        ../qceplib/qcepimagedataformattiff.cpp \
        ../qceplib/qcepimagedataformatmar345.cpp \
        ../qceplib/qcepimagedataformatfactory.cpp
}

SOURCES += \
	qxrd.cpp \
        qwt_plot_rescaler.cpp \
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
	qxrdcenterfinder.cpp \
	qxrdcenterfinderplot.cpp \
	qxrdcenterfinderdialog.cpp \
	qxrdintegrator.cpp \
	qxrdintegratorplot.cpp \
	qxrdintegratordialog.cpp \
	qxrdimageplot.cpp \
        qxrdplotzoomer.cpp \
        qxrdplottracker.cpp \
	qxrdserver.cpp \
	qxrdserverthread.cpp \
	qxrdspecserver.cpp \
	qxrdacquisitionthread.cpp \
	qxrdacquisition.cpp


FORMS = \
	qxrdwindow.ui \
	qxrdcenterfinderdialog.ui \
	qxrdintegratordialog.ui

unix{
  HEADERS += xisl_dummy.h
  SOURCES += xisl_dummy.cpp
  INCLUDEPATH += /usr/include/qwt/
  LIBS += -lqwt -ltiff -lqceplib
}

win32{
  PLATFORM_PREFIX=win32
  QTBINDIR=C:/Qt/2009.01/qt/bin
#  QTBINDIR=C:/Qt/4.4.3/bin
#  QWT=C:/Qwt-5.1.1/
  QWT=C:/Qwt-5.1.2-svn/
  MINGW=C:/Qt/2009.01/mingw/bin
#  MINGW=c:/mingw/bin
  app.target = app
  zip.target = zip
  CONFIG(debug, debug|release) {
    LIBS += -L$${QWT}/lib/ -lqwtd5

	app.commands = C:/cygwin/bin/cp.exe \
                        $${QTBINDIR}/QtCored4.dll \
                        $${QTBINDIR}/QtNetworkd4.dll \
                        $${QTBINDIR}/QtGuid4.dll \
                        $${QTBINDIR}/QtScriptd4.dll \
                        $${QWT}/lib/qwtd5.dll \
                        XISL.dll \
                        $${MINGW}/mingwm10.dll \
                        debug
   
    zip.commands = C:/cygwin/bin/zip.exe -j $${TARGET}_debug.zip \
			debug/$${TARGET}.exe \
                        $${QTBINDIR}/QtCored4.dll \
                        $${QTBINDIR}/QtNetworkd4.dll \
                        $${QTBINDIR}/QtGuid4.dll \
                        $${QTBINDIR}/QtScriptd4.dll \
                        $${QWT}/lib/qwtd5.dll \
                        XISL.dll \
                        $${MINGW}/mingwm10.dll
  }else{
    LIBS += -L$${QWT}/lib/ -lqwt5

	app.commands = C:/cygwin/bin/cp.exe \
                        $${QTBINDIR}/QtCore4.dll \
                        $${QTBINDIR}/QtNetwork4.dll \
                        $${QTBINDIR}/QtGui4.dll \
                        $${QTBINDIR}/QtScript4.dll \
                        $${QWT}/lib/qwt5.dll \
						XISL.dll \
                        $${MINGW}/mingwm10.dll \
                        release
                        
    zip.commands = C:/cygwin/bin/zip.exe -j $${TARGET}.zip \
			release/$${TARGET}.exe \
                        $${QTBINDIR}/QtCore4.dll \
                        $${QTBINDIR}/QtNetwork4.dll \
                        $${QTBINDIR}/QtGui4.dll \
                        $${QTBINDIR}/QtScript4.dll \
                        $${QWT}/lib/qwt5.dll \
                        XISL.dll \
                        $${MINGW}/mingwm10.dll
  }
  INCLUDEPATH += $${QWT}/include/
  INCLUDEPATH += .
  INCLUDEPATH += SDK
  
  QMAKE_EXTRA_TARGETS += zip app
  QMAKE_LFLAGS += -Wl,--script,nordata.lscript
  
  LIBS += XISL.lib
}

QMAKE_EXTRA_TARGETS += dox rpmsource mock website

dox.target = docs
dox.commands += doxygen
dox.depends = Doxyfile $${SOURCES} $${HEADERS}

rpmsource.commands += make dist ;
rpmsource.commands += cp qxrd0.0.3.tar.gz ~/rpmbuild/SOURCES/
rpmsource.depends = $${DISTFILES} $${SOURCES} $${FORMS} $${HEADERS} $${RESOURCES}

mock.commands += mock-build qxrd.spec
mock.depends = rpmsource ~/rpmbuild/SOURCES/qxrd0.0.3.tar.gz qxrd.spec

website.commands =  rsync -e ssh -avx dox/html/ www12.xor.aps.anl.gov:/var/www/html/software/qxrd/
