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

#include "qxrdprocessor-ptr.h"
#include "qceprasterdata.h"
#include "qcepmaskrasterdata.h"
#include "qcepmaskcolormap.h"
#include "qxrdimageplotsettings.h"
#include "qxrdpowderpointpicker.h"
#include "qxrdroimodel-ptr.h"
#include <QItemSelectionModel>
#include "qcepplotwidgetdialog-ptr.h"

class QXRD_EXPORT QxrdImagePlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdImagePlot(QWidget *parent = 0);
  //TODO: change to QcepObjectWPtr...
  void initialize(QxrdImagePlotSettingsWPtr settings, QcepObjectWPtr parent);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;

signals:
  void slicePolygon(QVector<QPointF> poly);
  void selectHistogram(QRectF rect);

public slots:
  void autoScale();
  void setAutoRange();

  void toggleShowROI();
  void changeROIShown(bool shown);

//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(QPointF c);
  void onMarkedPointsChanged();

  void clearPowderMarkers();
  void displayPowderMarkers();

//  void enableContextMenu();
//  void disableContextMenu();

  void zapPixel(int x, int y);

  virtual void onLegendChecked(const QVariant &itemInfo, bool on, int index);

public:
//  const QxrdRasterData* raster() const;
//  QxrdRasterData* raster();

  QxrdProcessorWPtr processor() const;
  void setProcessor(QxrdProcessorWPtr proc);

  virtual QwtText trackerTextF(const QPointF &pos);

  void contextMenuEvent(QContextMenuEvent *event);

  void enableROIDisplay(bool enable);

  void setROIModel(QxrdROIModelWPtr model);
  void setROISelection(QItemSelectionModel *select);

  QxrdROIModelWPtr roiModel();
  QItemSelectionModel            *roiSelection();

  void setData(QcepImageDataBaseWPtr data);
  void setMask(QcepMaskDataWPtr mask);

//  void roiMouseSelected(const QVector<QPointF> &p);
//  void roiMouseAdded(const QVector<QPointF> &p);
//  void roiMouseRemoved(const QPointF &pt);
//  void roiMouseRotated(const QVector<QPointF> &p);
//  void roiMouseResized(const QVector<QPointF> &p);

private:

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

protected:
  void selectROILabel(int i, bool on);
  void moveSelectedROICenter(double x, double y);
  void editSelectedROI(double x, double y);

private:
  QcepObjectNamer            m_ObjectNamer;

protected:
  QxrdImagePlotSettingsWPtr  m_ImagePlotSettings;

private:
  QcepObjectWPtr             m_Parent;

  QcepImageDataBaseWPtr      m_Data;
  QcepMaskDataWPtr           m_Mask;
//  QcepMaskDataWPtr            m_Overflow;

//  QxrdRasterData            *m_DataRaster;
//  QxrdMaskRasterData        *m_MaskRaster;

  QxrdProcessorWPtr          m_Processor;

//  QwtPlotMarker             *m_CenterMarker;

//  QxrdPowderPointPicker     *m_PowderPointPicker;
  QVector<QwtPlotCurve*>     m_PowderPointCurves;

  bool                       m_FirstTime;

  bool                       m_ContextMenuEnabled;

  QxrdROIModelWPtr           m_ROIModel;
  QItemSelectionModel       *m_ROISelection;
  QVector<QwtPlotCurve*>     m_ROICurves;
};

#endif
