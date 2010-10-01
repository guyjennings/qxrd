include("qxrd.version.pri")
include("qxrd.platform.pri")

message(Version = $${VERSION})
message(Host Arch = $${QMAKE_HOST.arch})

TEMPLATE = subdirs

SUBDIRS = source plugins

win32 {
#  include("qxrd.app.pri")
#  include("qxrd.nsis.pri")

  SUBDIRS += qxrd.app.pro qxrd.nsis.pro
}

DISTFILES += source plugins

OTHER_FILES += qxrd.nsi

QMAKE_EXTRA_TARGETS += dist

dist.depends = FORCE
dist.commands += \
        rm -rf qxrd-$${VERSION} ; \
        $(MKDIR) qxrd-$${VERSION} &&

dist.commands += \
        $(COPY_FILE) --parents *.{pro,pri,spec} source/images/*.{png,ico} source/{[TA]*.h,q*.cpp,q*.h,q*.qrc,q*.rc,q*.ui,source.pro,*.pri} \
                      source/levmar-2.5/*.{c,h} \
                      source/help/*.html \
                      source/help/images/*.png \
                      qwt-5.2/src/*.{cpp,h} \
                      qceplib/*.{cpp,h} \
                      tiffconfig/{macx,win32}/*.h tiff-3.8.2/libtiff/*.{c,h,def} \
                      plugins/plugins.pro plugins/*/q*.{cpp,h,pro} qxrd-$${VERSION} &&

dist.commands += \
tar -czf qxrd-$${VERSION}.tar.gz qxrd-$${VERSION} ; rm -rf qxrd-$${VERSION}

QMAKE_EXTRA_TARGETS += dox \
    rpmsource \
    mock \
    website
dox.target = docs
dox.commands += ( cat Doxyfile ; echo "PROJECT_NUMBER=$${VERSION}" ; echo "ALIASES = \"projectnumber=$${VERSION}\" ) | doxygen -
dox.depends = Doxyfile \
    $${SOURCES} \
    $${HEADERS}

rpmsource.depends += dist
rpmsource.commands += cp \
    qxrd-$${VERSION}.tar.gz \
    ~/rpmbuild/SOURCES/ ;

rpmsource.commands += perl -pi -e '\'s/Version:.*/Version: $${VERSION}/\'' qxrd.spec

mock.commands += mock-build qxrd.spec
mock.commands += && cp /var/lib/mock/*/result/qxrd*.rpm download
mock.depends = FORCE

website.commands = rsync \
    -e \
    ssh \
    -avx \
    dox/html/ \
    download \
    www12.xor.aps.anl.gov:/var/www/html/software/qxrd/qxrd-$${VERSION}/

website.commands += && rsync \
    -e \
    ssh \
    -avx \
    download \
    www12.xor.aps.anl.gov:/var/www/html/software/qxrd/download/

website.commands += && \
    ssh www12.xor.aps.anl.gov ln -s
