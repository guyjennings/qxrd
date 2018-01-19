#include "qxrdmaskcirclescommand.h"
#include <QToolButton>
#include <QStringList>

QxrdMaskCirclesCommand::QxrdMaskCirclesCommand(QString name,
                                               QxrdPlotWidget *plot,
                                               QxrdPlotWidgetSettingsWPtr set,
                                               QxrdMaskStackWPtr maskStack) :
  QxrdPlotButtonCommand(name, plot, set, ":/images/mask_circles.png", "Mask Circles", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_circles_invert.png", "Unmask Circles");
}
