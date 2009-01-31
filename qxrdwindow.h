#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include "ui_qxrdwindow.h"

class QCheckBox;
class QxrdApplication;
class QxrdAcquisition;
class QwtPlotZoomer;
class QwtLegend;
class QxrdHighlighter;

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

 public:
  QxrdWindow(QxrdApplication *app, QWidget *parent=0);

  const QPushButton *acquireButton();
  const QAction     *acquireAction();

  double integrationTime();
  int    nRepetitions();

 public slots:
  void newDataAvailable();
  void viewScalingChanged();
  void viewParametersChanged();
  void autoScale();
  void viewAllData();
  void setAcquisition(QxrdAcquisition *acq);
  void chooserClicked(int n);

 public:
  void updateGraph();
  QPen curvePen(int ch);

 private:
  QxrdApplication  *m_Application;
  QxrdAcquisition  *m_Acquisition;
  QwtPlotZoomer     *m_Zoomer;
  QwtLegend         *m_Legend;
  int                m_XUnits;
  double             m_XStep;
  QxrdHighlighter  *m_Highlighter;
  QVector< QCheckBox*> m_CheckBoxes;
};

#endif
