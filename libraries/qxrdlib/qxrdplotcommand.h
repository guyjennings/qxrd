#ifndef QXRDPLOTCOMMAND_H
#define QXRDPLOTCOMMAND_H

#include "qcepserializableobject.h"
#include "qxrdimageplot-ptr.h"
#include "qxrdlib_global.h"

class QXRD_EXPORT QxrdPlotCommand : public QcepSerializableObject
{
public:
  QxrdPlotCommand(QString name, QxrdImagePlot* plot);

  virtual QToolButton *toolButton() = 0;
  virtual QAction     *contextMenuAction(const QPoint &pos) = 0;
  virtual bool         contextMenuSeparator() = 0;

protected:
  QxrdImagePlot *m_Plot;
};

#endif // QXRDPLOTCOMMAND_H
