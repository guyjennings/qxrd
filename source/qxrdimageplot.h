#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include "qcepmacros.h"

#include <QObject>

#include "qxrdforwardtypes.h"
#include "qxrdplot.h"
#include "qxrddataprocessor.h"

#include <qwt_color_map.h>
#include <qwt_array.h>

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

  Q_PROPERTY(bool overflowShown  READ get_OverflowShown WRITE set_OverflowShown);
  QCEP_BOOLEAN_PROPERTY(OverflowShown);

  Q_PROPERTY(bool interpolatePixels        READ get_InterpolatePixels WRITE set_InterpolatePixels);
  QCEP_BOOLEAN_PROPERTY(InterpolatePixels);

  Q_PROPERTY(bool maintainAspectRatio        READ get_MaintainAspectRatio WRITE set_MaintainAspectRatio);
  QCEP_BOOLEAN_PROPERTY(MaintainAspectRatio);

  Q_PROPERTY(double tthMouse READ get_TTHMouse WRITE set_TTHMouse STORED false);
  QCEP_DOUBLE_PROPERTY(TTHMouse);

  Q_PROPERTY(double valMouse READ get_ValMouse WRITE set_ValMouse STORED false);
  QCEP_DOUBLE_PROPERTY(ValMouse);

  Q_PROPERTY(double maskMouse READ get_MaskMouse WRITE set_MaskMouse STORED false);
  QCEP_DOUBLE_PROPERTY(MaskMouse);

signals:
  void slicePolygon(QwtArray<QwtDoublePoint> poly);

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
  void toggleShowOverflow();

  void changeImageShown(bool shown);
  void changeMaskShown(bool shown);
  void changeOverflowShown(bool shown);

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

  const QxrdMaskRasterData* maskRaster() const;
  QxrdMaskRasterData* maskRaster();

  QxrdDataProcessorPtr processor() const;
  void setDataProcessor(QxrdDataProcessorPtr proc);

  void replot();

  virtual QwtText trackerText(const QwtDoublePoint &pos);

  void contextMenuEvent(QContextMenuEvent *event);

private:
  void replotImage();
  void setImage(QxrdRasterData data);
  void setMask(QxrdMaskRasterData data);
  void setOverflows(QxrdMaskRasterData overflow);
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

  QxrdDoubleImageDataPtr     m_Data;
  QxrdMaskDataPtr            m_Mask;
  QxrdMaskDataPtr            m_Overflow;

  QxrdRasterData             m_DataRaster;
  QxrdMaskRasterData         m_MaskRaster;
  QxrdMaskRasterData         m_OverflowRaster;

  QwtPlotSpectrogramPtr      m_DataImage;
  QwtPlotSpectrogramPtr      m_MaskImage;
  QwtPlotSpectrogramPtr      m_OverflowImage;
//  QxrdPlotImagePtr           m_PlotImage;
  QwtLinearColorMap          m_ColorMap;

  QxrdMaskColorMap           m_MaskColorMap;
  int                        m_MaskAlpha;

  QxrdMaskColorMap           m_OverflowColorMap;
  int                        m_OverflowAlpha;

  QxrdDataProcessorPtr       m_DataProcessor;

  QxrdCenterFinderPickerPtr  m_CenterFinderPicker;
  QwtPlotMarkerPtr           m_CenterMarker;

  QxrdMaskPickerPtr          m_Circles;
  QxrdMaskPickerPtr          m_Polygons;

//  QPen                       m_Pen;
  bool                 m_FirstTime;
};

#endif
