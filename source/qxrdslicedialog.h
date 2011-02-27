#ifndef QXRDSLICEDIALOG_H
#define QXRDSLICEDIALOG_H

#include <QDockWidget>
#include "ui_qxrdslicedialog.h"
#include "qxrdsettings.h"
#include "qxrdimagedata.h"

class QxrdSliceDialog : public QDockWidget, public Ui::QxrdSliceDialog
{
  Q_OBJECT

public:
  explicit QxrdSliceDialog(QWidget *parent = 0);

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
};

#endif // QXRDSLICEDIALOG_H
