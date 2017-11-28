#ifdef HAVE_DATAVIS

#include "qxrdtestimageplotdatavishelper.h"
#include "qcepimagedata.h"

using namespace QtDataVisualization;

QxrdTestImagePlotDataVisHelper::QxrdTestImagePlotDataVisHelper()
  : QcepSerializableObject("imagePlotHelper")
{

}

void QxrdTestImagePlotDataVisHelper::onNewImageAvailable(QcepDoubleImageDataPtr data)
{
//  int ind = -1;

  if (data) {
//    ind = data->get_ImageNumber();

//  m_Messages->append(tr("New Image Received : Index %1 : Addr %2").arg(ind).HEXARG(data.data()));

    QSurfaceDataArray *d = new QSurfaceDataArray();

    int wd = data->get_Width();
    int ht = data->get_Height();

    for (int j=0; j<ht; j++) {
      QSurfaceDataRow *r = new QSurfaceDataRow(wd);

      for (int i=0; i<wd; i++) {
        (*r)[i].setPosition(QVector3D(i,data->getImageData(i,j),j));
      }

      d->append(r);
    }

    QSurfaceDataProxy *p = new QSurfaceDataProxy();
    p->resetArray(d);

    QSurface3DSeriesPtr series =
        QSurface3DSeriesPtr(new QSurface3DSeries(p));

    series -> setDrawMode(QSurface3DSeries::DrawSurface);
    series -> setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    series -> setBaseGradient(gr);
    series -> setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    emit newSurfaceSeries(series);
  }
}

#endif // HAVE_DATAVIS
