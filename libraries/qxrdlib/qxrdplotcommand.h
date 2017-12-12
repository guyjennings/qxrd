#ifndef QXRDPLOTCOMMAND_H
#define QXRDPLOTCOMMAND_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdplotwidget-ptr.h"
#include "qxrdimageplot-ptr.h"

class QXRD_EXPORT QxrdPlotCommand : public QcepObject
{
public:
  QxrdPlotCommand(QString name, QxrdPlotWidget* plot);

  virtual QToolButton *toolButton() = 0;
  virtual QAction     *contextMenuAction(const QPoint &pos) = 0;

  virtual void         enable();
  virtual void         disable();

protected:
  QxrdPlotWidget      *m_PlotWidget;
  QxrdImagePlot       *m_Plot;
};

#endif // QXRDPLOTCOMMAND_H