#ifndef QXRDMASKSTACKVIEW_H
#define QXRDMASKSTACKVIEW_H

#include "qxrdlib_global.h"
#include <QTableView>
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskstackmodel-ptr.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdMaskStackView : public QTableView
{
  Q_OBJECT
public:
  explicit QxrdMaskStackView(QWidget *parent = 0);
  void setMaskStack(QxrdMaskStackWPtr stk);
  void setProcessor(QxrdProcessorWPtr proc);
  void contextMenuEvent ( QContextMenuEvent * event );

signals:

public slots:

private:
  QxrdMaskStackWPtr     m_MaskStack;
  QxrdMaskStackModelPtr m_MaskStackModel;
  QxrdProcessorWPtr     m_Processor;
};

#endif // QXRDMASKSTACKVIEW_H
