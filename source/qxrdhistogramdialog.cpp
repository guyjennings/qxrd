#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qxrdsettingssaver.h"
#include "qxrddebug.h"

QxrdHistogramDialog::QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings, QWidget *parent) :
  QDockWidget(parent),
  m_HistogramDialogSettings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::QxrdHistogramDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    m_HistogramPlot->init(set->histogramPlotSettings());
  }
}

QxrdHistogramDialog::~QxrdHistogramDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::~QxrdHistogramDialog(%p)\n", this);
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
