#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qxrdsettings.h"
#include "qxrdimagedata.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QWidget *parent = 0);

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
};

#endif // QXRDHISTOGRAMDIALOG_H
