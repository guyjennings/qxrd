#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qxrdsettingssaver.h"

QxrdHistogramDialog::QxrdHistogramDialog(QxrdSettingsSaverWPtr saver, QWidget *parent) :
  QDockWidget(parent),
  m_Saver(saver)
{
  setupUi(this);
}

void QxrdHistogramDialog::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    m_HistogramPlot->readSettings(settings, section+"/plot");

    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    settings->beginGroup(section+"/rect");

    if (settings->contains("left"))   m_HistogramRect.setLeft(settings->value("left",0).toDouble());
    if (settings->contains("top"))    m_HistogramRect.setTop(settings->value("top",0).toDouble());
    if (settings->contains("right"))  m_HistogramRect.setRight(settings->value("right",0).toDouble());
    if (settings->contains("bottom")) m_HistogramRect.setBottom(settings->value("bottom",0).toDouble());

    settings->endGroup();
  }
}

void QxrdHistogramDialog::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    m_HistogramPlot->writeSettings(settings, section+"/plot");

    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    settings->beginGroup(section+"/rect");

    settings->setValue("left", m_HistogramRect.left());
    settings->setValue("top", m_HistogramRect.top());
    settings->setValue("right", m_HistogramRect.right());
    settings->setValue("bottom", m_HistogramRect.bottom());

    settings->endGroup();
  }
}

void QxrdHistogramDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Image = image;

  recalculateHistogram();
}

void QxrdHistogramDialog::histogramSelectionChanged(QwtDoubleRect rect)
{
  QxrdSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->changed(NULL);
  }

  m_HistogramRect = rect;

  recalculateHistogram();
}

void QxrdHistogramDialog::recalculateHistogram()
{
}
