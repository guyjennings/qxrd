/******************************************************************
*
*  $Id: qxrdimageplot.h,v 1.21 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include "qcepmacros.h"

#include <QObject>

#include "qxrdplot.h"
#include <qwt_color_map.h>

class QxrdPlotZoomer;
class QxrdPlotTracker;
class QxrdPlotSlicer;
class QxrdPlotMeasurer;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QwtLegend;
class QwtPlotSpectrogram;
class QwtPlotRescaler;
class QxrdCenterFinder;
class QxrdSettings;
class QxrdDataProcessor;
class QxrdCenterFinderPicker;
class QwtPlotMarker;

#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdmaskcolormap.h"

class QxrdImagePlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdImagePlot(QWidget *parent = 0);

public:
  Q_PROPERTY(double displayMinimumPct     READ get_DisplayMinimumPct WRITE set_DisplayMinimumPct);
  QCEP_DOUBLE_PROPERTY(DisplayMinimumPct);

  Q_PROPERTY(double displayMaximumPct     READ get_DisplayMaximumPct WRITE set_DisplayMaximumPct);
  QCEP_DOUBLE_PROPERTY(DisplayMaximumPct);

  Q_PROPERTY(double displayMinimumVal     READ get_DisplayMinimumVal WRITE set_DisplayMinimumVal);
  QCEP_DOUBLE_PROPERTY(DisplayMinimumVal);

  Q_PROPERTY(double displayMaximumVal     READ get_DisplayMaximumVal WRITE set_DisplayMaximumVal);
  QCEP_DOUBLE_PROPERTY(DisplayMaximumVal);

  Q_PROPERTY(int displayScalingMode        READ get_DisplayScalingMode WRITE set_DisplayScalingMode);
  QCEP_INTEGER_PROPERTY(DisplayScalingMode);

  Q_PROPERTY(int displayColorMap        READ get_DisplayColorMap WRITE set_DisplayColorMap);
  QCEP_INTEGER_PROPERTY(DisplayColorMap);

  Q_PROPERTY(bool imageShown READ get_ImageShown WRITE set_ImageShown);
  QCEP_BOOLEAN_PROPERTY(ImageShown);

  Q_PROPERTY(bool maskShown  READ get_MaskShown WRITE set_MaskShown);
  QCEP_BOOLEAN_PROPERTY(MaskShown);

  Q_PROPERTY(bool interpolatePixels        READ get_InterpolatePixels WRITE set_InterpolatePixels);
  QCEP_BOOLEAN_PROPERTY(InterpolatePixels);

  Q_PROPERTY(bool maintainAspectRatio        READ get_MaintainAspectRatio WRITE set_MaintainAspectRatio);
  QCEP_BOOLEAN_PROPERTY(MaintainAspectRatio);

public slots:
  void autoScale();
  void doZoomIn();
  void doZoomOut();
  void doZoomAll();
  void doSetCenter();
  void doSlice();
  void doMeasure();

  void set005Range();
  void set010Range();
  void set100Range();
  void recalculateDisplayedRange();
  void setAutoRange();

  void setGrayscale();
  void setInverseGrayscale();
  void setEarthTones();
  void setSpectrum();
  void setFire();
  void setIce();
  void setColorMap(int index);

  void toggleShowImage();
  void toggleShowMask();

  void changeImageShown(bool shown);
  void changeMaskShown(bool shown);

  void onInterpolateChanged(bool interp);
  void onMaintainAspectChanged(bool interp);

  void onProcessedImageAvailable(QxrdImageData *image);
  void onDarkImageAvailable(QxrdImageData *image);

  void enableZooming();
  void enableCentering();
  void enableSlicing();
  void enableMeasuring();

signals:
  void minimum_changed(double min);
  void maximum_changed(double max);\

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  void setImage(QxrdRasterData data);
  void setMask(QxrdMaskRasterData data);
  QxrdRasterData* raster();
  void replotImage();
  void setCenterFinder(QxrdCenterFinder *f);

  void setDataProcessor(QxrdDataProcessor *proc);

  void replot();

private:
  void changedColorMap();
  void setTrackerPen(const QPen &pen);

  enum {
    PercentageMode,
    AbsoluteMode
  };

private:
  QxrdPlotTracker     *m_Tracker;
  QxrdPlotZoomer      *m_Zoomer;
  QwtPlotPanner       *m_Panner;
  QwtPlotMagnifier    *m_Magnifier;
  QwtPlotRescaler     *m_Rescaler;
  QxrdCenterFinder    *m_CenterFinder;
  QxrdPlotSlicer      *m_Slicer;
  QxrdPlotMeasurer    *m_Measurer;
  QwtLegend           *m_Legend;
  QwtPlotSpectrogram  *m_Spectrogram;
  QwtPlotSpectrogram  *m_MaskImage;
  QxrdRasterData       m_Raster;
  QxrdMaskRasterData   m_MaskRaster;
  QwtLinearColorMap    m_ColorMap;
  QxrdMaskColorMap     m_MaskColorMap;
  int                  m_MaskAlpha;
  QxrdDataProcessor   *m_DataProcessor;

  QxrdCenterFinderPicker    *m_CenterFinderPicker;
  QwtPlotMarker             *m_CenterMarker;
//  QPen                       m_Pen;

  HEADER_IDENT("$Id: qxrdimageplot.h,v 1.21 2009/07/21 22:55:48 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimageplot.h,v $
*  Revision 1.21  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.20  2009/07/20 00:35:23  jennings
*  Trying to optimise screen redraws
*
*  Revision 1.19  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.18  2009/07/16 21:26:25  jennings
*  Made various image display variables into properties
*
*  Revision 1.17  2009/07/16 20:10:43  jennings
*  Made various image display variables into properties
*
*  Revision 1.16  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.15  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

