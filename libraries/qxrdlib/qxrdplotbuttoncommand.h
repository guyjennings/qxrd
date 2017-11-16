#ifndef QXRDPLOTBUTTONCOMMAND_H
#define QXRDPLOTBUTTONCOMMAND_H

#include "qxrdplotcommand.h"

class QXRD_EXPORT QxrdPlotButtonCommand : public QxrdPlotCommand
{
public:
  QxrdPlotButtonCommand(QString name, QxrdPlotWidget *plot);

  QAction *contextMenuAction(const QPoint &pos);
};

#endif // QXRDPLOTBUTTONCOMMAND_H
