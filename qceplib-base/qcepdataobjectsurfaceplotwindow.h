#ifndef QCEPDATAOBJECTSURFACEPLOTWINDOW_H
#define QCEPDATAOBJECTSURFACEPLOTWINDOW_H

#ifdef HAVE_DATAVIS

#include "qceplib_global.h"
#include "qcepdataobjectwindow.h"
#include "ui_qcepdataobjectsurfaceplotwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepDataObjectSurfacePlotWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectSurfacePlotWindow
{
  Q_OBJECT

private:
  typedef QcepDataObjectWindow inherited;

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
  void onNewImageAvailable(QcepDoubleImageDataWPtr img);
};

#endif // HAVE_DATAVIS

#endif // QCEPDATAOBJECTSURFACEPLOTWINDOW_H
