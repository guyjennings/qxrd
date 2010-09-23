########################################################################
#  CUDA
########################################################################
win32 {
 INCLUDEPATH += $(CUDA_INC_DIR)
 QMAKE_LIBDIR += $(CUDA_LIB_DIR)
 LIBS += -lcudart

 cuda.output = $$OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.obj
 cuda.commands = $(CUDA_BIN_DIR)/nvcc.exe -c -Xcompiler $$join(QMAKE_CXXFLAGS,",") $$join(INCLUDEPATH,'" -I "','-I "','"') ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
}
unix {
 # auto-detect CUDA path
 CUDA_DIR = $$system(which nvcc | sed 's,/bin/nvcc$,,')
 INCLUDEPATH += $$CUDA_DIR/include
 QMAKE_LIBDIR += $$CUDA_DIR/lib $$CUDA_DIR/lib64
 LIBS += -lcudart

 cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.obj
 cuda.dependency_type = TYPE_C
 cuda.depend_command = nvcc -M -Xcompiler $$join(QMAKE_CXXFLAGS,",") $$join(INCLUDEPATH,'" -I "','-I "','"') ${QMAKE_FILE_NAME} | sed "s,^.*: ,," | sed "s,^ *,," | tr -d '\\\n'
 cuda.commands = nvcc -c -Xcompiler -fPIC -Xcompiler $$join(QMAKE_CXXFLAGS,",") $$join(INCLUDEPATH,'" -I "','-I "','"') ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
}
cuda.input = CUDA_SOURCES
QMAKE_EXTRA_COMPILERS += cuda

message(Object dir = $$OBJECTS_DIR or ${OBJECTS_DIR})
message(CUDA dir = $$CUDA_DIR)

########################################################################
