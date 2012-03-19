#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qxrdsettingssaver.h"

QxrdHistogramDialog::QxrdHistogramDialog(QWidget *parent) :
  QDockWidget(parent)
{
  setupUi(this);
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
