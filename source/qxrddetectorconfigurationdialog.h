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

  void appendProperty(int type,
                      QString name,
                      QString description,
                      QVariant value);

  QVariant propertyValue(int propType, int i);

protected:
  void changeEvent(QEvent *e);

private slots:
  void browseToDirectory(int i);

private:
  QxrdDetectorProxyPtr m_Proxy;
  int                  m_CurrentRow;
  QVector<QWidget*>    m_Widgets;
};

#endif // QXRDDETECTORCONFIGURATIONDIALOG_H
