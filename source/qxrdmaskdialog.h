#ifndef QXRDMASKDIALOG_H
#define QXRDMASKDIALOG_H

#include "qcepmacros.h"

#include <QWidget>
#include "qxrddataprocessor.h"
#include "qxrdwindow.h"
#include "qxrdmasklistmodel.h"

namespace Ui {
  class QxrdMaskDialog;
}

class QxrdMaskDialog : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdMaskDialog(QxrdWindowPtr win, QxrdDataProcessorPtr proc, QWidget *parent = 0);
  ~QxrdMaskDialog();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdMaskDialog  *ui;
  QxrdWindowPtr        m_Window;
  QxrdDataProcessorPtr m_Processor;
  QxrdMaskListModelPtr m_MaskListModel;
};

#endif // QXRDMASKDIALOG_H
