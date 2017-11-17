#ifndef QXRDHISTOGRAMDIALOG_H
#define QXRDHISTOGRAMDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdhistogramdialog.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qwt_plot_piecewise_curve-ptr.h"

class QXRD_EXPORT QxrdHistogramDialog : public QDialog, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                               QxrdExperimentWPtr expt,
                               QWidget *parent);
  virtual ~QxrdHistogramDialog();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);

signals:
  void newHistogramCurves(QwtPlotPiecewiseCurvePtr totalCurve,
                          QwtPlotPiecewiseCurvePtr selectCurve);
public slots:
  void histogramSelectionChanged(QRectF rect);
  void updateHistogramNeeded();

  void onNewHistogramCurves(QwtPlotPiecewiseCurvePtr totalCurve,
                          QwtPlotPiecewiseCurvePtr selectCurve);

private slots:
  void recalculateHistogram();

private:
  QxrdExperimentWPtr              m_Experiment;
  QxrdHistogramDialogSettingsWPtr m_HistogramDialogSettings;
  QcepDoubleImageDataPtr          m_Image;
  QwtPlotPiecewiseCurvePtr        m_TotalCurve;
  QwtPlotPiecewiseCurvePtr        m_SelectCurve;
  QxrdImagePlot                  *m_Plot;
};

#endif // QXRDHISTOGRAMDIALOG_H
