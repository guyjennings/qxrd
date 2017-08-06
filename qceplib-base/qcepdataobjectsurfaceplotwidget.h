#ifndef QCEPDATAOBJECTSURFACEPLOTWIDGET_H
#define QCEPDATAOBJECTSURFACEPLOTWIDGET_H

#include <QWidget>
#include "ui_qcepdataobjectsurfaceplotwidget.h"
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"
#include <QtDataVisualization>
#include "qcepimagedata-ptr.h"

class QcepDataObjectSurfacePlotWidget : public QWidget, public Ui::QcepDataObjectSurfacePlotWidget
{
  Q_OBJECT
public:
  explicit QcepDataObjectSurfacePlotWidget(QWidget *parent = 0);
  virtual ~QcepDataObjectSurfacePlotWidget();

signals:

public slots:
  void onNewImageAvailable(QcepDoubleImageDataPtr img);
  void onColorMapChanged(int newMap);
  void onScalingModeChanged(int newMode);
  void onReplotWanted();
  double scaledValue(double v);

  double percentileValue(double pctl);

private:
  void setColorMap(QLinearGradient &map);
  void setGrayscale(QLinearGradient &map);
  void setInverseGrayscale(QLinearGradient &map);
  void setSpectrum(QLinearGradient &map);
  void setFire(QLinearGradient &map);
  void setEarthTones(QLinearGradient &map);
  void setIce(QLinearGradient &map);
  void setBandedSpectrum(QLinearGradient &map);

private:
  QcepDoubleImageDataPtr                        m_Image;

  QtDataVisualization::Q3DSurface              *m_SurfaceGraph;
  QtDataVisualization::QSurface3DSeries        *m_Surface;

  double                                        m_MinPlottedVal;
  double                                        m_MaxPlottedVal;

  double                                        m_ImageMinValue;
  double                                        m_ImageMaxValue;
  QVector<double>                               m_ImagePercentiles;

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
};

#endif // QCEPDATAOBJECTSURFACEPLOTWIDGET_H
