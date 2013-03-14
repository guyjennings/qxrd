TEMPLATE = subdirs

SUBDIRS = plugins source
#SUBDIRS = plugins

CONFIG += ordered

include("qxrd.version.pri")
include("qxrd.platform.pri")

message(Version = $${VERSION})
message(Host Arch = $${QMAKE_HOST.arch})
message(Target = $${TARGET})

DISTFILES += source plugins

OTHER_FILES += qxrd.nsi qxrd.dox Doxyfile qxrd.spec qxrd.desktop readme.markdown

QMAKE_EXTRA_TARGETS += tarball

tarball.depends = FORCE qxrd.spec
tarball.commands += \
        rm -rf $${TARGET}-$${VERSION} ; \
        $(MKDIR) $${TARGET}-$${VERSION} && \
        $(MKDIR) $${TARGET}-$${VERSION}/source && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/images && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/levmar-2.5 && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/help && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/help/images && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/qwt-5.2 && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/qwt-5.2/src && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/tiffconfig && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/tiffconfig/macx && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/tiffconfig/win32 && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/tiff-3.8.2 && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/tiff-3.8.2/libtiff && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/submodules && \
        $(MKDIR) $${TARGET}-$${VERSION}/source/submodules/qceplib && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins/qxrdareadetectorplugin && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins/qxrdperkinelmerplugin && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins/qxrdcudaprocessorplugin && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins/qxrdpilatusplugin && \
        $(MKDIR) $${TARGET}-$${VERSION}/plugins/qxrdnidaqplugin && \

tarball.commands += \
        $(COPY_FILE)  $${PWD}/*.{pro,pri,spec,desktop,markdown} -t $${TARGET}-$${VERSION} &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/images/*.{png,ico,icns} \
                      -t $${TARGET}-$${VERSION}/source/images &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/{*.cpp,*.h,*.qrc,*.rc,*.ui,source.pro,*.pri,*.js} \
                      -t $${TARGET}-$${VERSION}/source &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/levmar-2.5/*.{c,h} \
                      -t $${TARGET}-$${VERSION}/source/levmar-2.5 &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/help/*.html \
                      -t $${TARGET}-$${VERSION}/source/help &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/help/images/*.png \
                      -t $${TARGET}-$${VERSION}/source/help/images &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/qwt-5.2/src/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/source/qwt-5.2/src &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/tiffconfig/macx/*.h \
                      -t $${TARGET}-$${VERSION}/source/tiffconfig/macx &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/tiffconfig/win32/*.h \
                      -t $${TARGET}-$${VERSION}/source/tiffconfig/win32 &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/tiff-3.8.2/libtiff/*.{c,h,def} \
                      -t $${TARGET}-$${VERSION}/source/tiff-3.8.2/libtiff &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/source/submodules/qceplib/*.{cpp,h} \
                      -t $${TARGET}-$${VERSION}/source/submodules/qceplib &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/plugins.pro \
                      -t $${TARGET}-$${VERSION}/plugins &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdareadetectorplugin/q*.{cpp,h,pro} \
                      -t $${TARGET}-$${VERSION}/plugins/qxrdareadetectorplugin &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdperkinelmerplugin/q*.{cpp,h,pro} \
                      -t $${TARGET}-$${VERSION}/plugins/qxrdperkinelmerplugin &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdcudaprocessorplugin/q*.{cpp,h,pro} \
                      -t $${TARGET}-$${VERSION}/plugins/qxrdcudaprocessorplugin &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdpilatusplugin/q*.{cpp,h,pro} \
                      -t $${TARGET}-$${VERSION}/plugins/qxrdpilatusplugin &&
tarball.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdnidaqplugin/q*.{cpp,h,pro} \
                      -t $${TARGET}-$${VERSION}/plugins/qxrdnidaqplugin &&
tarball.commands += \
        $(COPY_FILE) qxrd.spec -t $${TARGET}-$${VERSION}/ &&

tarball.commands += \
  tar -czf $${TARGET}-$${VERSION}.tar.gz $${TARGET}-$${VERSION} ; rm -rf $${TARGET}-$${VERSION}

QMAKE_EXTRA_TARGETS += dox

#dox.target = dox/html/index.html
dox.commands = "("
dox.commands += cat $${PWD}/Doxyfile ;
dox.commands += echo "PROJECT_NAME=\"QXRD\"" ;
dox.commands += echo "PROJECT_NUMBER=$${VERSION}" ;
dox.commands += echo "INPUT=\"$${PWD}\"" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/source/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/source/submodules/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/source/submodules/qceplib/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdareadetectorplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdperkinelmerplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdcudaprocessorplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdpilatusplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdnidaqplugin/" ;
dox.commands += echo "IMAGE_PATH+= \"$${PWD}\"/images \"$${PWD}\"/source/images" ;
dox.commands += echo "IMAGE_PATH+= \"$${PWD}\"/source/help/images \"$${PWD}\"/qwt-5.2/doc/images" ;
dox.commands += echo "ALIASES=\"projectnumber=$${VERSION}\""
dox.commands += ")" > Doxyfile.out ;
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
