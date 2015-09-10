#ifndef QXRDDETECTORCONFIGURATIONDIALOG_H
#define QXRDDETECTORCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ui_qxrddetectorconfigurationdialog.h"
#include "qxrddetectorproxy-ptr.h"

class QxrdDetectorConfigurationDialog : public QDialog, public Ui::QxrdDetectorConfigurationDialog
{
  Q_OBJECT

public:
  explicit QxrdDetectorConfigurationDialog(QxrdDetectorProxyPtr proxy, QWidget *parent = 0);
  virtual ~QxrdDetectorConfigurationDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdDetectorProxyPtr m_Proxy;
};

#endif // QXRDDETECTORCONFIGURATIONDIALOG_H
