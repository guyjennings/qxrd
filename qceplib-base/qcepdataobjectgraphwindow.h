#ifndef QCEPDATAOBJECTGRAPHWINDOW_H
#define QCEPDATAOBJECTGRAPHWINDOW_H

#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectgraphwindow.h"
#include "qcepdataobjectwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepdataobjectgraphcontroller-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepproperty.h"
#include "qwt_plot_curve.h"

class QcepDataObjectGraphWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectGraphWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectGraphWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj,
      QWidget *parent = 0);
  virtual ~QcepDataObjectGraphWindow();

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

  int controllerRow() const;
  int controllerColumn() const;

  int currentGraphMode();
  void clearPlot();
  void appendCurve(QwtPlotCurve *curve);

  QcepExperimentWPtr experiment();

signals:

public slots:
  void updateDisplay();

protected:

private slots:
  void setGraphMode(int mode);
  void allowGraphMode(int mode);
  void changeGraphMode(int idx);

private:
  QcepExperimentWPtr                            m_Experiment;
  QcepDataObjectWPtr                            m_Object;
  int                                           m_PlottingMode;
  QcepDataObjectGraphControllerPtr              m_Controller;
//  QcepSettingsSaverPtr                          m_SettingsSaver;
//  QcepPlotSettingsPtr                           m_PlotSettings;

public:
  enum {
    NoPlot,
    DefaultPlot,
    ScatterPlot,
    ImagePlot,
    HistogramPlot,
    HorizontalSlice,
    VerticalSlice
  };

public:
  Q_PROPERTY(int colorMap READ get_ColorMap WRITE set_ColorMap)
  QCEP_INTEGER_PROPERTY(ColorMap)

  Q_PROPERTY(int scalingMode READ get_ScalingMode WRITE set_ScalingMode)
  QCEP_INTEGER_PROPERTY(ScalingMode)

  Q_PROPERTY(double minimumPct READ get_MinimumPct WRITE set_MinimumPct)
  QCEP_DOUBLE_PROPERTY(MinimumPct)

  Q_PROPERTY(double maximumPct READ get_MaximumPct WRITE set_MaximumPct)
  QCEP_DOUBLE_PROPERTY(MaximumPct)

  Q_PROPERTY(double minimumPctle READ get_MinimumPctle WRITE set_MinimumPctle)
  QCEP_DOUBLE_PROPERTY(MinimumPctle)

  Q_PROPERTY(double maximumPctle READ get_MaximumPctle WRITE set_MaximumPctle)
  QCEP_DOUBLE_PROPERTY(MaximumPctle)

  Q_PROPERTY(double minimumVal READ get_MinimumVal WRITE set_MinimumVal)
  QCEP_DOUBLE_PROPERTY(MinimumVal)

  Q_PROPERTY(double maximumVal READ get_MaximumVal WRITE set_MaximumVal)
  QCEP_DOUBLE_PROPERTY(MaximumVal)

  Q_PROPERTY(bool histFullVis READ get_HistFullVis WRITE set_HistFullVis)
  QCEP_BOOLEAN_PROPERTY(HistFullVis)

  Q_PROPERTY(bool histAVis READ get_HistAVis WRITE set_HistAVis)
  QCEP_BOOLEAN_PROPERTY(HistAVis)

  Q_PROPERTY(bool histBVis READ get_HistBVis WRITE set_HistBVis)
  QCEP_BOOLEAN_PROPERTY(HistBVis)

  Q_PROPERTY(bool histCVis READ get_HistCVis WRITE set_HistCVis)
  QCEP_BOOLEAN_PROPERTY(HistCVis)

  Q_PROPERTY(int histALeft READ get_HistALeft WRITE set_HistALeft)
  QCEP_INTEGER_PROPERTY(HistALeft)

  Q_PROPERTY(int histATop READ get_HistATop WRITE set_HistATop)
  QCEP_INTEGER_PROPERTY(HistATop)

  Q_PROPERTY(int histARight READ get_HistARight WRITE set_HistARight)
  QCEP_INTEGER_PROPERTY(HistARight)

  Q_PROPERTY(int histABottom READ get_HistABottom WRITE set_HistABottom)
  QCEP_INTEGER_PROPERTY(HistABottom)

  Q_PROPERTY(int histBLeft READ get_HistBLeft WRITE set_HistBLeft)
  QCEP_INTEGER_PROPERTY(HistBLeft)

  Q_PROPERTY(int histBTop READ get_HistBTop WRITE set_HistBTop)
  QCEP_INTEGER_PROPERTY(HistBTop)

  Q_PROPERTY(int histBRight READ get_HistBRight WRITE set_HistBRight)
  QCEP_INTEGER_PROPERTY(HistBRight)

  Q_PROPERTY(int histBBottom READ get_HistBBottom WRITE set_HistBBottom)
  QCEP_INTEGER_PROPERTY(HistBBottom)

  Q_PROPERTY(int histCLeft READ get_HistCLeft WRITE set_HistCLeft)
  QCEP_INTEGER_PROPERTY(HistCLeft)

  Q_PROPERTY(int histCTop READ get_HistCTop WRITE set_HistCTop)
  QCEP_INTEGER_PROPERTY(HistCTop)

  Q_PROPERTY(int histCRight READ get_HistCRight WRITE set_HistCRight)
  QCEP_INTEGER_PROPERTY(HistCRight)

  Q_PROPERTY(int histCBottom READ get_HistCBottom WRITE set_HistCBottom)
  QCEP_INTEGER_PROPERTY(HistCBottom)

  Q_PROPERTY(int sliceHStart READ get_SliceHStart WRITE set_SliceHStart)
  QCEP_INTEGER_PROPERTY(SliceHStart)

  Q_PROPERTY(int sliceHSummed READ get_SliceHSummed WRITE set_SliceHSummed)
  QCEP_INTEGER_PROPERTY(SliceHSummed)

  Q_PROPERTY(int sliceHSkipped READ get_SliceHSkipped WRITE set_SliceHSkipped)
  QCEP_INTEGER_PROPERTY(SliceHSkipped)

  Q_PROPERTY(int sliceHRepeats READ get_SliceHRepeats WRITE set_SliceHRepeats)
  QCEP_INTEGER_PROPERTY(SliceHRepeats)

  Q_PROPERTY(int sliceVStart READ get_SliceVStart WRITE set_SliceVStart)
  QCEP_INTEGER_PROPERTY(SliceVStart)

  Q_PROPERTY(int sliceVSummed READ get_SliceVSummed WRITE set_SliceVSummed)
  QCEP_INTEGER_PROPERTY(SliceVSummed)

  Q_PROPERTY(int sliceVSkipped READ get_SliceVSkipped WRITE set_SliceVSkipped)
  QCEP_INTEGER_PROPERTY(SliceVSkipped)

  Q_PROPERTY(int sliceVRepeats READ get_SliceVRepeats WRITE set_SliceVRepeats)
  QCEP_INTEGER_PROPERTY(SliceVRepeats)
};

#endif // QCEPDATAOBJECTGRAPHWINDOW_H
