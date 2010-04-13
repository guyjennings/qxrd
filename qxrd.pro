message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_SETTINGS])
message(Examples: $$[QT_INSTALL_EXAMPLES])
message(Demonstrations: $$[QT_INSTALL_DEMOS])

TEMPLATE = app
CONFIG += qt
include("qxrd.version.pri")
QT += network \
    script

# QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -DVERSION=\"\\\"$$VERSION\\\"\"

# QMAKE_CFLAGS += -g
# QMAKE_LFLAGS += -g
vectorize { 
    QMAKE_CXXFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
    QMAKE_CFLAGS += -msse2 \
        -ftree-vectorize \
        -ftree-vectorizer-verbose=5
}
profile { 
    QMAKE_CXXFLAGS += -pg
    QMAKE_CFLAGS += -pg
    QMAKE_LFLAGS += -pg
}

# INCLUDEPATH += /usr/local/lib/spec.d/include/
MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
TARGET = qxrd
RC_FILE=qxrd.rc
win32 { 
    contains(QMAKE_HOST.arch,x86_64) {
        CONFIG(debug, debug|release) {
            OBJECTS_DIR = objd-64
            TARGET = qxrdd
            CONFIG += console
        }
        else {
            OBJECTS_DIR = obj-64
            TARGET = qxrd
        }
    } else {
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
}
RESOURCES += qxrdhelptext.qrc \
    qxrdresources.qrc
DISTFILES += qxrdhelptext.html \
    qxrd.dox \
    download/index.php \
    images/*.png \
    tif_config.h \
    tiffconf.h
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
    qxrdfilebrowser.h \
    qxrdimagecalculator.h \
    qxrdmutexlocker.h
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
    qxrdfilebrowser.cpp \
    qxrdimagecalculator.cpp \
    qxrdmutexlocker.cpp
FORMS = qxrdwindow.ui \
    qxrdcenterfinderdialog.ui \
    qxrdintegratordialog.ui \
    qxrdfilebrowser.ui \
    qxrdimagecalculator.ui
unix { 
    HEADERS += xisl_dummy.h
    SOURCES += xisl_dummy.cpp
    
    # INCLUDEPATH += /usr/include/qwt/
    LIBS += -ltiff
}
win32 { 
contains(QMAKE_HOST.arch,x86_64) {
    HEADERS += xisl_dummy.h
    SOURCES += xisl_dummy.cpp
}

    PLATFORM_PREFIX = win32
    QTBINDIR = $$[QT_INSTALL_BINS]
    QTBASEDIR= $$[QT_INSTALL_PREFIX]

    exists($${QTBINDIR}) {
      LIBDIR = $${QTBINDIR}
      message("LIBDIR = $${LIBDIR}")
    }

    exists($${QTBASEDIR}/../mingw/bin/libgcc_s_dw2-1.dll) {
      message("MINGW found in $${QTBASEDIR}/../mingw/bin/libgcc_s_dw2-1.dll")
      MINGWRT = $${QTBASEDIR}/../mingw/bin/libgcc_s_dw2-1.dll $${QTBASEDIR}/../mingw/bin/mingwm10.dll
    }
    else:exists($${QTBASEDIR}/../mingw/bin/mingwm10.dll) {
      message("MINGW found in $${QTBASEDIR}/../mingw/bin/mingwm10.dll")
      MINGWRT = $${QTBASEDIR}/../mingw/bin/mingwm10.dll
    }
    else {
      message("MINGW Not Found")
    }
    app.target = app
    zip.target = zip
    CONFIG(debug, debug|release) { 
        app.commands = c:\cygwin\bin\cp.exe  \
            $${LIBDIR}/QtCored4.dll \
            $${LIBDIR}/QtNetworkd4.dll \
            $${LIBDIR}/QtGuid4.dll \
            $${LIBDIR}/QtScriptd4.dll \
            xisl.dll \
            $${MINGWRT} \
            debug
        zip.commands = c:\cygwin\bin\zip.exe -j \
            $${TARGET}_debug-$${VERSION}.zip \
            debug/$${TARGET}.exe \
            $${LIBDIR}/QtCored4.dll \
            $${LIBDIR}/QtNetworkd4.dll \
            $${LIBDIR}/QtGuid4.dll \
            $${LIBDIR}/QtScriptd4.dll \
            xisl.dll \
            $${MINGWRT}
    }
    else { 
        app.commands = c:\cygwin\bin\cp.exe \
            $${LIBDIR}/QtCore4.dll \
            $${LIBDIR}/QtNetwork4.dll \
            $${LIBDIR}/QtGui4.dll \
            $${LIBDIR}/QtScript4.dll \
            xisl.dll \
            $${MINGWRT} \
            release
        zip.commands = c:\cygwin\bin\zip.exe -j \
            $${TARGET}-$${VERSION}.zip \
            release/$${TARGET}.exe \
            $${LIBDIR}/QtCore4.dll \
            $${LIBDIR}/QtNetwork4.dll \
            $${LIBDIR}/QtGui4.dll \
            $${LIBDIR}/QtScript4.dll \
            xisl.dll \
            $${MINGWRT}
    }
    INCLUDEPATH += .
    INCLUDEPATH += SDK
    QMAKE_EXTRA_TARGETS += zip \
        app
    
    # QMAKE_LFLAGS += -Wl,--script,nordata.lscript
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
    download \
    www12.xor.aps.anl.gov:/var/www/html/software/qxrd/
OTHER_FILES += qxrd.rc qxrd.nsi
