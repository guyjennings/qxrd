#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include "qcepplot.h"

#include <qwt_color_map.h>
#include <qwt_plot_rescaler.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_marker.h>

#include "qcepobjectnamer.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

#include "qxrddataprocessor-ptr.h"
#include "qxrdrasterdata.h"
#include "qxrdmaskrasterdata.h"
#include "qxrdmaskcolormap.h"
#include "qxrdhistogramselector-ptr.h"
#include "qxrdplotslicer-ptr.h"
#include "qxrdimageplotmeasurer-ptr.h"
#include "qxrdcenterfinderpicker-ptr.h"
#include "qxrdmaskpicker-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdimageplotsettings.h"
#include "qxrdpowderpointpicker.h"
#include "qxrdroicoordinateslistmodel-ptr.h"
#include <QItemSelectionModel>

class QxrdImagePlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdImagePlot(QWidget *parent = 0);
  void init(QxrdImagePlotSettingsWPtr settings);

signals:
  void slicePolygon(QVector<QPointF> poly);
  void selectHistogram(QRectF rect);

public slots:
  void autoScale();
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

  void redoColorMap();
  void setColorMap(int index);

  void toggleShowImage();
  void toggleShowMask();
  void toggleShowOverflow();

  void changeImageShown(bool shown);
  void changeMaskShown(bool shown);
  void changeOverflowShown(bool shown);

  void onInterpolateChanged(bool interp);
  void onMaintainAspectChanged(bool interp);

  void onProcessedImageAvailable(QcepImageDataBasePtr image, QcepMaskDataPtr overflow);
  void onMaskedImageAvailable(QcepImageDataBasePtr image, QcepMaskDataPtr mask);
  void onDarkImageAvailable(QcepImageDataBasePtr image);
  void onCenterXChanged(double cx);
  void onCenterYChanged(double cy);
  void onCenterChanged(QPointF c);
  void onMarkedPointsChanged();
  void onImageScaleChanged();

  void enableZooming();
  void enableCentering();
  void enableSlicing();
  void enableMeasuring();
  void enableHistograms();
  void enableMaskCircles();
  void enableMaskPolygons();
  void enablePowderPoints();

  void clearPowderMarkers();
  void displayPowderMarkers();

  void enableContextMenu();
  void disableContextMenu();

  void zapPixel(int x, int y);

  virtual void onLegendChecked(const QVariant &itemInfo, bool on, int index);

public:
  QxrdImagePlotSettingsWPtr imagePlotSettings();

  const QxrdRasterData* raster() const;
  QxrdRasterData* raster();

  const QxrdMaskRasterData* maskRaster() const;
  QxrdMaskRasterData* maskRaster();

  QxrdDataProcessorWPtr processor() const;
  void setProcessor(QxrdDataProcessorWPtr proc);

  void replot();

  virtual QwtText trackerTextF(const QPointF &pos);

  void contextMenuEvent(QContextMenuEvent *event);

  void enableROIDisplay(bool enable);
  void setROIModel(QxrdROICoordinatesListModelPtr model);
  void setROISelection(QItemSelectionModel *select);

private:
  void replotImage();
  void setImage(QxrdRasterData *data);
  void setMask(QxrdMaskRasterData *data);
  void setOverflows(QxrdMaskRasterData *overflow);

  void colorMapStart(QColor startColor, QColor endColor);
  void colorMapRange(double value1, QColor color1, double value2, QColor color2);
  void changedColorMap();
  void setTrackerPen(const QPen &pen);

  void disablePickers();

  void clearROIDisplay();
  void updateROIDisplay();
  void selectROIItem(int n, bool selected);
  void updateROISelection(const QItemSelection &selected,
                          const QItemSelection &deselected);

public:
  enum {
    PercentageMode,
    PercentileMode,
    AbsoluteMode
  };

private:
  QcepObjectNamer            m_ObjectNamer;
  QxrdImagePlotSettingsWPtr  m_ImagePlotSettings;
  QwtPlotRescaler           *m_Rescaler;
  QxrdPlotSlicer            *m_Slicer;
  QxrdImagePlotMeasurer     *m_Measurer;
  QxrdHistogramSelector     *m_HistogramSelector;
//  QwtLegendPtr               m_Legend;

  QcepImageDataBasePtr       m_Data;
  QcepMaskDataPtr            m_Mask;
  QcepMaskDataPtr            m_Overflow;

  QxrdRasterData            *m_DataRaster;
  QxrdMaskRasterData        *m_MaskRaster;
  QxrdMaskRasterData        *m_OverflowRaster;

  QwtPlotSpectrogram        *m_DataImage;
  QwtPlotSpectrogram        *m_MaskImage;
  QwtPlotSpectrogram        *m_OverflowImage;
  QwtLinearColorMap         *m_ColorMap;

  QxrdMaskColorMap          *m_MaskColorMap;
  int                        m_MaskAlpha;

  QxrdMaskColorMap          *m_OverflowColorMap;
  int                        m_OverflowAlpha;

  QxrdDataProcessorWPtr      m_DataProcessor;

  QxrdCenterFinderPicker    *m_CenterFinderPicker;
  QwtPlotMarker             *m_CenterMarker;

  QxrdMaskPicker            *m_Circles;
  QxrdMaskPicker            *m_Polygons;

  QxrdPowderPointPicker     *m_PowderPointPicker;
  QVector<QwtPlotCurve*>     m_PowderPointCurves;

//  QPen                       m_Pen;
  bool                       m_FirstTime;

  bool                       m_ContextMenuEnabled;

  bool                           m_ROIDisplayed;
  QxrdROICoordinatesListModelPtr m_ROIModel;
  QItemSelectionModel           *m_ROISelection;
  QVector<QwtPlotCurve*>         m_ROICurves;
};

#endif
