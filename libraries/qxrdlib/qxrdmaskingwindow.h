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

private:
  void doHideMaskAll();
  void doShowMaskAll();
  void doHideMaskRange();
  void doShowMaskRange();
  void doInvertMask();
  void doGrowMask();
  void doShrinkMask();
  void doAndMask();
  void doOrMask();
  void doXorMask();
  void doAndNotMask();
  void doOrNotMask();
  void doXorNotMask();
  void doExchangeMask();
  void doRollMask();
  void doRollUpMask();
  void doRollDownMask();
  void doClearMask();
  void doClearMaskTop();
  void doNewMask();
  void doPushMask();
  void doUndoMask();
  void doZingersMask();

  int maskStackSelectPopup();

protected:
  void changeEvent(QEvent *e);

  QxrdMaskingWindowSettingsWPtr m_MaskingWindowSettings;
  QxrdMaskStackWPtr             m_MaskStack;
};

#endif // QXRDMASKINGWINDOW_H
