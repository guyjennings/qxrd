#ifndef QXRDPLOTCONTEXTMENUCOMMAND_H
#define QXRDPLOTCONTEXTMENUCOMMAND_H

#include "qxrdplotcommand.h"
#include <QObject>
#include "qxrdplotwidget.h"

class QXRD_EXPORT QxrdPlotContextMenuCommand : public QxrdPlotCommand
{
public:
  QxrdPlotContextMenuCommand(QString                    name,
                             QxrdPlotWidget            *plot,
                             QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
  QAction* contextMenuAction(const QPoint &pos) = 0;

protected:
  QAction* newContextAction(QString s, QxrdPlotWidget *t, void (QxrdPlotWidget::*sl)());
};

#endif // QXRDPLOTCONTEXTMENUCOMMAND_H
