#ifndef QCEPPLOTWIDGET_H
#define QCEPPLOTWIDGET_H

#include <QWidget>
#include <QTimer>
#include "ui_qcepplotwidget.h"
#include "qcepplotcommand-ptr.h"
#include "qcepplotwidgetsettings-ptr.h"
#include "qcepplot-ptr.h"
#include "qcepplotoverlay-ptr.h"
#include <qwt_legend.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

//TODO: implement mouse tracking
class QCEP_EXPORT QcepPlotWidget : public QWidget, public Ui::QcepPlotWidget
{
  Q_OBJECT

public:
  explicit QcepPlotWidget(QWidget *parent = 0);
  ~QcepPlotWidget();
  //TODO: change to QcepObjectWPtr
  void initialize(QcepPlotWidgetSettingsWPtr settings);

  QcepPlot *plot();

  void addPlotOverlay(QcepPlotOverlayPtr ovl);

  void addPlotCommand(QcepPlotCommandPtr cmd);
  void addPlotCommandSpacer();

  void disableCommands(QcepPlotCommand *except);

  void enableZooming();
  void disableZooming();


public slots:
  void zoomIn();
  void zoomOut();
  void zoomAll();
  void printGraph();

  virtual void editPreferences();

private slots:
  void contextMenu(const QPoint &pos);

  void setLegendPosition(int legendPosition);

  virtual void onLegendClicked(const QVariant &itemInfo, int index);
  virtual void onLegendChecked(const QVariant &itemInfo, bool on, int index);

private:
  void setAxisVis(int axis, bool isVis);
  void setAxisLog(int axis, bool isLog);

protected:
  void updateTrackerPen(int mapIndex);

protected:
  QcepPlotWidgetSettingsWPtr  m_Settings;
  QVector<QcepPlotCommandPtr> m_PlotCommands;
  QVector<QcepPlotOverlayPtr> m_PlotOverlays;

  QwtLegend                  *m_Legend;
  QwtPlotZoomer              *m_Zoomer;
  QwtPlotPanner              *m_Panner;
  QwtPlotMagnifier           *m_Magnifier;
  QcepPlotMeasurer           *m_Measurer;

  int                         m_IsLog[QwtPlot::axisCnt];
};

#endif // QCEPPLOTWIDGET_H
