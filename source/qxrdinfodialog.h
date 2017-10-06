#ifndef QXRDINFODIALOG_H
#define QXRDINFODIALOG_H

#include <QDockWidget>
#include "ui_qxrdinfodialog.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata-ptr.h"
#include "qxrdinfodialogsettings.h"

class QxrdInfoDialog : public QDockWidget, public Ui::QxrdInfoDialog
{
  Q_OBJECT

public:
  explicit QxrdInfoDialog(QxrdInfoDialogSettingsWPtr settings, QWidget *parent);
  virtual ~QxrdInfoDialog();

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image);

private:
  QxrdInfoDialogSettingsWPtr m_InfoDialogSettings;
};

#endif // QXRDINFODIALOG_H
