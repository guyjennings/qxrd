#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"

QxrdHistogramDialog::QxrdHistogramDialog(QWidget *parent) :
  QDockWidget(parent)
{
  setupUi(this);
}

void QxrdHistogramDialog::readSettings(QxrdSettings &settings, QString section)
{
  m_HistogramPlot->readSettings(settings, section+"/plot");

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdHistogramDialog::writeSettings(QxrdSettings &settings, QString section)
{
  m_HistogramPlot->writeSettings(settings, section+"/plot");

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdHistogramDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{

}
