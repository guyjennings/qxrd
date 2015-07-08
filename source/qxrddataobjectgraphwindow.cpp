#include "qxrddataobjectgraphwindow.h"
#include "qxrdexperiment.h"
#include "qcepdataobject.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"

#include "qxrdintegrateddata-ptr.h"
#include "qxrdintegrateddata.h"

#include "qcepdataarray.h"
#include "qcepdataarray-ptr.h"

#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

#include "qcepdatacolumnscan.h"
#include "qcepdatacolumnscan-ptr.h"

#include "qcepdatagroup.h"
#include "qcepdatagroup-ptr.h"

#include "qxrdimagedatagraphcontroller.h"
#include "qxrdscatterplotgraphcontroller.h"
#include "qxrdimagehistogramgraphcontroller.h"
#include "qxrdimageslicegraphcontroller.h"

QxrdDataObjectGraphWindow::QxrdDataObjectGraphWindow(
    QxrdExperimentWPtr expt, QcepDataObjectPtr obj, QWidget *parent) :
  QMainWindow(parent),
  m_Experiment(expt),
  m_Object(obj),
  m_PlottingMode(NoPlot)
{
  setupUi(this);

  QxrdExperimentPtr e(m_Experiment);

  if (m_Object && expt) {
    setWindowTitle(tr("%1 Graph from %2")
                   .arg(m_Object->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (m_Object) {
    setWindowTitle(tr("%1 Graph")
                   .arg(m_Object->pathName()));
  } else {
    setWindowTitle("Unknown Graph");
  }

  m_PlotModeSelector->clear();

  setGraphMode(DefaultPlot);

  connect(m_PlotModeSelector, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changeGraphMode(int)));

  setAttribute(Qt::WA_DeleteOnClose, true);
}

void QxrdDataObjectGraphWindow::setGraphMode(int mode)
{
  if (mode != m_PlottingMode) {
    QSharedPointer<QcepImageDataBase> image = qSharedPointerDynamicCast<QcepImageDataBase>(m_Object);
    QxrdIntegratedDataPtr             integ = qSharedPointerDynamicCast<QxrdIntegratedData>(m_Object);
    QcepDataArrayPtr                  array = qSharedPointerDynamicCast<QcepDataArray>(m_Object);
    QcepDataColumnPtr                 col   = qSharedPointerDynamicCast<QcepDataColumn>(m_Object);
    QcepDataColumnScanPtr             scan  = qSharedPointerDynamicCast<QcepDataColumnScan>(m_Object);
    QcepDataGroupPtr                  group = qSharedPointerDynamicCast<QcepDataGroup>(m_Object);

    if (image || array) {
      allowGraphMode(ImagePlot);
      allowGraphMode(HistogramPlot);
      allowGraphMode(HorizontalSlice);
      allowGraphMode(VerticalSlice);

      if (mode == DefaultPlot || mode == ImagePlot) {
        m_Controller = QSharedPointer<QxrdImageDataGraphController>(
              new QxrdImageDataGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = ImagePlot;
      } else if (mode == HistogramPlot) {
        m_Controller = QSharedPointer<QxrdImageHistogramGraphController>(
              new QxrdImageHistogramGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = HistogramPlot;
      } else if (mode == HorizontalSlice || mode == VerticalSlice) {
        m_Controller = QSharedPointer<QxrdImageSliceGraphController>(
              new QxrdImageSliceGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = mode;
      }
    } else if (integ || col || scan || group) {
      allowGraphMode(ScatterPlot);

      if (mode == DefaultPlot || mode == ScatterPlot) {
        m_Controller = QSharedPointer<QxrdScatterPlotGraphController> (
              new QxrdScatterPlotGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = ScatterPlot;
      }
    }

    int idx = m_PlotModeSelector->findData(mode);

    if (idx >= 0) {
      m_PlotModeSelector->setCurrentIndex(idx);
    }
  }
}

void QxrdDataObjectGraphWindow::allowGraphMode(int mode)
{
  switch (mode) {
  case ImagePlot:
    m_PlotModeSelector->addItem("2-D Image Plot", ImagePlot);
    break;

  case ScatterPlot:
    m_PlotModeSelector->addItem("Scatter Plot", ScatterPlot);
    break;

  case HistogramPlot:
    m_PlotModeSelector->addItem("Histogram of Image", HistogramPlot);
    break;

  case HorizontalSlice:
    m_PlotModeSelector->addItem("Horizontal Slices", HorizontalSlice);
    break;

  case VerticalSlice:
    m_PlotModeSelector->addItem("Vertical Slices", VerticalSlice);
    break;
  }
}
void QxrdDataObjectGraphWindow::changeGraphMode(int idx)
{
  QString newName = m_PlotModeSelector->itemText(idx);
  int     newMode = m_PlotModeSelector->itemData(idx).toInt();

  bool b = m_PlotModeSelector->blockSignals(true);

  if (QMessageBox::question(this,
                            tr("Change Plotting Mode?"),
                            tr("Do you want to change the plotting mode to %1").arg(newName),
                            QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {

    m_PlotModeSelector->clear();

    setGraphMode(newMode);
  } else {
    int oldIdx = m_PlotModeSelector->findData(m_PlottingMode);

    if (oldIdx >= 0) {
      m_PlotModeSelector->setCurrentIndex(oldIdx);
    }
  }

  m_PlotModeSelector->blockSignals(b);
}

void QxrdDataObjectGraphWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QxrdDataObjectGraphWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

int QxrdDataObjectGraphWindow::controllerRow() const
{
  return 1;
}

int QxrdDataObjectGraphWindow::controllerColumn() const
{
  return 2;
}
