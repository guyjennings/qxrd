#ifndef QXRDMASKSTACKVIEW_H
#define QXRDMASKSTACKVIEW_H

#include "qxrdlib_global.h"
#include <QTableView>
#include "qcepmaskstack-ptr.h"
#include "qcepmaskstackmodel-ptr.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdMaskStackView : public QTableView
{
  Q_OBJECT
public:
  explicit QxrdMaskStackView(QWidget *parent = 0);
  void setMaskStack(QcepMaskStackWPtr stk);
  void setProcessor(QxrdProcessorWPtr proc);
  void contextMenuEvent ( QContextMenuEvent * event );

signals:

public slots:

private:
  QcepMaskStackWPtr     m_MaskStack;
  QcepMaskStackModelPtr m_MaskStackModel;
  QxrdProcessorWPtr     m_Processor;
};

#endif // QXRDMASKSTACKVIEW_H
