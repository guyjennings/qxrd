#ifndef QCEPAPPLICATION_H
#define QCEPAPPLICATION_H

#include "qceplib_global.h"
#include <QApplication>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"

class QCEP_EXPORT QcepApplication : public QApplication, public QEnableSharedFromThis<QcepApplication>
{
  Q_OBJECT

public:
  QcepApplication(int &argc, char **argv);
  void initialize();

  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void setDefaultObjectData(QcepDataObject *obj) = 0;
};

extern QCEP_EXPORT QcepApplication *g_Application;

#endif // QCEPAPPLICATION_H
