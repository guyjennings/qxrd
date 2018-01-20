#include "qxrdslicecommand.h"
#include <QToolButton>
#include "qxrdplotslicer.h"
#include "qxrdimageplot.h"

QxrdSliceCommand::QxrdSliceCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/slice.png", "Slice", true)
{
  m_Slicer = new QxrdPlotSlicer(m_Plot);
  m_Slicer -> setEnabled(false);

  setPlotPicker(m_Slicer);

  connect(m_Slicer, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QxrdSliceCommand::selected);
}

//TODO: implement
void QxrdSliceCommand::selected(const QVector<QPointF> &p)
{
  printf("QxrdSliceCommand::selected\n");
}
