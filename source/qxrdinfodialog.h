#ifndef QXRDINFODIALOG_H
#define QXRDINFODIALOG_H

#include <QDockWidget>
#include "ui_qxrdinfodialog.h"
#include "qxrdsettings.h"
#include "qxrdimagedata.h"

class QxrdInfoDialog : public QDockWidget, public Ui::QxrdInfoDialog
{
  Q_OBJECT

public:
  explicit QxrdInfoDialog(QWidget *parent = 0);

  void readSettings(QSettings &settings, QString section);
  void writeSettings(QSettings &settings, QString section);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
};

#endif // QXRDINFODIALOG_H
