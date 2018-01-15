#ifdef HAVE_DATAVIS

#include "qcepdataobjectsurfaceplotwidget.h"
#include <QGridLayout>
#include "qcepimagedata.h"

using namespace QtDataVisualization;

QcepDataObjectSurfacePlotWidget::QcepDataObjectSurfacePlotWidget(QWidget *parent)
  : QWidget(parent),
    m_Surface(NULL),
    m_MinPlottedVal(0),
    m_MaxPlottedVal(256),
    m_ColorMap     (NULL, "colorMap",      0,     "Image Color Map Index"),
    m_ScalingMode  (NULL, "scalingMode",   0,     "Image Scaling Mode"),
    m_MinimumPct   (NULL, "minimumPct",    0,     "Image Display Minimum %"),
    m_MaximumPct   (NULL, "maximumPct",    100,   "Image Display Maximum %"),
    m_MinimumPctle (NULL, "minimumPctle",  0,     "Image Display Minimum Percentile"),
    m_MaximumPctle (NULL, "maximumPctle",  99,    "Image Display Maximum Percentile"),
    m_MinimumVal   (NULL, "minimumVal",    0,     "Image Display Minimum Value"),
    m_MaximumVal   (NULL, "maximumVal",    10000, "Image Display Maximum Value")
{
  setupUi(this);

  QGridLayout *layout = new QGridLayout(m_SurfacePlot);

  m_SurfaceGraph = new Q3DSurface();
  m_SurfaceGraph -> setShadowQuality(Q3DSurface::ShadowQualityNone);

  QWidget *container = QWidget::createWindowContainer(m_SurfaceGraph);

  layout -> addWidget(container, 0,0, 1,1);

  m_ColorMapMenu->addItem("Grayscale");
  m_ColorMapMenu->addItem("Inverted Grayscale");
  m_ColorMapMenu->addItem("Fire");
  m_ColorMapMenu->addItem("Ice");
  m_ColorMapMenu->addItem("Earth");
  m_ColorMapMenu->addItem("Spectrum");
  m_ColorMapMenu->addItem("Banded Spectrum");

  m_ScalingModeMenu->addItem("Scale on Absolute Values");
  m_ScalingModeMenu->addItem("Scale on Percentage of data range");
  m_ScalingModeMenu->addItem("Scale on Percentiles of data range");

  m_MinimumPercentCtrl->setRange(-10,110);
  m_MinimumPercentCtrl->setSingleStep(0.1);
  m_MaximumPercentCtrl->setRange(-10,110);
  m_MaximumPercentCtrl->setSingleStep(0.1);

  m_MinimumPercentileCtrl->setRange(-10,110);
  m_MinimumPercentileCtrl->setSingleStep(0.1);
  m_MaximumPercentileCtrl->setRange(-10,110);
  m_MaximumPercentileCtrl->setSingleStep(0.1);

  m_MinimumValCtrl->setRange(-10000, 100000);
  m_MaximumValCtrl->setRange(-10000, 100000);

  m_ColorMap.linkTo(m_ColorMapMenu);
  m_ScalingMode.linkTo(m_ScalingModeMenu);
  m_MinimumPct.linkTo(m_MinimumPercentCtrl);
  m_MaximumPct.linkTo(m_MaximumPercentCtrl);
  m_MinimumPctle.linkTo(m_MinimumPercentileCtrl);
  m_MaximumPctle.linkTo(m_MaximumPercentileCtrl);
  m_MinimumVal.linkTo(m_MinimumValCtrl);
  m_MaximumVal.linkTo(m_MaximumValCtrl);

  connect(m_ColorMapMenu,    (void(QComboBox::*)(int))&QComboBox::currentIndexChanged,
          this, &QcepDataObjectSurfacePlotWidget::onColorMapChanged);
  connect(m_ScalingModeMenu, (void(QComboBox::*)(int))&QComboBox::currentIndexChanged,
          this, &QcepDataObjectSurfacePlotWidget::onScalingModeChanged);
  connect(m_ReplotButton,    &QAbstractButton::clicked,
          this, &QcepDataObjectSurfacePlotWidget::onReplotWanted);
}

QcepDataObjectSurfacePlotWidget::~QcepDataObjectSurfacePlotWidget()
{
  delete m_Surface;
}

void QcepDataObjectSurfacePlotWidget::onNewImageAvailable(QcepDoubleImageDataWPtr img)
{
  QcepDoubleImageDataPtr imgp(img);

  if (imgp) {
    m_Image = img;

    m_ImageMinValue = imgp->minValue();
    m_ImageMaxValue = imgp->maxValue();

    double range = m_ImageMaxValue - m_ImageMinValue;
    double step=0.1;

    while (step<range/50) {
      step *= 10;
    }

    m_MinimumValCtrl->setRange(m_ImageMinValue-range/10.0, m_ImageMaxValue+range/10.0);
    m_MaximumValCtrl->setRange(m_ImageMinValue-range/10.0, m_ImageMaxValue+range/10.0);
    m_MinimumValCtrl->setSingleStep(step);
    m_MaximumValCtrl->setSingleStep(step);

    int size = imgp->get_Width() * imgp->get_Height();

    m_ImagePercentiles.resize(size);

    double *imageData = imgp->data();
    double *pcntlData = m_ImagePercentiles.data();

    int j=0;

    for (int i=0; i<size; i++) {
      double v = imageData[i];

      if (qIsFinite(v)) {
        pcntlData[j++] = v;
      }
    }

    m_ImagePercentiles.resize(j);

    std::sort(m_ImagePercentiles.begin(),
              m_ImagePercentiles.end());

    onReplotWanted();
  }
}

double QcepDataObjectSurfacePlotWidget::scaledValue(double v)
{
  double res;

  if (v < m_MinPlottedVal) {
    res = qQNaN();
  } else if (v > m_MaxPlottedVal) {
    res = qQNaN();
  } else {
    res = v;
  }

  return res;
}

void QcepDataObjectSurfacePlotWidget::onReplotWanted()
{
  QcepDoubleImageDataPtr img(m_Image);

  if (img) {
    onScalingModeChanged(get_ScalingMode());

    QSurfaceDataArray *d = new QSurfaceDataArray();

    int wd = img->get_Width();
    int ht = img->get_Height();

    for (int j=0; j<ht; j++) {
      QSurfaceDataRow *r = new QSurfaceDataRow(wd);

      for (int i=0; i<wd; i++) {
        (*r)[i].setPosition(QVector3D(i, scaledValue(img->getImageData(i,j)), j));
      }

      d->append(r);
    }

    QSurfaceDataProxy *p = new QSurfaceDataProxy();

    p->resetArray(d);


    QSurface3DSeries *series = new QSurface3DSeries(p);

    series->setDrawMode(QSurface3DSeries::DrawSurface);
    series->setFlatShadingEnabled(false);

    QLinearGradient gr/*(m_MinPlottedVal, m_MinPlottedVal,
                       m_MaxPlottedVal, m_MaxPlottedVal)*/;

    setColorMap(gr);
//    gr.setColorAt(0.0, Qt::black);
//    gr.setColorAt(0.33, Qt::blue);
//    gr.setColorAt(0.67, Qt::red);
//    gr.setColorAt(1.0, Qt::yellow);

    series -> setBaseGradient(gr);
    series -> setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    if (m_Surface) {
      m_SurfaceGraph->removeSeries(m_Surface);
      delete m_Surface;
    }

    m_Surface = series;

    m_SurfaceGraph->axisX()->setRange(0, wd);
    m_SurfaceGraph->axisY()->setRange(m_MinPlottedVal, m_MaxPlottedVal);
    m_SurfaceGraph->axisZ()->setRange(0, ht);

    m_SurfaceGraph->addSeries(m_Surface);
  }
}

void QcepDataObjectSurfacePlotWidget::setColorMap(QLinearGradient &map)
{
  switch (get_ColorMap()) {
  case 0:
    setGrayscale(map);
    break;

  case 1:
    setInverseGrayscale(map);
    break;

  case 2:
    setFire(map);
    break;

  case 3:
    setIce(map);
    break;

  case 4:
    setEarthTones(map);
    break;

  case 5:
    setSpectrum(map);
    break;

  case 6:
    setBandedSpectrum(map);
    break;

  default:
    setGrayscale(map);
  }
}

void QcepDataObjectSurfacePlotWidget::setGrayscale(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::black);
  map.setColorAt(1.0, Qt::white);
}

void QcepDataObjectSurfacePlotWidget::setInverseGrayscale(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::white);
  map.setColorAt(1.0, Qt::black);
}

void QcepDataObjectSurfacePlotWidget::setFire(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::black);
  map.setColorAt(0.25, Qt::red);
  map.setColorAt(0.75, Qt::yellow);
  map.setColorAt(1.0, Qt::white);
}

void QcepDataObjectSurfacePlotWidget::setIce(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::black);
  map.setColorAt(0.25, Qt::blue);
  map.setColorAt(0.75, Qt::cyan);
  map.setColorAt(1.0, Qt::white);
}

void QcepDataObjectSurfacePlotWidget::setEarthTones(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::black);
  map.setColorAt(0.15, Qt::blue);
  map.setColorAt(0.25, Qt::gray);
  map.setColorAt(0.35, Qt::green);
  map.setColorAt(0.5, Qt::darkYellow);
  map.setColorAt(0.85, Qt::darkMagenta);
  map.setColorAt(1.0, Qt::white);
}

void QcepDataObjectSurfacePlotWidget::setSpectrum(QLinearGradient &map)
{
  map.setColorAt(0.0, Qt::red);
  map.setColorAt(0.2, Qt::blue);
  map.setColorAt(0.4, Qt::cyan);
  map.setColorAt(0.6, Qt::green);
  map.setColorAt(0.8, Qt::yellow);
  map.setColorAt(1.0, Qt::red);
}

void QcepDataObjectSurfacePlotWidget::setBandedSpectrum(QLinearGradient & /*map*/)
{
}

void QcepDataObjectSurfacePlotWidget::onColorMapChanged(int /*newMap*/)
{
}

void QcepDataObjectSurfacePlotWidget::onScalingModeChanged(int newMode)
{
  switch (newMode) {
  case 0: // Absolute values
    m_MinPlottedVal = get_MinimumVal();
    m_MaxPlottedVal = get_MaximumVal();
    break;

  case 1: // Percentage of data range
    m_MinPlottedVal = m_ImageMinValue + get_MinimumPct()*(m_ImageMaxValue-m_ImageMinValue);
    m_MaxPlottedVal = m_ImageMinValue + get_MaximumPct()*(m_ImageMaxValue-m_ImageMinValue);
    break;

  case 2: // Percentiles
    m_MinPlottedVal = percentileValue(get_MinimumPctle());
    m_MaxPlottedVal = percentileValue(get_MaximumPctle());
    break;
  }
}

double QcepDataObjectSurfacePlotWidget::percentileValue(double pctl)
{
  return m_ImagePercentiles.value(m_ImagePercentiles.size()*pctl/100.0);
}

#endif