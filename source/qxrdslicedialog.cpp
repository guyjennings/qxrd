#include "qxrdslicedialog.h"
#include "ui_qxrdslicedialog.h"

QxrdSliceDialog::QxrdSliceDialog(QWidget *parent) :
  QDockWidget(parent)
{
  setupUi(this);
}

void QxrdSliceDialog::readSettings(QxrdSettings &settings, QString section)
{
  m_SlicePlot->readSettings(settings, section+"/plot");

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdSliceDialog::writeSettings(QxrdSettings &settings, QString section)
{
  m_SlicePlot->writeSettings(settings, section+"/plot");

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdSliceDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Image = image;

  reslice();
}

void QxrdSliceDialog::slicePolygon(QwtArray<QwtDoublePoint> poly)
{
  m_Polygon = poly;

  reslice();
}

void QxrdSliceDialog::reslice()
{
  foreach (QwtDoublePoint pt, m_Polygon) {
    printf("%g,%g\n", pt.x(), pt.y());
  }
}
