TEMPLATE = subdirs

SUBDIRS = plugins source
#SUBDIRS = plugins

CONFIG += ordered

include("qxrd.version.pri")
include("qxrd.platform.pri")

message(Version = $${VERSION})
message(Host Arch = $${QMAKE_HOST.arch})

DISTFILES += source plugins

OTHER_FILES += qxrd.nsi qxrd.dox Doxyfile

QMAKE_EXTRA_TARGETS += dist

dist.depends = FORCE
dist.commands += \
        rm -rf qxrd-$${VERSION} ; \
        $(MKDIR) qxrd-$${VERSION} && \
        $(MKDIR) qxrd-$${VERSION}/source && \
        $(MKDIR) qxrd-$${VERSION}/source/images && \
        $(MKDIR) qxrd-$${VERSION}/source/levmar-2.5 && \
        $(MKDIR) qxrd-$${VERSION}/source/help && \
        $(MKDIR) qxrd-$${VERSION}/source/help/images && \
        $(MKDIR) qxrd-$${VERSION}/qwt-5.2 && \
        $(MKDIR) qxrd-$${VERSION}/qwt-5.2/src && \
        $(MKDIR) qxrd-$${VERSION}/qceplib && \
        $(MKDIR) qxrd-$${VERSION}/tiffconfig && \
        $(MKDIR) qxrd-$${VERSION}/tiffconfig/macx && \
        $(MKDIR) qxrd-$${VERSION}/tiffconfig/win32 && \
        $(MKDIR) qxrd-$${VERSION}/tiff-3.8.2 && \
        $(MKDIR) qxrd-$${VERSION}/tiff-3.8.2/libtiff && \
        $(MKDIR) qxrd-$${VERSION}/plugins && \
        $(MKDIR) qxrd-$${VERSION}/plugins/qxrdareadetectorplugin && \
        $(MKDIR) qxrd-$${VERSION}/plugins/qxrdperkinelmerplugin && \
        $(MKDIR) qxrd-$${VERSION}/plugins/qxrdcudaprocessorplugin && \
        $(MKDIR) qxrd-$${VERSION}/plugins/qxrdpilatusplugin && \
        $(MKDIR) qxrd-$${VERSION}/plugins/qxrdnidaqplugin && \

dist.commands += \
        $(COPY_FILE)  $${PWD}/*.{pro,pri,spec} -t qxrd-$${VERSION} &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/source/images/*.{png,ico} \
                      -t qxrd-$${VERSION}/source/images &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/source/{[TA]*.h,q*.cpp,q*.h,q*.qrc,q*.rc,q*.ui,source.pro,*.pri} \
                      -t qxrd-$${VERSION}/source &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/source/levmar-2.5/*.{c,h} \
                      -t qxrd-$${VERSION}/source/levmar-2.5 &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/source/help/*.html \
                      -t qxrd-$${VERSION}/source/help &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/source/help/images/*.png \
                      -t qxrd-$${VERSION}/source/help/images &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/qwt-5.2/src/*.{cpp,h} \
                      -t qxrd-$${VERSION}/qwt-5.2/src &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/qceplib/*.{cpp,h} \
                      -t qxrd-$${VERSION}/qceplib &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/tiffconfig/macx/*.h \
                      -t qxrd-$${VERSION}/tiffconfig/macx &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/tiffconfig/win32/*.h \
                      -t qxrd-$${VERSION}/tiffconfig/win32 &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/tiff-3.8.2/libtiff/*.{c,h,def} \
                      -t qxrd-$${VERSION}/tiff-3.8.2/libtiff &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/plugins.pro \
                      -t qxrd-$${VERSION}/plugins &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdareadetectorplugin/q*.{cpp,h,pro} \
                      -t qxrd-$${VERSION}/plugins/qxrdareadetectorplugin &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdperkinelmerplugin/q*.{cpp,h,pro} \
                      -t qxrd-$${VERSION}/plugins/qxrdperkinelmerplugin &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdcudaprocessorplugin/q*.{cpp,h,pro} \
                      -t qxrd-$${VERSION}/plugins/qxrdcudaprocessorplugin &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdpilatusplugin/q*.{cpp,h,pro} \
                      -t qxrd-$${VERSION}/plugins/qxrdpilatusplugin &&
dist.commands += \
        $(COPY_FILE)  $${PWD}/plugins/qxrdnidaqplugin/q*.{cpp,h,pro} \
                      -t qxrd-$${VERSION}/plugins/qxrdnidaqplugin &&

dist.commands += \
  tar -czf qxrd-$${VERSION}.tar.gz qxrd-$${VERSION} ; # rm -rf qxrd-$${VERSION}

QMAKE_EXTRA_TARGETS += dox \
    rpmsource \
    mock \
    website

dox.target = dox/html/index.html
dox.commands = "("
dox.commands += cat $${PWD}/Doxyfile ;
dox.commands += echo "PROJECT_NUMBER=$${VERSION}" ;
dox.commands += echo "INPUT=\"$${PWD}\"" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/qceplib/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/source/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdareadetectorplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdperkinelmerplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdcudaprocessorplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdpilatusplugin/" ;
dox.commands += echo "INPUT+=\"$${PWD}\"/plugins/qxrdnidaqplugin/" ;
dox.commands += echo "IMAGE_PATH=  \"$${PWD}\"/images" ;
dox.commands += echo "IMAGE_PATH+= \"$${PWD}\"/images \"$${PWD}\"/source/images" ;
dox.commands += echo "IMAGE_PATH+= \"$${PWD}\"/source/help/images \"$${PWD}\"/qwt-5.2/doc/images" ;
dox.commands += echo "ALIASES=\"projectnumber=$${VERSION}\""
dox.commands += ")" > Doxyfile.out ;
dox.commands += doxygen < Doxyfile.out -
dox.depends = FORCE $${PWD}/Doxyfile \
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

# rsync -avP -e ssh dox/html/ guyjennings,qxrd@web.sourceforge.net:htdocs/
