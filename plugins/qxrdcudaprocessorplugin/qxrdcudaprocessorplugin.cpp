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

Q_EXPORT_PLUGIN2(qxrdcudaprocessorplugin, QxrdCudaProcessorPlugin);
