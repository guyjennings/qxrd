#ifndef QXRDNIDAQPLUGIN_H
#define QXRDNIDAQPLUGIN_H

#include "qcepobject.h"
#include "qxrdsynchronizerplugin.h"

class QxrdNIDAQPlugin :
    public QxrdSynchronizerPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SynchronizerInterface_iid FILE "nidaq.json")
  Q_INTERFACES(QxrdSynchronizerPlugin)

private:
  typedef QxrdSynchronizerPlugin inherited;

public:
  QxrdNIDAQPlugin();
  virtual ~QxrdNIDAQPlugin();

  void registerMetaTypes();

  QxrdSynchronizerPtr createSynchronizer(QString name);

  QString name() const;
};

Q_DECLARE_METATYPE(QxrdNIDAQPlugin*)

#endif // QXRDNIDAQPLUGIN_H
