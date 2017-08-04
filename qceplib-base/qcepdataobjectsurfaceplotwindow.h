#ifndef QCEPDATAOBJECTSURFACEPLOTWINDOW_H
#define QCEPDATAOBJECTSURFACEPLOTWINDOW_H

#include "qcepdataobjectwindow.h"
#include "ui_qcepdataobjectsurfaceplotwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepimagedata-ptr.h"

class QcepDataObjectSurfacePlotWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectSurfacePlotWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectSurfacePlotWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj,
      QWidget *parent = 0);

  virtual ~QcepDataObjectSurfacePlotWindow();

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:
  void onNewImageAvailable(QcepDoubleImageDataPtr img);
};

#endif // QCEPDATAOBJECTSURFACEPLOTWINDOW_H
