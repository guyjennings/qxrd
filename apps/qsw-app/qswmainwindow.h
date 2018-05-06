#ifndef QSWMAINWINDOW_H
#define QSWMAINWINDOW_H

#include <QMainWindow>
#include "ui_qswmainwindow.h"
#include "qcepplotsettings-ptr.h"
#include "qcepmainwindow.h"

class QswMainWindow : public QcepMainWindow, public Ui::QswMainWindow
{
  Q_OBJECT

private:
  typedef QcepMainWindow inherited;

public:
  explicit QswMainWindow(QString name);
  ~QswMainWindow();
  void initialize(QcepObjectWPtr parent);

public slots:
  void onDataAvailable(QVector<float> d);
  void doSave();
  void possiblyClose();

  void saveExperiment();
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();

private:
  void fadeoutPlotCurves();

private:
  QcepPlotWidgetSettingsWPtr m_Settings;
  QVector<QwtPlotCurve*>     m_PlotCurves;
};

#endif // QSWMAINWINDOW_H
