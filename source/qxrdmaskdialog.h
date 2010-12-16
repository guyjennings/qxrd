#ifndef QXRDMASKDIALOG_H
#define QXRDMASKDIALOG_H

#include "qcepmacros.h"

#include <QWidget>
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdmaskstackmodel.h"

namespace Ui {
  class QxrdMaskDialog;
}

class QxrdMaskDialog : public QWidget
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
  Ui::QxrdMaskDialog  *ui;
  QxrdWindowPtr        m_Window;
  QxrdDataProcessorPtr m_Processor;
  QxrdMaskStackPtr     m_Masks;
  QxrdMaskStackModelPtr m_MaskStackModel;
};

#endif // QXRDMASKDIALOG_H
