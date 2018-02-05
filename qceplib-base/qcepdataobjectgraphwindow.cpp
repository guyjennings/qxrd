#include "qcepdataobjectgraphwindow.h"
#include "qcepproperty.h"
#include "qcepexperiment.h"
#include "qcepdataobject.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"

#include "qcepintegrateddata-ptr.h"
#include "qcepintegrateddata.h"

#include "qcepdataarray.h"
#include "qcepdataarray-ptr.h"

#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"

#include "qcepdatacolumnscan.h"
#include "qcepdatacolumnscan-ptr.h"

#include "qcepdatagroup.h"
#include "qcepdatagroup-ptr.h"

#include "qcepplotsettings.h"

#include "qcepimagedatagraphcontroller.h"
#include "qcepscatterplotgraphcontroller.h"
#include "qcepimagehistogramgraphcontroller.h"
#include "qcepimageslicegraphcontroller.h"

QcepDataObjectGraphWindow::QcepDataObjectGraphWindow(QcepExperimentWPtr expt, QcepDataObjectWPtr obj, QWidget *parent) :
  inherited(parent),
  m_Experiment(expt),
  m_Object(obj),
  m_PlottingMode(NoPlot),
//  m_SettingsSaver(new QcepSettingsSaver(this)),
//  m_PlotSettings(new QcepPlotSettings("plotSettings", m_SettingsSaver, NULL)),
  m_ColorMap     (NULL, "colorMap",      0,     "Image Color Map Index"),
  m_ScalingMode  (NULL, "scalingMode",   0,     "Image Scaling Mode"),
  m_MinimumPct   (NULL, "minimumPct",    0,     "Image Display Minimum %"),
  m_MaximumPct   (NULL, "maximumPct",    100,   "Image Display Maximum %"),
  m_MinimumPctle (NULL, "minimumPctle",  0,     "Image Display Minimum Percentile"),
  m_MaximumPctle (NULL, "maximumPctle",  99,    "Image Display Maximum Percentile"),
  m_MinimumVal   (NULL, "minimumVal",    0,     "Image Display Minimum Value"),
  m_MaximumVal   (NULL, "maximumVal",    10000, "Image Display Maximum Value"),
  m_HistFullVis  (NULL, "histFullVis",   true,  "Full Image Histogram Visible?"),
  m_HistAVis     (NULL, "histAVis",      false, "Histogram A Visible?"),
  m_HistBVis     (NULL, "histBVis",      false, "Histogram B Visible?"),
  m_HistCVis     (NULL, "histCVis",      false, "Histogram C Visible"),
  m_HistALeft    (NULL, "histALeft",     10,    "Histogram A Left"),
  m_HistATop     (NULL, "histATop",      10,    "Histogram A Top"),
  m_HistARight   (NULL, "histARight",    20,    "Histogram A Right"),
  m_HistABottom  (NULL, "histABottom",   20,    "Histogram A Bottom"),
  m_HistBLeft    (NULL, "histBLeft",     30,    "Histogram B Left"),
  m_HistBTop     (NULL, "histBTop",      30,    "Histogram B Top"),
  m_HistBRight   (NULL, "histBRight",    40,    "Histogram B Right"),
  m_HistBBottom  (NULL, "histBBottom",   40,    "Histogram B Bottom"),
  m_HistCLeft    (NULL, "histCLeft",     50,    "Histogram B Left"),
  m_HistCTop     (NULL, "histCTop",      50,    "Histogram B Top"),
  m_HistCRight   (NULL, "histCRight",    60,    "Histogram B Right"),
  m_HistCBottom  (NULL, "histCBottom",   60,    "Histogram B Bottom"),
  m_SliceHStart  (NULL, "sliceHStart",   0,     "Horizontal Slice Start"),
  m_SliceHSummed (NULL, "sliceHSummed",  1,     "Horizontal Slice Summed"),
  m_SliceHSkipped(NULL, "sliceHSkipped", 0,     "Horizontal Slice Skipped"),
  m_SliceHRepeats(NULL, "sliceHRepeats", 100,   "Horizontal Slice Repeats"),
  m_SliceVStart  (NULL, "sliceVStart",   0,     "Vertical Slice Start"),
  m_SliceVSummed (NULL, "sliceVSummed",  1,     "Vertical Slice Summed"),
  m_SliceVSkipped(NULL, "sliceVSkipped", 0,     "Vertical Slice Skipped"),
  m_SliceVRepeats(NULL, "sliceVRepeats", 100,   "Vertical Slice Repeats")
{
  setupUi(this);

  QcepExperimentPtr e(m_Experiment);
  QcepDataObjectPtr objp(m_Object);

  if (objp && expt) {
    setWindowTitle(tr("%1 Graph from %2")
                   .arg(objp->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (objp) {
    setWindowTitle(tr("%1 Graph")
                   .arg(objp->pathName()));
  } else {
    setWindowTitle("Unknown Graph");
  }

  QcepPlotSettingsPtr settings(new QcepPlotSettings("plotSettings"));

  m_ImagePlot->initialize(settings);

  m_PlotModeSelector->clear();

  setGraphMode(DefaultPlot);

  connect(m_PlotModeSelector, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
          this, &QcepDataObjectGraphWindow::changeGraphMode);

  connect(m_ImageZoomInButton, &QAbstractButton::clicked, m_ImagePlot, &QcepPlot::enableZooming);
  connect(m_ImageZoomOutButton, &QAbstractButton::clicked, m_ImagePlot, &QcepPlot::zoomOut);
  connect(m_ImageZoomAllButton, &QAbstractButton::clicked, m_ImagePlot, &QcepPlot::autoScale);
  connect(m_ImageMeasureButton, &QAbstractButton::clicked, m_ImagePlot, &QcepPlot::enableMeasuring);

  m_ImagePlot->enableZooming();

  setAttribute(Qt::WA_DeleteOnClose, true);

  if (objp) {
    connect(objp.data(), &QcepDataObject::dataObjectChanged, this, &QcepDataObjectGraphWindow::updateDisplay);

    updateDisplay();
  }
}

QcepDataObjectGraphWindow::~QcepDataObjectGraphWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Graph Window\n");
#endif
}

QcepExperimentWPtr QcepDataObjectGraphWindow::experiment()
{
  return m_Experiment;
}

void QcepDataObjectGraphWindow::setGraphMode(int mode)
{
  if (mode != m_PlottingMode) {
    QSharedPointer<QcepImageDataBase> image = qSharedPointerDynamicCast<QcepImageDataBase>(m_Object);
    QcepIntegratedDataPtr             integ = qSharedPointerDynamicCast<QcepIntegratedData>(m_Object);
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
        m_Controller = QSharedPointer<QcepImageDataGraphController>(
              new QcepImageDataGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = ImagePlot;
      } else if (mode == HistogramPlot) {
        m_Controller = QSharedPointer<QcepImageHistogramGraphController>(
              new QcepImageHistogramGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = HistogramPlot;
      } else if (mode == HorizontalSlice || mode == VerticalSlice) {
        m_Controller = QSharedPointer<QcepImageSliceGraphController>(
              new QcepImageSliceGraphController(this, mode, m_Object));

        m_Controller->activate();

        m_PlottingMode = mode;
      }
    } else if (integ || col || scan || group) {
      allowGraphMode(ScatterPlot);

      if (mode == DefaultPlot || mode == ScatterPlot) {
        m_Controller = QSharedPointer<QcepScatterPlotGraphController> (
              new QcepScatterPlotGraphController(this, mode, m_Object));

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

void QcepDataObjectGraphWindow::allowGraphMode(int mode)
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

void QcepDataObjectGraphWindow::changeGraphMode(int idx)
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

    updateDisplay();
  } else {
    int oldIdx = m_PlotModeSelector->findData(m_PlottingMode);

    if (oldIdx >= 0) {
      m_PlotModeSelector->setCurrentIndex(oldIdx);
    }
  }

  m_PlotModeSelector->blockSignals(b);
}

int QcepDataObjectGraphWindow::currentGraphMode()
{
  return m_PlottingMode;
}

void QcepDataObjectGraphWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QcepDataObjectGraphWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

int QcepDataObjectGraphWindow::controllerRow() const
{
  return 1;
}

int QcepDataObjectGraphWindow::controllerColumn() const
{
  return 2;
}

void QcepDataObjectGraphWindow::updateDisplay()
{
  if (m_Controller) {
    m_Controller->updateDisplay();
  }
}

void QcepDataObjectGraphWindow::clearPlot()
{
  m_ImagePlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
}

void QcepDataObjectGraphWindow::appendCurve(QwtPlotCurve *curve)
{
  if (curve) {
    curve->attach(m_ImagePlot);
  }
}
