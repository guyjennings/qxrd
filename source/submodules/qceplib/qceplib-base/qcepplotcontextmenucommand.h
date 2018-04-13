#ifndef QCEPPLOTCONTEXTMENUCOMMAND_H
#define QCEPPLOTCONTEXTMENUCOMMAND_H

#include "qcepplotcommand.h"
#include <QObject>
#include "qcepplotwidget.h"

class QCEP_EXPORT QcepPlotContextMenuCommand : public QcepPlotCommand
{
  Q_OBJECT

public:
  QcepPlotContextMenuCommand(QString                    name,
                             QString                    desc,
                             QcepPlotWidget            *plot,
                             QcepPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
  QAction* contextMenuAction(const QPoint &pos) = 0;

protected:
  QAction* newContextAction(QString s, QcepPlotWidget *t, void (QcepPlotWidget::*sl)());
};

#endif // QCEPPLOTCONTEXTMENUCOMMAND_H
