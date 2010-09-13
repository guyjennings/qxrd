/******************************************************************
*
*  $Id: qxrdcudaprocessorplugin.cpp,v 1.2 2010/09/13 20:00:13 jennings Exp $
*
*******************************************************************/

#include "qxrdcudaprocessorplugin.h"

QxrdCudaProcessorPlugin::QxrdCudaProcessorPlugin()
{
}

QString QxrdCudaProcessorPlugin::name() const
{
  return "CUDA Plugin";
}

Q_EXPORT_PLUGIN2(qxrdcudaprocessorplugin, QxrdCudaProcessorPlugin);

/******************************************************************
*
*  $Log: qxrdcudaprocessorplugin.cpp,v $
*  Revision 1.2  2010/09/13 20:00:13  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
