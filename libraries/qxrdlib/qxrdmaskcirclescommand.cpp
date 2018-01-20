#include "qxrdmaskcirclescommand.h"
#include <QToolButton>
#include <QStringList>
#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"

QxrdMaskCirclesCommand::QxrdMaskCirclesCommand(QString name,
                                               QxrdPlotWidget *plot,
                                               QxrdPlotWidgetSettingsWPtr set,
                                               QxrdMaskStackWPtr maskStack) :
  QxrdPlotButtonCommand(name, plot, set, ":/images/mask_circles.png", "Mask Circles", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_circles_invert.png", "Unmask Circles");

  m_Picker = new QxrdCircularMaskPicker(m_Plot->canvas(), m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QRectF &)) &QwtPlotPicker::selected,
          this, &QxrdMaskCirclesCommand::selected);
}

//TODO: implement
void QxrdMaskCirclesCommand::selected(const QRectF &r)
{
  printf("QxrdMaskCirclesCommand::selected(l:%g,t:%g,r:%g,b:%g\n",
         r.left(), r.top(), r.right(), r.bottom());
}
