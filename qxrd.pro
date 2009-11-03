TEMPLATE = app
CONFIG += qt
include("qxrd.version.pri")
QT += network \
    script
#QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -DVERSION=\"\\\"$$VERSION\\\"\"
#QMAKE_CFLAGS += -g
#QMAKE_LFLAGS += -g
vectorize { 
    QMAKE_CXXFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
    QMAKE_CFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
}

# INCLUDEPATH += /usr/local/lib/spec.d/include/
MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
TARGET = qxrd
win32 { 
    CONFIG(debug, debug|release) { 
        OBJECTS_DIR = objd
        TARGET = qxrdd
        CONFIG += console
    }
    else { 
        OBJECTS_DIR = obj
        TARGET = qxrd
    }
}
RESOURCES += qxrdhelptext.qrc \
    qxrdresources.qrc
DISTFILES += qxrdhelptext.html \
    qxrd.dox \
    images/*.png
win32:include("qt-libtiff-win32.pri")
include("qwt-5.2.pri")
include("qceplib.pri")
HEADERS += TODO.h \
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
    qxrdcenterfinderpicker.h \
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
    Acq.h \
    qxrdacquisitionparameters.h \
    qxrdacquisitionscripting.h \
    qxrdacquisitionoperations.h \
    qxrdacquisitionperkinelmer.h \
    qxrdplot.h \
    qxrdplotslicer.h \
    qxrdplotmeasurer.h \
    qxrdscriptengine.h \
    qxrdscriptenginethread.h \
    qxrddataprocessorthread.h \
    qxrdacquisitionsimulated.h \
    qxrdmaskpicker.h \
    qxrdmaskdata.h \
    qxrdimageplotmeasurer.h \
    qxrdfilebrowser.h
unix:HEADERS += AcqLinuxTypes.h
SOURCES += qxrd.cpp \
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
    qxrdcenterfinderpicker.cpp \
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
    qxrdacquisition.cpp \
    qxrdacquisitionparameters.cpp \
    qxrdacquisitionscripting.cpp \
    qxrdacquisitionoperations.cpp \
    qxrdacquisitionperkinelmer.cpp \
    qxrdplot.cpp \
    qxrdplotslicer.cpp \
    qxrdplotmeasurer.cpp \
    qxrdscriptengine.cpp \
    qxrdscriptenginethread.cpp \
    qxrddataprocessorthread.cpp \
    qxrdacquisitionsimulated.cpp \
    qxrdmaskpicker.cpp \
    qxrdmaskdata.cpp \
    qxrdimageplotmeasurer.cpp \
    qxrdfilebrowser.cpp
FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui
unix { 
    HEADERS += xisl_dummy.h
    SOURCES += xisl_dummy.cpp
    
    # INCLUDEPATH += /usr/include/qwt/
    LIBS += -ltiff
}
win32 { 
    PLATFORM_PREFIX = win32
#    QTBINDIR = C:/Qt/2009.03/qt/bin
    QTBINDIR = C:/Qt/qt-all-opensource-src-4.5.3/bin
    MINGW = C:/Qt/2009.03/qt/bin
    app.target = app
    zip.target = zip
    CONFIG(debug, debug|release) { 
        app.commands = C:/cygwin/bin/cp.exe \
            $${QTBINDIR}/QtCored4.dll \
            $${QTBINDIR}/QtNetworkd4.dll \
            $${QTBINDIR}/QtGuid4.dll \
            $${QTBINDIR}/QtScriptd4.dll \
            XISL.dll \
            $${MINGW}/mingwm10.dll \
            debug
        zip.commands = C:/cygwin/bin/zip.exe \
            -j \
            $${TARGET}_debug.zip \
            debug/$${TARGET}.exe \
            $${QTBINDIR}/QtCored4.dll \
            $${QTBINDIR}/QtNetworkd4.dll \
            $${QTBINDIR}/QtGuid4.dll \
            $${QTBINDIR}/QtScriptd4.dll \
            XISL.dll \
            $${MINGW}/mingwm10.dll
    }
    else { 
        app.commands = C:/cygwin/bin/cp.exe \
            $${QTBINDIR}/QtCore4.dll \
            $${QTBINDIR}/QtNetwork4.dll \
            $${QTBINDIR}/QtGui4.dll \
            $${QTBINDIR}/QtScript4.dll \
            XISL.dll \
            $${MINGW}/mingwm10.dll \
            release
        zip.commands = C:/cygwin/bin/zip.exe \
            -j \
            $${TARGET}.zip \
            release/$${TARGET}.exe \
            $${QTBINDIR}/QtCore4.dll \
            $${QTBINDIR}/QtNetwork4.dll \
            $${QTBINDIR}/QtGui4.dll \
            $${QTBINDIR}/QtScript4.dll \
            XISL.dll \
            $${MINGW}/mingwm10.dll
    }
    INCLUDEPATH += .
    INCLUDEPATH += SDK
    QMAKE_EXTRA_TARGETS += zip \
        app
    #QMAKE_LFLAGS += -Wl,--script,nordata.lscript
    
    # QMAKE_LFLAGS += -Wl,--disable-auto-import
    QMAKE_CFLAGS += -g
    QMAKE_CXXFLAGS += -g
    LIBS += XISL.lib
}
QMAKE_EXTRA_TARGETS += dox \
    rpmsource \
    mock \
    website
dox.target = docs
dox.commands += doxygen
dox.depends = Doxyfile \
    $${SOURCES} \
    $${HEADERS}
rpmsource.commands += make \
    dist \
    ;
rpmsource.commands += cp \
    qxrd0.0.3.tar.gz \
    ~/rpmbuild/SOURCES/
rpmsource.depends = $${DISTFILES} \
    $${SOURCES} \
    $${FORMS} \
    $${HEADERS} \
    $${RESOURCES}
mock.commands += mock-build \
    qxrd.spec
mock.depends = rpmsource \
    ~/rpmbuild/SOURCES/qxrd0.0.3.tar.gz \
    qxrd.spec
website.commands = rsync \
    -e \
    ssh \
    -avx \
    dox/html/ \
    www12.xor.aps.anl.gov:/var/www/html/software/qxrd/
OTHER_FILES += 
