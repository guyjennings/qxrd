/******************************************************************
*
*  $Id: qxrdimageplot.h,v 1.16 2009/07/10 22:54:23 jennings Exp $
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

#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdmaskcolormap.h"

class QxrdImagePlot : public QxrdPlot
{
  Q_OBJECT;

 public:
  QxrdImagePlot(QWidget *parent = 0);

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
  void setDisplayedRange(double min, double max);
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

  void on_minimum_changed(double min);
  void on_maximum_changed(double max);
  void on_interpolate_changed(int interp);
  void on_maintain_aspect_changed(int interp);

  void enableZooming();
  void enableCentering();
  void enableSlicing();
  void enableMeasuring();

 signals:
  void minimum_changed(double min);
  void maximum_changed(double max);\

 public:
  void setImage(QxrdRasterData data);
  void setMask(QxrdMaskRasterData data);
  QxrdRasterData* raster();
  void replotImage();
  void setCenterFinder(QxrdCenterFinder *f);

 private:
  void changedColorMap();
  void setTrackerPen(const QPen &pen);

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
  int                  m_RasterShown;
  int                  m_MaskShown;
  int                  m_MaskAlpha;
  double               m_MinDisplayed;
  double               m_MaxDisplayed;
  int                  m_Interpolate;
  int                  m_MaintainAspect;
  HEADER_IDENT("$Id: qxrdimageplot.h,v 1.16 2009/07/10 22:54:23 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimageplot.h,v $
*  Revision 1.16  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.15  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

