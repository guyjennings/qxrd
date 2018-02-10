#ifndef QXRDIMAGEPLOTWIDGET_H
#define QXRDIMAGEPLOTWIDGET_H

#include "qxrdplotwidget.h"
#include "qxrdimageplotwidgetsettings-ptr.h"
#include "qxrdrasterdata-ptr.h"
#include "qxrdmaskrasterdata-ptr.h"
#include "qxrdoverflowrasterdata-ptr.h"
#include "qwt_plot_spectrogram-ptr.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdImagePlotWidget : public QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdImagePlotWidget(QWidget *parent = 0);
  ~QxrdImagePlotWidget();

  //TODO: change to QcepObjectWPtr
  void initialize(QxrdImagePlotWidgetSettingsWPtr settings,
                  QxrdProcessorWPtr               processor);

  void editPreferences();

  enum {
    PercentageScalingMode,
    PercentileScalingMode,
    AbsoluteScalingMode,
    LastScalingMode
  };

  static QString scalingModeName(int n);

public slots:
  void newImage(QcepDoubleImageDataPtr img);
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

  QcepDoubleImageDataPtr      m_NewImageData;
  QcepDoubleImageDataPtr      m_ImageData;

  QcepMaskDataPtr             m_NewMaskData;
  QcepMaskDataPtr             m_MaskData;

  QxrdRasterData             *m_ImageRaster;
  QxrdMaskRasterData         *m_MaskRaster;
  QxrdOverflowRasterData     *m_OverflowRaster;

  QwtPlotSpectrogram         *m_ImageSpectrogram;
  QwtPlotSpectrogram         *m_MaskSpectrogram;
  QwtPlotSpectrogram         *m_OverflowSpectrogram;

  QwtPlotRescaler            *m_PlotRescaler;

private:
  QxrdImagePlotWidgetSettingsWPtr m_ImageSettings;
  QxrdProcessorWPtr               m_Processor;
};

#endif // QXRDIMAGEPLOTWIDGET_H
