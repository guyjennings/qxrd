#ifndef QXRDSETCENTERCOMMAND_H
#define QXRDSETCENTERCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdSetCenterCommand : public QxrdPlotButtonCommand
{
public:
  QxrdSetCenterCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDSETCENTERCOMMAND_H
