#include "qxrdmaskpolygonscommand.h"
#include <QToolButton>
#include <QAction>
#include <QMenu>

QxrdMaskPolygonsCommand::QxrdMaskPolygonsCommand(QString name,
                                                 QxrdPlotWidget *plot,
                                                 QxrdPlotWidgetSettingsWPtr set,
                                                 QxrdMaskStackWPtr maskStack) :
  QxrdPlotButtonCommand(name, plot, set, ":/images/mask_polygon.png", "Mask Polygons", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_polygon_invert.png", "Unmask Polygons");
}
