#include "qxrdcudaprocessorplugin.h"
#include <stdio.h>
//#include <cuda_runtime_api.h>

QxrdCudaProcessorPlugin::QxrdCudaProcessorPlugin()
{
//  printf("Cuda Processor Plugin\n");

//  int nDevices;

//  cudaGetDeviceCount(&nDevices);

//  printf("%d CUDA devices found\n", nDevices);
}

QString QxrdCudaProcessorPlugin::name() const
{
  return "CUDA Plugin";
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
Q_EXPORT_PLUGIN2(qxrdcudaprocessorplugin, QxrdCudaProcessorPlugin)
#endif
