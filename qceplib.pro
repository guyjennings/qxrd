TARGET  = qceplib

VERSION = 0.1.1

TEMPLATE = subdirs

SUBDIRS  = tests

OTHER_FILES += \
    qceplib.pri \
    qceplib-cbf.pri \
    qceplib-mar345.pri \
    qceplib-specserver.pri \
    qceplib-hdf5.pri \
    qceplib-nexus.pri \
    qceplib-qwt.pri \
    qceplib-tiff.pri \
    qceplib-levmar.pri

QMAKE_EXTRA_TARGETS += tarball

tarball.depends = FORCE

tarball.commands += \
    rm -rf $${TARGET}-$${VERSION} ; \
    $(MKDIR) $${TARGET}-$${VERSION} &&

tarball.commands += \
    $(COPY_FILE) -r $${PWD}/* -t $${TARGET}-$${VERSION} &&

tarball.commands += \
    tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}
