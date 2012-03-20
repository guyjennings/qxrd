#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qxrdsettingssaver.h"

QxrdHistogramDialog::QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings, QWidget *parent) :
  QDockWidget(parent),
  m_HistogramDialogSettings(settings)
{
  setupUi(this);

  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    m_HistogramPlot->init(set->histogramPlotSettings());
  }
}

void QxrdHistogramDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_Image = image;

  recalculateHistogram();
}

void QxrdHistogramDialog::histogramSelectionChanged(QwtDoubleRect rect)
{
  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    set->set_HistogramRect(rect);

    recalculateHistogram();
  }
}

void QxrdHistogramDialog::recalculateHistogram()
{
}
