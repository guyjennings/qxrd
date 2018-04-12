#ifndef QCEPIMAGEPLOTWIDGET_H
#define QCEPIMAGEPLOTWIDGET_H

#include "qceplib_global.h"
#include "qcepplotwidget.h"
#include "qcepimageplotwidgetsettings-ptr.h"
#include "qceprasterdata-ptr.h"
#include "qcepmaskrasterdata-ptr.h"
#include "qcepoverflowrasterdata-ptr.h"
#include "qwt_plot_spectrogram-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepmaskrasterdata-ptr.h"
#include "qcepmaskstack-ptr.h"
#include "qcepcenterfinder-ptr.h"
#include "qceppowderringsmodel-ptr.h"
#include "qceproimodel-ptr.h"

class QCEP_EXPORT QcepImagePlotWidget : public QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QcepImagePlotWidget(QWidget *parent = 0);
  ~QcepImagePlotWidget();

  //TODO: change to QcepObjectWPtr
  void initialize(QcepImagePlotWidgetSettingsWPtr settings);

  void setMaskStack   (QcepMaskStackWPtr masks);
  void setCenterFinder(QcepCenterFinderWPtr center);
  void setPowderRings (QcepPowderRingsModelWPtr powderRings);
  void setROIModel    (QcepROIModelWPtr roiModel);

  void editPreferences();

  enum {
    PercentageScalingMode,
    PercentileScalingMode,
    AbsoluteScalingMode,
    LastScalingMode
  };

  static QString scalingModeName(int n);

public slots:
  void newImage(QcepImageDataBasePtr img);
  void newMask (QcepMaskDataPtr        msk);

  void replotImage();
  void replotMask();

private slots:
  void updateColorMap();
  void updateImage();
  void updateMask();
  void replotGraph();
  void maskChanged();

protected:
  QTimer                      m_ImageTimer;

  QcepImageDataBasePtr        m_NewImageData;
  QcepImageDataBasePtr        m_ImageData;

  QcepMaskDataPtr             m_NewMaskData;
  QcepMaskDataPtr             m_MaskData;

  QcepRasterData             *m_ImageRaster;
  QcepMaskRasterData         *m_MaskRaster;
  QcepOverflowRasterData     *m_OverflowRaster;

  QwtPlotSpectrogram         *m_ImageSpectrogram;
  QwtPlotSpectrogram         *m_MaskSpectrogram;
  QwtPlotSpectrogram         *m_OverflowSpectrogram;

  QwtPlotRescaler            *m_PlotRescaler;

private:
  QcepImagePlotWidgetSettingsWPtr m_ImageSettings;
  QcepMaskStackWPtr               m_MaskStack;
  QcepCenterFinderWPtr            m_CenterFinder;
  QcepPowderRingsModelWPtr        m_PowderRings;
  QcepROIModelWPtr                m_ROIModel;
};

#endif // QCEPIMAGEPLOTWIDGET_H
