#ifndef QXRDPOWDERFITIMAGEPLOT_H
#define QXRDPOWDERFITIMAGEPLOT_H

#include "qxrdimageplot.h"

class QxrdPowderFitImagePlot : public QxrdImagePlot
{
  Q_OBJECT;
public:
  explicit QxrdPowderFitImagePlot(QWidget *parent = 0);

  void setRingSetSampleMarkers(QxrdRingSetSampledDataPtr p);
  void setRingSampleMarkers(int n);
  void showRingSetSampleMarkers(bool show);

signals:

public slots:

private:
  QxrdRingSetSampledDataPtr m_RingSamples;
  int                       m_Ring;
  bool                      m_RingsSamplesShown;
};

#endif // QXRDPOWDERFITIMAGEPLOT_H
