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

  void disableCommands(QxrdPlotCommand *except);

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
  QxrdPlotWidgetSettingsWPtr  m_Settings;
  QVector<QxrdPlotCommandPtr> m_PlotCommands;

  QwtLegend                  *m_Legend;
  QwtPlotZoomer              *m_Zoomer;
  QwtPlotPanner              *m_Panner;
  QwtPlotMagnifier           *m_Magnifier;
  QcepPlotMeasurer           *m_Measurer;

  int                         m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOTWIDGET_H
