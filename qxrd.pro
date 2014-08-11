greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

TEMPLATE = subdirs

SUBDIRS = plugins source
#SUBDIRS = plugins

CONFIG += ordered

include("qxrd.version.pri")
include("qxrd.platform.pri")

message(Version = $${VERSION})
message(Host Arch = $${QMAKE_HOST.arch})
message(Target = $${TARGET})
message(QT Version $$[QT_VERSION])

DISTFILES += source plugins

OTHER_FILES += qxrd.nsi qxrd-qt5.nsi qxrd.dox Doxyfile qxrd.spec qxrd.desktop readme.markdown

QMAKE_EXTRA_TARGETS += tarball

tarball.depends = FORCE $${TARGET}.spec
tarball.commands += \
        rm -rf $${TARGET}-$${VERSION} ; \
        $(MKDIR) $${TARGET}-$${VERSION} &&

tarball.commands += \
        $(COPY_FILE)  -r $${PWD}/* -t $${TARGET}-$${VERSION} && \
        $(COPY_FILE) $${TARGET}.spec -t $${TARGET}-$${VERSION} &&

tarball.commands += \
  tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}

QMAKE_EXTRA_TARGETS += dox

#dox.target = dox/html/index.html
dox.commands = ( cat $${PWD}/Doxyfile ; \
                 echo "PROJECT_NAME=\"QXRD\"" ; \
                 echo "PROJECT_NUMBER=$${VERSION}" ; \
                 echo "INPUT=\"$${PWD}\"" ; \
                 echo "INPUT+=\"$${PWD}\"/source/" ; \
                 echo "INPUT+=\"$${PWD}\"/source/submodules/" ; \
                 echo "INPUT+=\"$${PWD}\"/source/submodules/qceplib/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/qxrdareadetectorplugin/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/qxrdperkinelmerplugin/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/qxrdcudaprocessorplugin/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/qxrdpilatusplugin/" ; \
                 echo "INPUT+=\"$${PWD}\"/plugins/qxrdnidaqplugin/" ; \
                 echo "IMAGE_PATH+= \"$${PWD}\"/images \"$${PWD}\"/source/images" ; \
                 echo "IMAGE_PATH+= \"$${PWD}\"/source/help/images \"$${PWD}\"/qwt-5.2/doc/images" ; \
                 echo "ALIASES=\"projectnumber=$${VERSION}\"" \
               ) > Doxyfile.out ;
dox.commands += doxygen < Doxyfile.out -
dox.depends = FORCE $${PWD}/Doxyfile

QMAKE_EXTRA_TARGETS += upload-dox

upload-dox.commands = rsync -e ssh -av --del dox/html/ guyjennings,$${TARGET}@web.sourceforge.net:htdocs/

QMAKE_EXTRA_TARGETS += mock specfile

specfile.target = $${TARGET}.spec
specfile.depends = $${PWD}/$${TARGET}.spec.in $${PWD}/$${TARGET}.version.pri
specfile.commands = perl -p -e '\'s/Version:.*/Version: $${VERSION}/\'' $${PWD}/$${TARGET}.spec.in > $${TARGET}.spec

mock.depends = tarball $${TARGET}.spec
mock.commands += cp $${TARGET}-$${VERSION}.tar.gz ~/rpmbuild/SOURCES/ &&
mock.commands += mock-build $${TARGET}.spec

QMAKE_EXTRA_TARGETS += website

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

# rsync -avP -e ssh dox/html/ guyjennings,qxrd@web.sourceforge.net:htdocs/

#for(m, QT) {
#  message("QT contains $${m}")
#}
