#ifndef QXRDPLOTBUTTONCOMMAND_H
#define QXRDPLOTBUTTONCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdPlotButtonCommand : public QxrdPlotCommand
{
public:
  QxrdPlotButtonCommand(QString name, QxrdImagePlot *plot);

  QAction *contextMenuAction(const QPoint &pos);
  bool     contextMenuSeparator();
};

#endif // QXRDPLOTBUTTONCOMMAND_H
