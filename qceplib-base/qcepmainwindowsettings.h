#ifndef QCEPMAINWINDOWSETTINGS_H
#define QCEPMAINWINDOWSETTINGS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepmainwindow-ptr.h"

class QCEP_EXPORT QcepMainWindowSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepMainWindowSettings(QString name, QString desc);
  void initialize(QcepObjectWPtr parent);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  virtual QcepMainWindowPtr newWindow() = 0;

  QcepMainWindowPtr  window();

  void openWindow();
  void closeWindow();

protected:
  QcepMainWindowPtr   m_Window;

public:
  Q_PROPERTY(QString description READ get_Description WRITE set_Description STORED false)
  QCEP_STRING_PROPERTY(Description)

  Q_PROPERTY(int windowOpen READ get_WindowOpen WRITE set_WindowOpen)
  QCEP_INTEGER_PROPERTY(WindowOpen)

  Q_PROPERTY(QRectF windowRect READ get_WindowRect WRITE set_WindowRect)
  QCEP_DOUBLE_RECT_PROPERTY(WindowRect)
};

Q_DECLARE_METATYPE(QcepMainWindowSettings*)

#endif // QCEPMAINWINDOWSETTINGS_H
