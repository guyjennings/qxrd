#ifndef QXRDMASKSTACKVIEW_H
#define QXRDMASKSTACKVIEW_H

#include <QTableView>
#include "qxrdmaskstack-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdMaskDialog;

class QxrdMaskStackView : public QTableView
{
  Q_OBJECT
public:
  explicit QxrdMaskStackView(QWidget *parent = 0);
  void setMaskStack(QxrdMaskStackPtr stk);
  void setMaskDialog(QxrdMaskDialog *dlg);
  void setProcessor(QxrdDataProcessorWPtr proc);
  void contextMenuEvent ( QContextMenuEvent * event );

signals:

public slots:

private:
  QxrdMaskDialog      *m_Dialog;
  QxrdMaskStackPtr     m_MaskStack;
  QxrdDataProcessorWPtr m_Processor;
};

#endif // QXRDMASKSTACKVIEW_H
