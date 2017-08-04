#ifndef QCEPDATAOBJECTSURFACEPLOTWINDOW_H
#define QCEPDATAOBJECTSURFACEPLOTWINDOW_H

#include "qcepdataobjectwindow.h"
#include "ui_qcepdataobjectsurfaceplotwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include <QtDataVisualization>
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

signals:

public slots:
  void onNewImageAvailable(QcepDoubleImageDataPtr img);

private:
  QcepExperimentWPtr                            m_Experiment;
  QcepDataObjectWPtr                            m_Object;

  QtDataVisualization::Q3DSurface              *m_SurfaceGraph;
  QtDataVisualization::QSurface3DSeries        *m_Surface;
};

#endif // QCEPDATAOBJECTSURFACEPLOTWINDOW_H
