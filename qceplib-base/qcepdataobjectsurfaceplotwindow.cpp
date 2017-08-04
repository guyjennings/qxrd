#include "qcepdataobjectsurfaceplotwindow.h"
#include <QMessageBox>
#include "qcepimagedata.h"

using namespace QtDataVisualization;

QcepDataObjectSurfacePlotWindow::QcepDataObjectSurfacePlotWindow(
    QcepExperimentWPtr expt,
    QcepDataObjectWPtr obj,
    QWidget *parent)
  : QcepDataObjectWindow(parent)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  QcepDoubleImageDataPtr img = qSharedPointerDynamicCast<QcepDoubleImageData>(obj);

  onNewImageAvailable(img);
}

QcepDataObjectSurfacePlotWindow::~QcepDataObjectSurfacePlotWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Surface Plot Window\n");
#endif
}

void QcepDataObjectSurfacePlotWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QcepDataObjectSurfacePlotWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QcepDataObjectSurfacePlotWindow::onNewImageAvailable(QcepDoubleImageDataPtr img)
{
  if (img) {
    setWindowTitle(tr("Surface Plot : %1").arg(img->get_Name()));

    m_SurfacePlot->onNewImageAvailable(img);
  }
}
