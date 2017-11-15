#ifndef QXRDPLOTCONTEXTMENUCOMMAND_H
#define QXRDPLOTCONTEXTMENUCOMMAND_H

#include "qxrdplotcommand.h"
#include <QObject>
#include "qxrdimageplot.h"

class QxrdPlotContextMenuCommand : public QxrdPlotCommand
{
public:
  QxrdPlotContextMenuCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
  QAction* contextMenuAction(const QPoint &pos) = 0;
  bool     contextMenuSeparator();

protected:
  QAction* newContextAction(QString s, QxrdImagePlot *t, void (QxrdImagePlot::*sl)());
};

#endif // QXRDPLOTCONTEXTMENUCOMMAND_H
