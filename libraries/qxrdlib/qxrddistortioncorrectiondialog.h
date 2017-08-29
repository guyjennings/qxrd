#ifndef QXRDDISTORTIONCORRECTIONDIALOG_H
#define QXRDDISTORTIONCORRECTIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "qxrddistortioncorrection-ptr.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddistortioncorrectiondialogsettings-ptr.h"
#include "qxrdwindow-ptr.h"

class QXRD_EXPORT QxrdDistortionCorrectionDialog : public QDockWidget, public Ui::QxrdDistortionCorrectionDialog
{
  Q_OBJECT

public:
  explicit QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QxrdWindowPtr parent);
  virtual ~QxrdDistortionCorrectionDialog();
  void init(QxrdDistortionCorrectionDialogSettingsWPtr settings);

public slots:
  void updateGridTable();
  void updateDistortionGraph();

private slots:

private:
  QxrdWindowWPtr                      m_Window;
  QxrdDistortionCorrectionWPtr        m_DistortionCorrection;
};

#endif // QXRDDISTORTIONCORRECTIONDIALOG_H
