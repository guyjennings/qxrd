#ifndef QXRDSTARTUPWINDOW_H
#define QXRDSTARTUPWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdstartupwindow.h"
#include <QDateTime>
#include "qxrdappcommon-ptr.h"
#include "qxrdstartupwindowsettings-ptr.h"

class QXRD_EXPORT QxrdStartupWindow :
    public QxrdMainWindow,
    public Ui::QxrdStartupWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdStartupWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdStartupWindow();

  void setApplicationIcon        (QIcon     icon);
  void setApplicationDescription (QString   desc);

public slots:
  void appendSplashMessage       (QString   msg,
                                  QDateTime dt=QDateTime::currentDateTime());
  void appendMessage             (QString   msg,
                                  QString color,
                                  QDateTime dt=QDateTime::currentDateTime());

  void onMessageWindowLinesChanged(int newVal);

private:
  QDateTime                     m_LastDateTime;
};

#endif // QXRDSTARTUPWINDOW_H
