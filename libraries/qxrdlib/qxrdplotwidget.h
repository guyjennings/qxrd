#ifndef QXRDPLOTWIDGET_H
#define QXRDPLOTWIDGET_H

#include <QWidget>
#include <QTimer>
#include "ui_qxrdplotwidget.h"
#include "qxrdplotcommand-ptr.h"
#include "qxrdplotwidgetsettings-ptr.h"

//TODO: implement mouse tracking
class QXRD_EXPORT QxrdPlotWidget : public QWidget, public Ui::QxrdPlotWidget
{
  Q_OBJECT

public:
  explicit QxrdPlotWidget(QWidget *parent = 0);
  ~QxrdPlotWidget();
  void initialize(QxrdPlotWidgetSettingsWPtr settings);

  void addPlotCommand(QxrdPlotCommandPtr cmd);
  void addPlotCommandSpacer();

  void disableCommands();

public slots:
  void onProcessedImageAvailable(QcepDoubleImageDataPtr img);

  void zoomIn();
  void zoomOut();
  void zoomAll();
  void printGraph();

private slots:
  void contextMenu(const QPoint &pos);
  void updateDisplayedImage();

  void setLegendPosition(int legendPosition);

  virtual void onLegendClicked(const QVariant &itemInfo, int index);
  virtual void onLegendChecked(const QVariant &itemInfo, bool on, int index);

  void setXAxisLog(bool isLog);
  void setYAxisLog(bool isLog);
  void setX2AxisLog(bool isLog);
  void setY2AxisLog(bool isLog);

private:
  void setLogAxis(int axis, int isLog);
  int logAxis(int axis);

protected:
  QxrdPlotWidgetSettingsWPtr  m_Settings;
  QTimer                      m_Timer;
  QVector<QxrdPlotCommandPtr> m_PlotCommands;

  QcepDoubleImageDataPtr      m_NewImageData;
  QcepDoubleImageDataPtr      m_ImageData;

  QwtLegend                  *m_Legend;
  QwtPlotZoomer              *m_Zoomer;
  QwtPlotPanner              *m_Panner;
  QwtPlotMagnifier           *m_Magnifier;
  QcepPlotMeasurer           *m_Measurer;

  int                         m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOTWIDGET_H
