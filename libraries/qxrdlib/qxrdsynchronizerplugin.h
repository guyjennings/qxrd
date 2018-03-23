#ifndef QXRDSYNCHRONIZERPLUGIN_H
#define QXRDSYNCHRONIZERPLUGIN_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdsynchronizer-ptr.h"

class QXRD_EXPORT QxrdSynchronizerPlugin : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdSynchronizerPlugin(QString name);

  virtual void registerMetaTypes() = 0;

  virtual QxrdSynchronizerPtr createSynchronizer(QString name) = 0;

  virtual QString name() const = 0;
};

#define SynchronizerInterface_iid "gov.anl.aps.cep.Qxrd.Synchronizer"
Q_DECLARE_INTERFACE(QxrdSynchronizerPlugin, SynchronizerInterface_iid)

#endif // QXRDSYNCHRONIZERPLUGIN_H
