#ifndef QCEPAPPLICATION_H
#define QCEPAPPLICATION_H

#include "qceplib_global.h"
#include <QApplication>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"
#include "qcepobject.h"
#include "qapplication-ptr.h"

class QCEP_EXPORT QcepApplication : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  explicit QcepApplication(int &argc, char **argv);
  void initializeRoot();

  virtual void setDefaultObjectData(QcepDataObject *obj) = 0;

  int exec();
  void processEvents() const;
  void exit();
  void quit();

public slots:
  void openObjectBrowserWindow(QcepObjectWPtr obj);

protected:
  QApplicationPtr m_Application;
};

extern QCEP_EXPORT QcepApplication *g_Application;

#endif // QCEPAPPLICATION_H
