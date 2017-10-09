#ifndef QXRDTESTIMAGEPLOTDATAVIS_H
#define QXRDTESTIMAGEPLOTDATAVIS_H

#ifdef HAVE_DATAVIS

#include "qxrdlib_global.h"
#include <QWidget>
#include "qxrdtestimageplotdatavishelper-ptr.h"
#include "qxrdtestimageplotdatavishelper.h"
#include "qxrdtestthread.h"
#include "QtDataVisualization"

class QXRD_EXPORT QxrdTestImagePlotDataVis : public QWidget
{
  Q_OBJECT

public:
  explicit QxrdTestImagePlotDataVis(QWidget *parent = nullptr);

  QxrdTestImagePlotDataVisHelperPtr helper();

signals:

public slots:
  void onNewSurfaceSeries(QSurface3DSeriesPtr surface);

private:
  QSharedPointer<QxrdTestThread<QxrdTestImagePlotDataVisHelper> > m_HelperThread;
  QxrdTestImagePlotDataVisHelperPtr                               m_Helper;
  QtDataVisualization::Q3DSurface                                *m_SurfaceGraph;
  QSurface3DSeriesPtr                                             m_Surface;
};

#endif // HAVE_DATAVIS

#endif // QXRDTESTIMAGEPLOTDATAVIS_H
