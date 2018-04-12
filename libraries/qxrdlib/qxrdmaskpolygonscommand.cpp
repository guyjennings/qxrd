#include "qxrdmaskpolygonscommand.h"
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"
#include "qxrdmaskstack.h"

QxrdMaskPolygonsCommand::QxrdMaskPolygonsCommand(QcepPlotWidget *plot,
                                                 QcepPlotWidgetSettingsWPtr set,
                                                 QxrdMaskStackWPtr maskStack) :
  QcepPlotButtonCommand("maskPolygonsCommand", "Mask Polygons", plot, set, ":/images/mask_polygon.png", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_polygon_invert.png", "Unmask Polygons");

  m_Picker = new QxrdPolygonalMaskPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QxrdMaskPolygonsCommand::selected);
}

void QxrdMaskPolygonsCommand::selected(const QVector<QPointF> &p)
{
  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    m->maskPolygon(p, m_Index);
  }
}
