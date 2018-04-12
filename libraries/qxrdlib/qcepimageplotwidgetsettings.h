#ifndef QCEPIMAGEPLOTWIDGETSETTINGS_H
#define QCEPIMAGEPLOTWIDGETSETTINGS_H

#include "qcepplotwidgetsettings.h"

class QCEP_EXPORT QcepImagePlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

private:
  typedef QcepPlotWidgetSettings inherited;

public:
  Q_INVOKABLE QcepImagePlotWidgetSettings(QString name);
  void initialize(QcepObjectWPtr parent);

public:
  Q_PROPERTY(double displayMinimumPctle     READ get_DisplayMinimumPctle WRITE set_DisplayMinimumPctle)
  QCEP_DOUBLE_PROPERTY(DisplayMinimumPctle)

  Q_PROPERTY(double displayMaximumPctle     READ get_DisplayMaximumPctle WRITE set_DisplayMaximumPctle)
  QCEP_DOUBLE_PROPERTY(DisplayMaximumPctle)

  Q_PROPERTY(double displayMinimumPct     READ get_DisplayMinimumPct WRITE set_DisplayMinimumPct)
  QCEP_DOUBLE_PROPERTY(DisplayMinimumPct)

  Q_PROPERTY(double displayMaximumPct     READ get_DisplayMaximumPct WRITE set_DisplayMaximumPct)
  QCEP_DOUBLE_PROPERTY(DisplayMaximumPct)

  Q_PROPERTY(double displayMinimumVal     READ get_DisplayMinimumVal WRITE set_DisplayMinimumVal)
  QCEP_DOUBLE_PROPERTY(DisplayMinimumVal)

  Q_PROPERTY(double displayMaximumVal     READ get_DisplayMaximumVal WRITE set_DisplayMaximumVal)
  QCEP_DOUBLE_PROPERTY(DisplayMaximumVal)

  Q_PROPERTY(int displayScalingMode        READ get_DisplayScalingMode WRITE set_DisplayScalingMode)
  QCEP_INTEGER_PROPERTY(DisplayScalingMode)

  Q_PROPERTY(int displayColorMap        READ get_DisplayColorMap WRITE set_DisplayColorMap)
  QCEP_INTEGER_PROPERTY(DisplayColorMap)

  Q_PROPERTY(bool displayLog READ get_DisplayLog WRITE set_DisplayLog)
  QCEP_BOOLEAN_PROPERTY(DisplayLog)

  Q_PROPERTY(bool imageShown READ get_ImageShown WRITE set_ImageShown)
  QCEP_BOOLEAN_PROPERTY(ImageShown)

  Q_PROPERTY(bool maskShown  READ get_MaskShown WRITE set_MaskShown)
  QCEP_BOOLEAN_PROPERTY(MaskShown)

  Q_PROPERTY(bool overflowShown  READ get_OverflowShown WRITE set_OverflowShown)
  QCEP_BOOLEAN_PROPERTY(OverflowShown)

  Q_PROPERTY(double overflowLevel  READ get_OverflowLevel WRITE set_OverflowLevel)
  QCEP_DOUBLE_PROPERTY(OverflowLevel)

  Q_PROPERTY(bool interpolatePixels        READ get_InterpolatePixels WRITE set_InterpolatePixels)
  QCEP_BOOLEAN_PROPERTY(InterpolatePixels)

  Q_PROPERTY(bool maintainAspectRatio        READ get_MaintainAspectRatio WRITE set_MaintainAspectRatio)
  QCEP_BOOLEAN_PROPERTY(MaintainAspectRatio)

  Q_PROPERTY(bool displayROI READ get_DisplayROI WRITE set_DisplayROI)
  QCEP_BOOLEAN_PROPERTY(DisplayROI)

  Q_PROPERTY(double tthMouse READ get_TTHMouse WRITE set_TTHMouse STORED false)
  QCEP_DOUBLE_PROPERTY(TTHMouse)

  Q_PROPERTY(double qMouse READ get_QMouse WRITE set_QMouse STORED false)
  QCEP_DOUBLE_PROPERTY(QMouse)

  Q_PROPERTY(double rMouse READ get_RMouse WRITE set_RMouse STORED false)
  QCEP_DOUBLE_PROPERTY(RMouse)

  Q_PROPERTY(double valMouse READ get_ValMouse WRITE set_ValMouse STORED false)
  QCEP_DOUBLE_PROPERTY(ValMouse)

  Q_PROPERTY(double maskMouse READ get_MaskMouse WRITE set_MaskMouse STORED false)
  QCEP_DOUBLE_PROPERTY(MaskMouse)
};

Q_DECLARE_METATYPE(QcepImagePlotWidgetSettings*)

#endif // QCEPIMAGEPLOTWIDGETSETTINGS_H
