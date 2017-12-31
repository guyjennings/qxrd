#include "qxrdplotcontextmenucommand.h"
#include <QAction>
#include <QObject>
#include "qxrdimageplot.h"

QxrdPlotContextMenuCommand::QxrdPlotContextMenuCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotCommand(name, plot, set)
{

}

QToolButton* QxrdPlotContextMenuCommand::toolButton()
{
  return NULL;
}

QAction* QxrdPlotContextMenuCommand::newContextAction(QString s, QxrdPlotWidget *t, void (QxrdPlotWidget::*sl)())
{
  QAction* res = new QAction(s);

  if (res && t && sl) {
    connect (res, &QAction::triggered, t, sl);
  }

  return res;
}
