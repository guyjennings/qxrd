TEMPLATE = lib
CONFIG += qt dll

VERSION = 0.0.5

QT += network script

#INCLUDEPATH += /usr/local/lib/spec.d/include/

MOC_DIR = moc
UI_DIR  = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

TARGET = qceplib

INCLUDEPATH += .

HEADERS = \
	spec_server.h \
	qspecserver.h \
	qcepimagedata.h \
	qcepimagedataformat.h \
	qcepimagedataformatfactory.h \
	qcepimagedataformatmar345.h \
	qcepimagedataformattiff.h

SOURCES = \
	qspecserver.cpp \
	qcepimagedata.cpp \
	qcepimagedataformat.cpp \
	qcepimagedataformatfactory.cpp \
	qcepimagedataformatmar345.cpp \
	qcepimagedataformattiff.cpp
	
#unix{
#INCLUDEPATH += /usr/include/qwt/
#LIBS += -lqwt 
#}

win32{
  PLATFORM_PREFIX=win32
  QTBINDIR=C:/Qt/4.4.0/bin/
#  QWT=C:/Qwt-5.1.1/
  zip.target = zip
  CONFIG(debug, debug|release) {
#    LIBS += -L$${QWT}/lib/ -lqwt_debug
    zip.commands = C:/cygwin/bin/zip.exe -j $${TARGET}_debug.zip \
			debug/$${TARGET}.exe \
                        $${QTBINDIR}/QtCored4.dll \
                        $${QTBINDIR}/QtNetworkd4.dll \
                        $${QTBINDIR}/QtGuid4.dll \
                        $${QTBINDIR}/QtScriptd4.dll \
                        $${QWT}/lib/qwt_debug5.dll \
                        C:/MinGW/bin/mingwm10.dll
  }else{
#    LIBS += -L$${QWT}/lib/ -lqwt5
    zip.commands = C:/cygwin/bin/zip.exe -j $${TARGET}.zip \
			release/$${TARGET}.exe \
                        $${QTBINDIR}/QtCore4.dll \
                        $${QTBINDIR}/QtNetwork4.dll \
                        $${QTBINDIR}/QtGui4.dll \
                        $${QTBINDIR}/QtScript4.dll \
						$${QWT}/lib/qwt5.dll \
                        C:/MinGW/bin/mingwm10.dll
  }
#  INCLUDEPATH += $${QWT}/include/
  INCLUDEPATH += .

  QMAKE_EXTRA_TARGETS += zip	
  QMAKE_LFLAGS += -Wl,--script,nordata.lscript
}

QMAKE_EXTRA_TARGETS += dox rpmsource mock website

dox.target = docs
dox.commands += doxygen
dox.depends = Doxyfile $${SOURCES} $${HEADERS}

rpmsource.commands += make dist ;
rpmsource.commands += cp qceplib0.0.5.tar.gz ~/rpmbuild/SOURCES/
rpmsource.depends = $${DISTFILES} $${SOURCES} $${FORMS} $${HEADERS} $${RESOURCES}

mock.commands += mock-build qceplib.spec
mock.depends = rpmsource ~/rpmbuild/SOURCES/qceplib0.0.5.tar.gz qceplib.spec

website.commands =  rsync -e ssh -avx dox/html/ www12.xor.aps.anl.gov:/var/www/html/software/qceplib/
