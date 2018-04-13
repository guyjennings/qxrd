#include "qcepmaskcommandbutton.h"
#include <QToolButton>
#include <QStringList>
#include "qcepmaskpicker.h"
#include "qcepimageplot.h"
#include "qcepmaskstack.h"

QcepMaskCommandButton::QcepMaskCommandButton(QString name,
                                             QcepPlotWidget *plot,
                                             QcepPlotWidgetSettingsWPtr set,
                                             QcepMaskStackWPtr maskStack) :
  QcepPlotButtonCommand("maskCommandButton", QString(), plot, set, QString(), true),
  m_MaskStack(maskStack)
{
  appendMode(":/images/mask_circles.png",           "Mask Circles");
  appendMode(":/images/mask_circles_invert.png",    "Unmask Circles");
  appendMode(":/images/mask_rectangles.png",        "Mask Rectangles");
  appendMode(":/images/mask_rectangles_invert.png", "Unmask Rectangles");
  appendMode(":/images/mask_polygon.png",           "Mask Polygons");
  appendMode(":/images/mask_polygon_invert.png",    "Unmask Polygons");

  m_RectanglePicker = new QcepRectangularMaskPicker(m_Plot);
  m_RectanglePicker -> setEnabled(false);

  m_CirclePicker = new QcepCircularMaskPicker(m_Plot);
  m_CirclePicker -> setEnabled(false);

  m_PolygonPicker = new QcepPolygonalMaskPicker(m_Plot);
  m_PolygonPicker -> setEnabled(false);

  changeMode(0);

  connect(m_RectanglePicker, (void (QwtPlotPicker::*)(const QRectF &)) &QwtPlotPicker::selected,
          this, &QcepMaskCommandButton::selectedRect);

  connect(m_CirclePicker, (void (QwtPlotPicker::*)(const QRectF &)) &QwtPlotPicker::selected,
          this, &QcepMaskCommandButton::selectedRect);

  connect(m_PolygonPicker, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QcepMaskCommandButton::selectedPolygon);
}

void QcepMaskCommandButton::changeMode(int i)
{
  QcepPlotButtonCommand::changeMode(i);

  switch (i) {
  default:
  case 0: case 1:
    setPlotPicker(m_CirclePicker);
    break;

  case 2: case 3:
    setPlotPicker(m_RectanglePicker);
    break;

  case 4: case 5:
    setPlotPicker(m_PolygonPicker);
    break;
  }
}

void QcepMaskCommandButton::selectedRect(const QRectF &r)
{
  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    switch (m_Index) {
    case 0: case 1:
      m->maskCircle(r, m_Index & 1);
      break;

    case 2: case 3:
      m->maskRectangle(r, m_Index & 1);
      break;

    default:
      break;
    }
  }
}

void QcepMaskCommandButton::selectedPolygon(const QVector<QPointF> &p)
{
  QcepMaskStackPtr m(m_MaskStack);

  if (m) {
    switch (m_Index) {
    case 4: case 5:
      m->maskPolygon(p, m_Index & 1);
      break;

    default:
      break;
    }
  }
}
