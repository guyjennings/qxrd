#ifndef QXRDIMAGEPLOT_H
#define QXRDIMAGEPLOT_H

#include "qxrdlib_global.h"
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

class QXRD_EXPORT QxrdImagePlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdImagePlot(QWidget *parent = 0);
  void init(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;

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
  void toggleShowROI();
  void toggleLogDisplay();

  void changeImageShown(bool shown);
  void changeMaskShown(bool shown);
  void changeOverflowShown(bool shown);
  void changeROIShown(bool shown);
  void changeLogDisplay(bool isLog);

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

  void setROIModel(QxrdROICoordinatesListModelWPtr model);
  void setROISelection(QItemSelectionModel *select);

  QxrdROICoordinatesListModelWPtr roiModel();
  QItemSelectionModel            *roiSelection();

//  void roiMouseSelected(const QVector<QPointF> &p);
//  void roiMouseAdded(const QVector<QPointF> &p);
//  void roiMouseRemoved(const QPointF &pt);
//  void roiMouseRotated(const QVector<QPointF> &p);
//  void roiMouseResized(const QVector<QPointF> &p);

private:
  void replotImage();
  void setImage(QxrdRasterData *data);
  void setMask(QxrdMaskRasterData *data);
  void setOverflows(QxrdMaskRasterData *overflow);
  void setAutoOverflow();
  void colorMapStart(QColor startColor, QColor endColor);
  void colorMapRange(double value1, QColor color1, double value2, QColor color2);
  void changedColorMap();
  void setTrackerPen(const QPen &pen);


  void clearROIDisplay();
  void updateROIDisplay();
  void selectROIItem(int n, bool selected);
  void updateROISelection(const QItemSelection &selected,
                          const QItemSelection &deselected);

  void roiDataChanged (const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int> ());

  void roiRowsInserted(const QModelIndex &parent, int first, int last);

  void roiRowsMoved   (const QModelIndex &parent, int start, int end,
                       const QModelIndex &destination, int row);

  void roiRowsRemoved (const QModelIndex &parent, int first, int last);

  QPointF scaledDelta(double dx, double dy);

protected:
  virtual void disablePickers();
  void selectROILabel(int i, bool on);
  void moveSelectedROICenter(double x, double y);
  void editSelectedROI(double x, double y);

public:
  enum {
    PercentageMode,
    PercentileMode,
    AbsoluteMode
  };

private:
  QcepObjectNamer            m_ObjectNamer;

protected:
  QxrdImagePlotSettingsWPtr  m_ImagePlotSettings;

private:
  QcepObjectWPtr             m_Parent;
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

  QxrdROICoordinatesListModelWPtr m_ROIModel;
  QItemSelectionModel           *m_ROISelection;
  QVector<QwtPlotCurve*>         m_ROICurves;
};

#endif