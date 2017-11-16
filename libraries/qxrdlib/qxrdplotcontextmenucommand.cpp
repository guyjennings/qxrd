#include "qxrdplotcontextmenucommand.h"
#include <QAction>
#include <QObject>
#include "qxrdimageplot.h"

QxrdPlotContextMenuCommand::QxrdPlotContextMenuCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotCommand(name, plot)
{

}

QToolButton* QxrdPlotContextMenuCommand::toolButton()
{
  return NULL;
}

QAction* QxrdPlotContextMenuCommand::newContextAction(QString s, QxrdImagePlot *t, void (QxrdImagePlot::*sl)())
{
  QAction* res = new QAction(s);

  if (res && t && sl) {
    connect (res, &QAction::triggered, t, sl);
  }

  return res;
}
