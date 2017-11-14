#ifndef QXRDMASKPOLYGONSCOMMAND_H
#define QXRDMASKPOLYGONSCOMMAND_H

#include "qxrdplotcommand.h"

class QxrdMaskPolygonsCommand : public QxrdPlotCommand
{
public:
  QxrdMaskPolygonsCommand(QString name);

  QToolButton *toolButton();
};

#endif // QXRDMASKPOLYGONSCOMMAND_H
