#include "qcepdataobjectsurfaceplotwindow.h"
#include <QGridLayout>
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"

using namespace QtDataVisualization;

QcepDataObjectSurfacePlotWindow::QcepDataObjectSurfacePlotWindow(
    QcepExperimentWPtr expt,
    QcepDataObjectWPtr obj,
    QWidget *parent)
  : QcepDataObjectWindow(parent),
    m_Experiment(expt),
    m_Object(obj),
    m_Surface(NULL)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  QGridLayout *layout = new QGridLayout(m_CentralWidget);

  m_SurfaceGraph = new Q3DSurface();

  QWidget *container = QWidget::createWindowContainer(m_SurfaceGraph);

  layout -> addWidget(container, 0,0, 1,1);

  QcepDoubleImageDataPtr img = qSharedPointerDynamicCast<QcepDoubleImageData>(obj);

  onNewImageAvailable(img);
}

QcepDataObjectSurfacePlotWindow::~QcepDataObjectSurfacePlotWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Surface Plot Window\n");
#endif
  delete m_Surface;
}

void QcepDataObjectSurfacePlotWindow::onNewImageAvailable(QcepDoubleImageDataPtr img)
{
  if (img) {
    setWindowTitle(img->get_Name());

    QSurfaceDataArray *d = new QSurfaceDataArray();

    int wd = img->get_Width();
    int ht = img->get_Height();

    for (int j=0; j<ht; j++) {
      QSurfaceDataRow *r = new QSurfaceDataRow(wd);

      for (int i=0; i<wd; i++) {
        (*r)[i].setPosition(QVector3D(i, img->getImageData(i,j), j));
      }

      d->append(r);
    }

    QSurfaceDataProxy *p = new QSurfaceDataProxy();

    p->resetArray(d);


    QSurface3DSeries *series = new QSurface3DSeries(p);

    series->setDrawMode(QSurface3DSeries::DrawSurface);
    series->setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    series -> setBaseGradient(gr);
    series -> setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    if (m_Surface) {
      m_SurfaceGraph->removeSeries(m_Surface);
      delete m_Surface;
    }

    m_Surface = series;

    m_SurfaceGraph->axisX()->setRange(0, wd);
    m_SurfaceGraph->axisY()->setAutoAdjustRange(true);
    m_SurfaceGraph->axisZ()->setRange(0, ht);

    m_SurfaceGraph->addSeries(m_Surface);
  }
}
