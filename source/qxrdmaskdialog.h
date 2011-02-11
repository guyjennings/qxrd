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

  void newMask();
  void enableMasks();
  void deleteMasks();
  void andMasks();
  void orMasks();
  void thresholdMasks();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWindowPtr        m_Window;
  QxrdDataProcessorPtr m_Processor;
  QxrdMaskStackPtr     m_Masks;
  QxrdMaskStackModelPtr m_MaskStackModel;
};

#endif // QXRDMASKDIALOG_H
