#include "qcepmaskcirclescommand.h"
#include <QToolButton>
#include <QStringList>
#include "qcepmaskpicker.h"
#include "qcepimageplot.h"
#include "qcepmaskstack.h"

QcepMaskCirclesCommand::QcepMaskCirclesCommand(QcepPlotWidget *plot,
                                               QcepPlotWidgetSettingsWPtr set,
                                               QcepMaskStackWPtr maskStack) :
  QcepPlotButtonCommand("maskCirclesCommand",
                        "Mask Circles",
                        plot,
                        set,
                        ":/images/mask_circles.png",
                        true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_circles_invert.png", "Unmask Circles");

  m_Picker = new QcepCircularMaskPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QRectF &)) &QwtPlotPicker::selected,
          this, &QcepMaskCirclesCommand::selected);
}

void QcepMaskCirclesCommand::selected(const QRectF &r)
{
  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    m->maskCircle(r, m_Index);
  }
}
