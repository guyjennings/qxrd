CUDA_ARCH = sm_13

win32:exists(C:/CUDA/include/cuda.h) {
    DEFINES += HAVE_CUDA
    contains(QMAKE_HOST.arch,x86_64) { 
        INCLUDEPATH += C:/CUDA/include/
        QMAKE_LIBDIR += C:/CUDA/lib64/cuda.lib
    }
    else { 
        INCLUDEPATH += C:/CUDA/include/
        QMAKE_LIBDIR += C:/CUDA/lib/cuda.lib
    }
    LIBS += -lcudart \
        -lcuda
    cuda.output = $$OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.obj
    cuda.commands = $(CUDA_BIN_DIR)/nvcc.exe \
        -c \
        -arch=$$CUDA_ARCH \
        -Xcompiler \
        $$join(QMAKE_CXXFLAGS,",") \
        $$join(INCLUDEPATH,'" -I"','-I "','"') \
        ${QMAKE_FILE_NAME} \
        -o \
        ${QMAKE_FILE_OUT}
}
unix { 
    CUDA_DIR = $$system(which nvcc | sed 's,/bin/nvcc$,,')
    exists(/usr/include/cuda/cuda.h) { 
        DEFINES += HAVE_CUDA
        INCLUDEPATH += $$CUDA_DIR/include
        contains(QMAKE_HOST.arch,x86_64) {
            QMAKE_LIBDIR += $$CUDA_DIR/lib64
        } else {
            QMAKE_LIBDIR += $$CUDA_DIR/lib
        }
        LIBS += -lcudart \
            -lcuda
    }
    cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.obj
    cuda.commands = nvcc \
        -c \
        -arch=$$CUDA_ARCH \
        -Xcompiler \
        $$join(QMAKE_CXXFLAGS,",") \
        $$join(INCLUDEPATH,'" -I "','-I "','"') \
        ${QMAKE_FILE_NAME} \
        -o \
        ${QMAKE_FILE_OUT}
    cuda.dependency_type = TYPE_C
    cuda.depend_command = nvcc \
        -M \
        -Xcompiler \
        $$join(QMAKE_CXXFLAGS,",") \
        $$join(INCLUDEPATH,'" -I "','-I "','"') \
        ${QMAKE_FILE_NAME} \
        | \
        sed \
        "s,^.*: ,," \
        | \
        sed \
        "s,^ *,," \
        | \
        tr \
        -d \
        '\\\n'
}
contains (DEFINES,HAVE_CUDA):message("CUDA found in $${CUDA_DIR}")
cuda.input = CUDA_SOURCES
QMAKE_EXTRA_UNIX_COMPILERS += cuda

CUDA_SOURCES += qxrddataprocessorcudakernel.cu
