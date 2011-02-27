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

}
