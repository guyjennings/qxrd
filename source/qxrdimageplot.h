/******************************************************************
*
*  $Id: qxrdimageplot.h,v 1.3 2010/10/22 21:44:26 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include "qcepmacros.h"

#include <QObject>

#include "qxrdforwardtypes.h"
#include "qxrdplot.h"
#include "qxrddataprocessor.h"

#include <qwt_color_map.h>

#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdmaskcolormap.h"
#include "qxrdplotimage.h"

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

  Q_PROPERTY(double displayMinimumPctle     READ get_DisplayMinimumPctle WRITE set_DisplayMinimumPctle);
  QCEP_DOUBLE_PROPERTY(DisplayMinimumPctle);

  Q_PROPERTY(double displayMaximumPctle     READ get_DisplayMaximumPctle WRITE set_DisplayMaximumPctle);
  QCEP_DOUBLE_PROPERTY(DisplayMaximumPctle);

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

  void onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow);
  void onMaskedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask);
  void onDarkImageAvailable(QxrdDoubleImageDataPtr image);
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(QwtDoublePoint c);

  void enableZooming();
  void enableCentering();
  void enableSlicing();
  void enableMeasuring();
  void enableMaskCircles();
  void enableMaskPolygons();

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

  const QxrdRasterData* raster() const;
  QxrdRasterData* raster();

  QxrdDataProcessorPtr processor() const;
  void setDataProcessor(QxrdDataProcessorPtr proc);

  void replot();

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  void replotImage();
  void setImage(QxrdRasterData data);
  void setMask(QxrdMaskRasterData data);
  void changedColorMap();
  void setTrackerPen(const QPen &pen);

public:
  enum {
    PercentageMode,
    PercentileMode,
    AbsoluteMode
  };

private:
  QwtPlotRescalerPtr         m_Rescaler;
  QxrdPlotSlicerPtr          m_Slicer;
  QxrdImagePlotMeasurerPtr   m_Measurer;
//  QwtLegendPtr               m_Legend;
  QwtPlotSpectrogramPtr      m_Spectrogram;
  QwtPlotSpectrogramPtr      m_MaskImage;
//  QxrdPlotImagePtr           m_PlotImage;
  QxrdRasterData             m_Raster;
  QxrdMaskRasterData         m_MaskRaster;
  QwtLinearColorMap          m_ColorMap;
  QxrdMaskColorMap           m_MaskColorMap;
  int                        m_MaskAlpha;
  QxrdDataProcessorPtr       m_DataProcessor;

  QxrdCenterFinderPickerPtr  m_CenterFinderPicker;
  QwtPlotMarkerPtr           m_CenterMarker;

  QxrdMaskPickerPtr          m_Circles;
  QxrdMaskPickerPtr          m_Polygons;

//  QPen                       m_Pen;
  bool                 m_FirstTime;

  HEADER_IDENT("$Id: qxrdimageplot.h,v 1.3 2010/10/22 21:44:26 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdimageplot.h,v $
*  Revision 1.3  2010/10/22 21:44:26  jennings
*  *** empty log message ***
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.4  2010/09/09 16:45:00  jennings
*  Added separate parameters for percentile display mode, changed display control
*  to only show relevent parameters
*
*  Revision 1.1.2.3  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.2  2010/08/11 15:58:27  jennings
*  Added changedValue(QwtDoublePoint) slots
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.28.4.4  2010/05/09 13:42:37  jennings
*  Eliminated spurious legend on image plot caused by duplicate definition of m_Legend in QxrdImagePlot and QxrdPlot
*
*  Revision 1.28.4.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.28.4.2  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.28.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.28  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.27  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.26  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.25  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.24  2009/08/02 18:02:42  jennings
*  Added a number of masking operations to the UI - no actual implementation yet
*
*  Revision 1.23  2009/07/25 15:18:39  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*  Made QxrdMaskColorMap descend from QwtLinearColorMap
*
*  Revision 1.22  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
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

