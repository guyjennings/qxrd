#ifndef QXRDMASKINGWINDOW_H
#define QXRDMASKINGWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdmaskingwindow.h"
#include "qxrdmaskingwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdMaskingWindow : public QxrdMainWindow, public Ui::QxrdMaskingWindow
{
  Q_OBJECT

public:
  explicit QxrdMaskingWindow(QxrdMaskingWindowSettingsWPtr set,
                             QString name,
                             QxrdApplicationWPtr app,
                             QxrdExperimentWPtr expt,
                             QxrdAcquisitionWPtr acqw,
                             QxrdProcessorWPtr procw);
  ~QxrdMaskingWindow();

protected:
  void changeEvent(QEvent *e);

  QxrdMaskingWindowSettingsWPtr m_MaskingWindowSettings;
};

#endif // QXRDMASKINGWINDOW_H
