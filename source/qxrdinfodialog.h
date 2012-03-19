#ifndef QXRDINFODIALOG_H
#define QXRDINFODIALOG_H

#include <QDockWidget>
#include "ui_qxrdinfodialog.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata-ptr.h"
#include "qxrdsettingssaver-ptr.h"
#include <QSettings>

class QxrdInfoDialog : public QDockWidget, public Ui::QxrdInfoDialog
{
  Q_OBJECT

public:
  explicit QxrdInfoDialog(QWidget *parent);

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

private:
  QxrdSettingsSaverWPtr m_Saver;
};

#endif // QXRDINFODIALOG_H
