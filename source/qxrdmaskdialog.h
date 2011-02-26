#ifndef QXRDMASKDIALOG_H
#define QXRDMASKDIALOG_H

#include "qcepmacros.h"

#include <QDockWidget>
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdmaskstackmodel.h"
#include "ui_qxrdmaskdialog.h"

class QxrdMaskDialog : public QDockWidget, public Ui::QxrdMaskDialog
{
  Q_OBJECT

public:
  explicit QxrdMaskDialog(QxrdWindowPtr win, QxrdDataProcessorPtr proc, QWidget *parent = 0);
  ~QxrdMaskDialog();

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void changeEvent(QEvent *e);

private slots:
  void doHideMaskAll();
  void doShowMaskAll();
  void doHideMaskRange();
  void doShowMaskRange();
  void doInvertMask();
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

private:
  int maskStackSelectPopup();

private:
  QxrdWindowPtr        m_Window;
  QxrdDataProcessorPtr m_Processor;
  QxrdMaskStackPtr     m_Masks;
  QxrdMaskStackModelPtr m_MaskStackModel;
};

#endif // QXRDMASKDIALOG_H
