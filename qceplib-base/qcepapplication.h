#ifndef QCEPAPPLICATION_H
#define QCEPAPPLICATION_H

#include "qceplib_global.h"
#include <QApplication>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"
#include "qcepobject.h"
#include "qnewapplication-ptr.h"

class QCEP_EXPORT QcepApplication : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  explicit QcepApplication(int &argc, char **argv);
  void initialize(QObjectWPtr parent);

//  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
//  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void setDefaultObjectData(QcepDataObject *obj) = 0;

  int exec();
  void processEvents();
  void exit();
  void quit();

public slots:
  void openObjectBrowserWindow(QcepObjectWPtr obj);

protected:
  QNewApplicationPtr m_Application;
};

extern QCEP_EXPORT QcepApplication *g_Application;

#endif // QCEPAPPLICATION_H
