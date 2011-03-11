#ifndef QXRDMASKDIALOG_H
#define QXRDMASKDIALOG_H

#include <QDockWidget>
#include "ui_qxrdmaskdialog.h"

class QxrdWindow;
class QxrdDataProcessor;
class QxrdMaskStackModel;

class QxrdMaskDialog : public QDockWidget, public Ui::QxrdMaskDialog
{
  Q_OBJECT

public:
  explicit QxrdMaskDialog(QxrdWindow *win, QxrdDataProcessor *proc, QWidget *parent = 0);
  ~QxrdMaskDialog();

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

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
  QxrdWindow           *m_Window;
  QxrdDataProcessor    *m_Processor;
  QxrdMaskStackPtr      m_Masks;
  QxrdMaskStackModel   *m_MaskStackModel;
};

#endif // QXRDMASKDIALOG_H
