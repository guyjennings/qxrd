#include "qxrdplotcontextmenucommand.h"
#include <QAction>
#include <QObject>
#include "qxrdimageplot.h"

QxrdPlotContextMenuCommand::QxrdPlotContextMenuCommand(QString                    name,
                                                       QString                    desc,
                                                       QxrdPlotWidget            *plot,
                                                       QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotCommand(name, desc, plot, set)
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
