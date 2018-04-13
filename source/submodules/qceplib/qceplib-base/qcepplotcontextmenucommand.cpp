#include "qcepplotcontextmenucommand.h"
#include <QAction>
#include <QObject>
#include "qcepimageplot.h"

QcepPlotContextMenuCommand::QcepPlotContextMenuCommand(QString                    name,
                                                       QString                    desc,
                                                       QcepPlotWidget            *plot,
                                                       QcepPlotWidgetSettingsWPtr set)
  : QcepPlotCommand(name, desc, plot, set)
{

}

QToolButton* QcepPlotContextMenuCommand::toolButton()
{
  return NULL;
}

QAction* QcepPlotContextMenuCommand::newContextAction(QString s, QcepPlotWidget *t, void (QcepPlotWidget::*sl)())
{
  QAction* res = new QAction(s);

  if (res && t && sl) {
    connect (res, &QAction::triggered, t, sl);
  }

  return res;
}
