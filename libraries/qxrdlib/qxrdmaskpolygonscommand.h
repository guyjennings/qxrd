#ifndef QXRDMASKPOLYGONSCOMMAND_H
#define QXRDMASKPOLYGONSCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdMaskPolygonsCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskPolygonsCommand(QString name, QxrdImagePlot *plot);

  QToolButton *toolButton();
};

#endif // QXRDMASKPOLYGONSCOMMAND_H
