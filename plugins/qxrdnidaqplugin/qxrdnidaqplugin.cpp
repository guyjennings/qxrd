#include "qxrdnidaqplugin.h"
#include "qxrdnidaqsynchronizer.h"
#include "qxrdnidaqsynchronizer-ptr.h"
#include "qxrdnidaqplugin-ptr.h"

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  inherited("nidaqPlugin")
{
}

QxrdNIDAQPlugin::~QxrdNIDAQPlugin()
{
}

void QxrdNIDAQPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdNIDAQPlugin*>("QxrdNIDAQPlugin*");
  qRegisterMetaType<QxrdNIDAQSynchronizer*>("QxrdNIDAQSynchronizer*");
}

QString QxrdNIDAQPlugin::name() const
{
  return "NIDAQ Plugin";
}

QxrdSynchronizerPtr QxrdNIDAQPlugin::createSynchronizer(QString name)
{
//  QxrdNIDAQPluginPtr plugin(
//        qSharedPointerDynamicCast<QxrdNIDAQPlugin>(sharedFromThis()));

  QxrdSynchronizerPtr res =
      QxrdSynchronizerPtr(new QxrdNIDAQSynchronizer(name));

  return res;
}
