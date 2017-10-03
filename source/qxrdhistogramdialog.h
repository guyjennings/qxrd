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
#include "qwt_plot_piecewise_curve-ptr.h"

class QxrdHistogramDialog : public QDockWidget, public Ui::QxrdHistogramDialog
{
  Q_OBJECT

public:
  explicit QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                               QxrdExperimentWPtr expt,
                               QWidget *parent);
  virtual ~QxrdHistogramDialog();

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);

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
  QxrdDoubleImageDataPtr          m_Image;
  QwtPlotPiecewiseCurvePtr        m_TotalCurve;
  QwtPlotPiecewiseCurvePtr        m_SelectCurve;
};

#endif // QXRDHISTOGRAMDIALOG_H
