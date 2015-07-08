#ifndef QXRDDATAOBJECTGRAPHWINDOW_H
#define QXRDDATAOBJECTGRAPHWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qxrddataobjectgraphwindow.h"
#include "qxrdexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qxrddataobjectgraphcontroller-ptr.h"

class QxrdDataObjectGraphWindow : public QMainWindow, public Ui::QxrdDataObjectGraphWindow
{
  Q_OBJECT
public:
  explicit QxrdDataObjectGraphWindow(
      QxrdExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

  int controllerRow() const;
  int controllerColumn() const;

signals:

public slots:

protected:

private slots:
  void setGraphMode(int mode);
  void allowGraphMode(int mode);
  void changeGraphMode(int idx);

private:
  QxrdExperimentWPtr                            m_Experiment;
  QcepDataObjectPtr                             m_Object;
  int                                           m_PlottingMode;
  QSharedPointer<QxrdDataObjectGraphController> m_Controller;

  enum {
    NoPlot,
    DefaultPlot,
    ScatterPlot,
    ImagePlot,
    HistogramPlot,
    HorizontalSlice,
    VerticalSlice
  };
};

#endif // QXRDDATAOBJECTGRAPHWINDOW_H
