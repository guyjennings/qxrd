#include "qcepslicecommand.h"
#include <QToolButton>
#include "qcepplotslicer.h"
#include "qcepimageplot.h"

QcepSliceCommand::QcepSliceCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("sliceCommand", "Slice", plot, set, ":/images/slice.png", true)
{
  m_Slicer = new QcepPlotSlicer(m_Plot);
  m_Slicer -> setEnabled(false);

  setPlotPicker(m_Slicer);

  connect(m_Slicer, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QcepSliceCommand::selected);
}

//TODO: implement
void QcepSliceCommand::selected(const QVector<QPointF> &p)
{
  printf("QcepSliceCommand::selected\n");
}

//void QcepSliceDialog::reslice()
//{
//  m_SlicePlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
//  m_SlicePlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

//  QcepSliceDialogSettingsPtr set(m_SliceDialogSettings);

//  if (set && m_Image) {
//    QVector<double> xp,yp;
//    QString title = m_Image->get_Name();

//    double length = 0;

//    QcepPolygon poly = set->get_SlicePolygon();

//    if (poly.size() >= 2) {
//      QPointF p0 = poly[0];

//      for (int i=1; i<poly.size(); i++) {
//        QPointF p1 = poly[i];
//        double dx = p1.x() - p0.x();
//        double dy = p1.y() - p0.y();
//        length += sqrt(dx*dx + dy*dy);
//        p0=p1;
//      }

//      p0 = poly[0];
//      double r = 0;
//      double r0 = 0;

//      title += tr(" [%1,%2]").arg(p0.x()).arg(p0.y());

//      for (int i=1; i<poly.size(); i++) {
//        QPointF p1 = poly[i];
//        double dx = p1.x() - p0.x();
//        double dy = p1.y() - p0.y();
//        double len = sqrt(dx*dx + dy*dy);

//        if (len > 0) {
//          for (; r<len; r+=1) {
//            double x = p0.x() + r*dx/len;
//            double y = p0.y() + r*dy/len;

//            xp.append(r+r0);
//            yp.append(m_Image->value((int) x, (int) y));
//          }
//        }

//        p0 = p1;
//        r0 += len;
//        r  -= len;
//      }

//      if (poly.size() > 2) {
//        title += "...";
//      } else {
//        title += "-";
//      }

//      QPointF p1 = poly.last();
//      title += tr("[%1,%2]").arg(p1.x()).arg(p1.y());

//      QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_SlicePlot, title);

//      pc->setSamples(xp, yp);

//      pc->attach(m_SlicePlot);

//      m_SlicePlot->updateZoomer();
//      m_SlicePlot->replot();
//    }
//  }
//}
