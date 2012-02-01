#ifndef QXRDPOWDERFITIDENTIFYPAGE_H
#define QXRDPOWDERFITIDENTIFYPAGE_H

#include "qxrdpowderfitwidget.h"
#include "qwt_plot_picker.h"

class QxrdPowderFitIdentifyPage : public QxrdPowderFitWidget
{
  Q_OBJECT
public:
  QxrdPowderFitIdentifyPage(QxrdSettingsSaver *saver, QxrdDataProcessorPtr proc, QWidget *parent);

  Q_PROPERTY(int    idCurrentRing     READ get_IdCurrentRing WRITE set_IdCurrentRing STORED false)
  QCEP_INTEGER_PROPERTY(IdCurrentRing)

public slots:
  void selectIdentifyPage();
  void pointClicked(QwtDoublePoint pt);

  void appendRing();
  void removeRing();
  void selectRing(int n);
  void selectNextRing();
  void selectPrevRing();
  void appendPoint(QwtDoublePoint pt);
  void idPerformFit();

public:
  void evaluateFit(double *p, double *x, int np, int nx);

private:
  QwtPlotPicker            *m_Picker;
  QxrdRingSetSampledDataPtr m_SampledData;
  QList<QxrdFitParameter*>  m_FittedParameters;
  QList<QxrdFitParameter*>  m_FixedParameters;
  QVector<double>           m_FitResult;
};

#endif // QXRDPOWDERFITIDENTIFYPAGE_H
