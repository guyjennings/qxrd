/******************************************************************
*
*  $Id: qxrdcudaprocessorplugin.cpp,v 1.3 2010/09/23 15:30:52 jennings Exp $
*
*******************************************************************/

#include "qxrdcudaprocessorplugin.h"
#include <stdio.h>
#include <cuda_runtime_api.h>

QxrdCudaProcessorPlugin::QxrdCudaProcessorPlugin()
{
  printf("Cuda Processor Plugin\n");

  int nDevices;

  cudaGetDeviceCount(&nDevices);

  printf("%d CUDA devices found\n", nDevices);
}

QString QxrdCudaProcessorPlugin::name() const
{
  return "CUDA Plugin";
}

Q_EXPORT_PLUGIN2(qxrdcudaprocessorplugin, QxrdCudaProcessorPlugin);

/******************************************************************
*
*  $Log: qxrdcudaprocessorplugin.cpp,v $
*  Revision 1.3  2010/09/23 15:30:52  jennings
*  Beginning CUDA support
*
*  Revision 1.2  2010/09/13 20:00:13  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
