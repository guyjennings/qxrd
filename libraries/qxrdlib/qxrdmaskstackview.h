#ifndef QXRDMASKSTACKVIEW_H
#define QXRDMASKSTACKVIEW_H

#include "qxrdlib_global.h"
#include <QTableView>
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskstackmodel-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdmaskdialog-ptr.h"

class QXRD_EXPORT QxrdMaskStackView : public QTableView
{
  Q_OBJECT
public:
  explicit QxrdMaskStackView(QWidget *parent = 0);
  void setMaskStack(QxrdMaskStackWPtr stk);
  void setMaskDialog(QxrdMaskDialog *dlg);
  void setProcessor(QxrdProcessorWPtr proc);
  void contextMenuEvent ( QContextMenuEvent * event );

signals:

public slots:

private:
  QxrdMaskDialog       *m_Dialog;
  QxrdMaskStackWPtr     m_MaskStack;
  QxrdMaskStackModelPtr m_MaskStackModel;
  QxrdProcessorWPtr     m_Processor;
};

#endif // QXRDMASKSTACKVIEW_H
