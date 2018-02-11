#ifndef QXRDGLOBALPREFERENCESDIALOG_H
#define QXRDGLOBALPREFERENCESDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include <QCheckBox>
#include "ui_qxrdglobalpreferencesdialog.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdplugininfomodel-ptr.h"

class QXRD_EXPORT QxrdGlobalPreferencesDialog : public QDialog, public Ui::QxrdGlobalPreferencesDialog
{
  Q_OBJECT

public:
  explicit QxrdGlobalPreferencesDialog(QxrdAppCommonWPtr       app,
                                       QxrdPluginInfoModelWPtr plugins);
  ~QxrdGlobalPreferencesDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  void setupDebugWidgets(qint64 dbg);
  qint64 readDebugWidgets();

private:
  QxrdAppCommonPtr         m_Application;
  QxrdPluginInfoModelPtr   m_PluginInfo;
  QVector<QCheckBox*>      m_DebugWidgetList;
};

#endif // QXRDGLOBALPREFERENCESDIALOG_H
