#include "qxrdmaskpolygonscommand.h"
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include "qxrdmaskpicker.h"
#include "qxrdimageplot.h"

QxrdMaskPolygonsCommand::QxrdMaskPolygonsCommand(QString name,
                                                 QxrdPlotWidget *plot,
                                                 QxrdPlotWidgetSettingsWPtr set,
                                                 QxrdMaskStackWPtr maskStack) :
  QxrdPlotButtonCommand(name, plot, set, ":/images/mask_polygon.png", "Mask Polygons", true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_polygon_invert.png", "Unmask Polygons");

  m_Picker = new QxrdPolygonalMaskPicker(m_Plot);
  m_Picker -> setEnabled(false);

  setPlotPicker(m_Picker);

  connect(m_Picker, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QxrdMaskPolygonsCommand::selected);
}

//TODO: implement
void QxrdMaskPolygonsCommand::selected(const QVector<QPointF> &p)
{
  printf("QxrdMaskPolygonsCommand::selected\n");
}
