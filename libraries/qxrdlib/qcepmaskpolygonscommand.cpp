#include "qcepmaskpolygonscommand.h"
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include "qcepmaskpicker.h"
#include "qcepimageplot.h"
#include "qcepmaskstack.h"

QcepMaskPolygonsCommand::QcepMaskPolygonsCommand(QcepPlotWidget *plot,
                                                 QcepPlotWidgetSettingsWPtr set,
                                                 QcepMaskStackWPtr maskStack) :
  QcepPlotButtonCommand("maskPolygonsCommand", "Mask Polygons", plot, set, ":/images/mask_polygon.png", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_polygon_invert.png", "Unmask Polygons");

  m_Picker = new QcepPolygonalMaskPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QcepMaskPolygonsCommand::selected);
}

void QcepMaskPolygonsCommand::selected(const QVector<QPointF> &p)
{
  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    m->maskPolygon(p, m_Index);
  }
}
