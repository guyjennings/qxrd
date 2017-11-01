#ifndef QXRDINFODIALOG_H
#define QXRDINFODIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdinfodialog.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdinfodialogsettings-ptr.h"

class QXRD_EXPORT QxrdInfoDialog : public QDockWidget, public Ui::QxrdInfoDialog
{
  Q_OBJECT

public:
  explicit QxrdInfoDialog(QxrdInfoDialogSettingsWPtr settings, QWidget *parent);
  virtual ~QxrdInfoDialog();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);

private:
  QxrdInfoDialogSettingsWPtr m_InfoDialogSettings;
};

#endif // QXRDINFODIALOG_H