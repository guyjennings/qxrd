#ifndef QCEPAPPLICATION_H
#define QCEPAPPLICATION_H

#include <QApplication>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"

class QcepApplication : public QApplication
{
public:
  QcepApplication(int &argc, char **argv);

  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void setDefaultObjectData(QcepDataObject *obj) = 0;
};

extern QcepApplication *g_Application;

#endif // QCEPAPPLICATION_H
