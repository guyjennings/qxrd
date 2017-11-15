#ifndef QXRDPOWDERPOINTSCOMMAND_H
#define QXRDPOWDERPOINTSCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPowderPointsCommand : public QxrdPlotButtonCommand
{
public:
  QxrdPowderPointsCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDPOWDERPOINTSCOMMAND_H
