#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdmaskdata.h"
#include "qxrdmaskdata-ptr.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdexperiment-ptr.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                               QxrdExperimentWPtr expt,
                               QWidget *parent);
  virtual ~QxrdHistogramDialog();

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

public slots:
  void histogramSelectionChanged(QRectF rect);

private:
  void recalculateHistogram();

private:
  QxrdExperimentWPtr              m_Experiment;
  QxrdHistogramDialogSettingsWPtr m_HistogramDialogSettings;
  QxrdDoubleImageDataPtr          m_Image;
};

#endif // QXRDHISTOGRAMDIALOG_H
