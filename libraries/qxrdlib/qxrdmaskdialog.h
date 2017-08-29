#ifndef QXRDMASKDIALOG_H
#define QXRDMASKDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdmaskdialog.h"

#include "qxrddataprocessor-ptr.h"
#include "qxrdmaskstackmodel-ptr.h"
#include "qxrdmaskstack-ptr.h"

class QXRD_EXPORT QxrdMaskDialog : public QDockWidget, public Ui::QxrdMaskDialog
{
  Q_OBJECT

public:
  explicit QxrdMaskDialog(QxrdDataProcessorWPtr procw, QWidget *parent = 0);
  ~QxrdMaskDialog();

protected:
  void changeEvent(QEvent *e);

private slots:
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

private:
  int maskStackSelectPopup();

private:
  QxrdDataProcessorWPtr  m_Processor;
  QxrdMaskStackWPtr      m_Masks;
  QxrdMaskStackModelPtr  m_MaskStackModel;
};

#endif // QXRDMASKDIALOG_H
