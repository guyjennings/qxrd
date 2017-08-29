#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                               QxrdExperimentWPtr expt,
                               QWidget *parent);
  virtual ~QxrdHistogramDialog();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);

public slots:
  void histogramSelectionChanged(QRectF rect);
  void updateHistogramNeeded();

private:
  void recalculateHistogram();

private:
  QxrdExperimentWPtr              m_Experiment;
  QxrdHistogramDialogSettingsWPtr m_HistogramDialogSettings;
  QcepDoubleImageDataPtr          m_Image;
};

#endif // QXRDHISTOGRAMDIALOG_H
