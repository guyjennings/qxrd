#ifndef QXRDDETECTORIMAGEPLOT_H
#define QXRDDETECTORIMAGEPLOT_H

#include "qxrdimageplot.h"

class QxrdDetectorImagePlot : public QxrdImagePlot
{
  Q_OBJECT

public:
  QxrdDetectorImagePlot(QWidget *parent=0);
  void init(QxrdImagePlotSettingsWPtr settings);

  void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:
};

#endif // QXRDDETECTORIMAGEPLOT_H
