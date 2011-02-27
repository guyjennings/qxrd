#include "qxrdinfodialog.h"
#include "ui_qxrdinfodialog.h"
#include "qcepproperty.h"

QxrdInfoDialog::QxrdInfoDialog(QWidget *parent) :
  QDockWidget(parent)
{
  setupUi(this);
}

void QxrdInfoDialog::readSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdInfoDialog::writeSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdInfoDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{

}
