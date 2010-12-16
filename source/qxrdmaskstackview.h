#ifndef QXRDMASKSTACKVIEW_H
#define QXRDMASKSTACKVIEW_H

#include <QTableView>
#include "qxrdmaskstack.h"
#include "qxrdmaskdialog.h"

class QxrdMaskStackView : public QTableView
{
  Q_OBJECT
public:
  explicit QxrdMaskStackView(QWidget *parent = 0);
  void setMaskStack(QxrdMaskStackPtr stk);
  void setMaskDialog(QxrdMaskDialog *dlg);
  void setProcessor(QxrdDataProcessor *proc);
  void contextMenuEvent ( QContextMenuEvent * event );

signals:

public slots:

private:
  QxrdMaskDialog    *m_Dialog;
  QxrdMaskStackPtr   m_MaskStack;
  QxrdDataProcessor *m_Processor;
};

#endif // QXRDMASKSTACKVIEW_H
