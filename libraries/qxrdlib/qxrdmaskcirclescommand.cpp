#include "qxrdmaskcirclescommand.h"
#include <QToolButton>
#include <QStringList>
#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdmaskstack.h"

QxrdMaskCirclesCommand::QxrdMaskCirclesCommand(QString name,
                                               QxrdPlotWidget *plot,
                                               QxrdPlotWidgetSettingsWPtr set,
                                               QxrdMaskStackWPtr maskStack) :
  QxrdPlotButtonCommand(name, plot, set, ":/images/mask_circles.png", "Mask Circles", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_circles_invert.png", "Unmask Circles");

  m_Picker = new QxrdCircularMaskPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QRectF &)) &QwtPlotPicker::selected,
          this, &QxrdMaskCirclesCommand::selected);
}

void QxrdMaskCirclesCommand::selected(const QRectF &r)
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->maskCircle(r, m_Index);
  }
}
