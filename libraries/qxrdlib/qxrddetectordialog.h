#ifndef QXRDDETECTORDIALOG_H
#define QXRDDETECTORDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>

class QXRD_EXPORT QxrdDetectorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit QxrdDetectorDialog(QWidget *parent = nullptr);

  void accept();

signals:

public slots:
};

#endif // QXRDDETECTORDIALOG_H
