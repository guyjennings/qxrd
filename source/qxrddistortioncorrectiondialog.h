#ifndef QXRDDISTORTIONCORRECTIONDIALOG_H
#define QXRDDISTORTIONCORRECTIONDIALOG_H

#include <QDockWidget>
#include "qxrddistortioncorrection-ptr.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddistortioncorrectiondialogsettings.h"

class QxrdDistortionCorrectionDialog : public QDockWidget, public Ui::QxrdDistortionCorrectionDialog
{
  Q_OBJECT

public:
  explicit QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QWidget *parent = 0);
  virtual ~QxrdDistortionCorrectionDialog();
  void init(QxrdDistortionCorrectionDialogSettingsWPtr settings);

public slots:
  void updateGridTable();

private slots:

private:
  QxrdDistortionCorrectionPtr         m_DistortionCorrection;
};

#endif // QXRDDISTORTIONCORRECTIONDIALOG_H
