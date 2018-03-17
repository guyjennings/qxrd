#include "qxrdextraiowindow.h"
#include "ui_qxrdextraiowindow.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrdsynchronizedoutputchannel.h"
#include "qxrdsynchronizedinputchannel.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdacquisitionparameterpack.h"
//#include "qxrdacquisitionextrainputs.h"
#include <QCheckBox>
#include <QMessageBox>
#include "qxrdextraiowindowsettings.h"
#include <QThread>
#include "qxrdnidaq.h"
#include "qxrdextraiodetectorsmodel.h"
#include "qxrdextraiooutputsmodel.h"
#include "qxrdextraioinputsmodel.h"
#include <QItemSelectionModel>
#include "qxrdextraiodetectorsdelegate.h"
#include "qxrdextraiooutputsdelegate.h"
#include "qxrdextraioinputsdelegate.h"

QxrdExtraIOWindow::QxrdExtraIOWindow(QString name) :
  inherited(name)/*,
  m_ChannelsInRows(0)*/
{
}

void QxrdExtraIOWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  if (exp) {
    QxrdAcqCommonPtr acq(exp->acquisition());

    if (acq) {
      m_SynchronizedAcquisition = acq->synchronizedAcquisition();
      m_NIDAQPlugin             = acq->nidaqPlugin();

      QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

      if (sync) {
        connect(sync.data(), &QxrdSynchronizedAcquisition::waveformsChanged,
                this,        &QxrdExtraIOWindow::updateWaveforms);


        QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

        if (nidaq) {
          for (int i=0; i<nidaq->detectorDeviceCount(); i++) {
            m_DetectorCounter -> addItem(nidaq->detectorDeviceName(i));
          }
        }

        sync->prop_PrimaryCounter()   -> linkTo(m_DetectorCounter);
        sync->prop_OutputSampleRate() -> linkTo(m_OutputSampleRate);
        sync->prop_InputSampleRate()  -> linkTo(m_InputSampleRate);
      }

      connect(acq->prop_ExposureTime(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
      connect(acq->prop_PhasesInGroup(), &QcepIntProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);

      waveformChanged();

      updateWaveforms();
    }

    connect(m_TestReadout, &QAbstractButton::clicked, this, &QxrdExtraIOWindow::initiateReadout);
  }

  connect(m_NewDetectorButton,    &QToolButton::clicked, this, &QxrdExtraIOWindow::doNewDetector);
  connect(m_NewOutputButton,      &QToolButton::clicked, this, &QxrdExtraIOWindow::doNewOutput);
  connect(m_NewInputButton,       &QToolButton::clicked, this, &QxrdExtraIOWindow::doNewInput);
  connect(m_DeleteDetectorButton, &QToolButton::clicked, this, &QxrdExtraIOWindow::doDeleteDetector);
  connect(m_DeleteOutputButton,   &QToolButton::clicked, this, &QxrdExtraIOWindow::doDeleteOutput);
  connect(m_DeleteInputButton,    &QToolButton::clicked, this, &QxrdExtraIOWindow::doDeleteInput);

  m_DetectorsModel =
      QxrdExtraIODetectorsModelPtr(
        new QxrdExtraIODetectorsModel(m_SynchronizedAcquisition));

  m_OutputsModel =
      QxrdExtraIOOutputsModelPtr(
        new QxrdExtraIOOutputsModel(m_SynchronizedAcquisition));

  m_InputsModel =
      QxrdExtraIOInputsModelPtr(
        new QxrdExtraIOInputsModel(m_SynchronizedAcquisition));

  m_DetectorsTable -> setModel(m_DetectorsModel.data());
  m_OutputsTable   -> setModel(m_OutputsModel.data());
  m_InputsTable    -> setModel(m_InputsModel.data());

  m_DetectorsTable -> setItemDelegate(new QxrdExtraIODetectorsDelegate(m_SynchronizedAcquisition));
  m_OutputsTable   -> setItemDelegate(new QxrdExtraIOOutputsDelegate(m_SynchronizedAcquisition));
  m_InputsTable    -> setItemDelegate(new QxrdExtraIOInputsDelegate(m_SynchronizedAcquisition));

  m_DetectorsTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
  m_OutputsTable   -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
  m_InputsTable    -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

  QxrdExtraIOWindowSettingsPtr settings(
        qSharedPointerDynamicCast<QxrdExtraIOWindowSettings>(m_Parent));

  if (settings) {
    m_InputWaveforms  -> initialize(settings->extraInputsPlotWidgetSettings());
    m_OutputWaveforms -> initialize(settings->extraOutputsPlotWidgetSettings());
  }
}

QxrdExtraIOWindow::~QxrdExtraIOWindow()
{
}

void QxrdExtraIOWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdExtraIOWindow::waveformChanged()
{
  QxrdAcqCommonPtr               acq(QxrdAcqCommon::findAcquisition(m_Parent));
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (acq && sync) {
    QxrdAcquisitionParameterPackPtr parms(acq->acquisitionParameterPack());

    if (parms) {
      sync->prepareForAcquisition(parms);

      sync->finishedAcquisition();
    }
  }
}


void QxrdExtraIOWindow::initiateReadout()
{
//  QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//  if (xtra) {
//    xtra->initiate();
//  }
}

void QxrdExtraIOWindow::updateInputWaveforms()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    m_InputWaveforms -> clear();

    for (int i=0; i<sync->inputCount(); i++) {
      QxrdSynchronizedInputChannelPtr inp(sync->input(i));

      if (inp) {
        m_InputWaveforms -> plotChannel(inp);
      }
    }

    m_InputWaveforms -> replot();
  }
}

void QxrdExtraIOWindow::updateOutputWaveforms()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    m_OutputWaveforms -> clear();

    for (int i=0; i<sync->outputCount(); i++) {
      QxrdSynchronizedOutputChannelPtr out(sync->output(i));

      if (out) {
        m_OutputWaveforms -> plotChannel(out);
      }
    }

    m_OutputWaveforms -> replot();
  }
}

void QxrdExtraIOWindow::updateWaveforms()
{
  updateInputWaveforms();
  updateOutputWaveforms();
}

void QxrdExtraIOWindow::doNewDetector()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_DetectorsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->detectorCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    int dest = sync->detectorCount();

    if (selectedRows.count()) {
      dest = selectedRows.first();
    }

    if (askToInsert("Detector", dest, sync->detectorCount())) {
      m_DetectorsModel -> newDetector(dest);
    }
  }
}

void QxrdExtraIOWindow::doNewOutput()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_OutputsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->outputCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    int dest = sync->outputCount();

    if (selectedRows.count()) {
      dest = selectedRows.first();
    }

    if (askToInsert("Output Channel", dest, sync->outputCount())) {
      m_OutputsModel -> newOutput(dest);
    }
  }
}

void QxrdExtraIOWindow::doNewInput()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_InputsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->inputCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    int dest = sync->inputCount();

    if (selectedRows.count()) {
      dest = selectedRows.first();
    }

    if (askToInsert("Input Channel", dest, sync->inputCount())) {
      m_InputsModel -> newInput(dest);
    }
  }
}

bool QxrdExtraIOWindow::askToInsert(QString thing, int before, int count)
{
  bool res = false;

  QString titleString, question;

  if (before < count) {
    titleString = tr("New %1 before %2 %3")
        .arg(thing).arg(thing).arg(before);
    question    = tr("Do you really want to create a new %1 before %2 %3")
        .arg(thing).arg(thing).arg(before);
  } else {
    titleString = tr("New %1 after %2 %3")
        .arg(thing).arg(thing).arg(before-1);
    question    = tr("Do you really want to create a new %1 after %2 %3")
        .arg(thing).arg(thing).arg(before-1);
  }

  int reply = QMessageBox::question(this, titleString, question,
                                    QMessageBox::Ok | QMessageBox::Cancel,
                                    QMessageBox::Cancel);

  if (reply == QMessageBox::Ok) {
    res = true;
  }

  return res;
}

void QxrdExtraIOWindow::doDeleteDetector()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_DetectorsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->detectorCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    if (askToDelete("Detector", selectedRows)) {
      m_DetectorsModel -> deleteDetectors(selectedRows);
    }
  }
}

void QxrdExtraIOWindow::doDeleteOutput()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_OutputsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->outputCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    if (askToDelete("Output Channel", selectedRows)) {
      m_OutputsModel -> deleteOutputs(selectedRows);
    }
  }
}

void QxrdExtraIOWindow::doDeleteInput()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);
  QItemSelectionModel *sel = m_InputsTable->selectionModel();
  QVector<int> selectedRows;

  if (sync && sel) {
    for (int i=0; i<sync->inputCount(); i++) {
      if (sel->rowIntersectsSelection(i, QModelIndex())) {
        selectedRows.append(i);
      }
    }

    if (askToDelete("Input Channel", selectedRows)) {
      m_InputsModel -> deleteInputs(selectedRows);
    }
  }
}

bool QxrdExtraIOWindow::askToDelete(QString thing, QVector<int> items)
{
  bool res = false;

  if (items.count() == 0) {
    QString titleString = tr("No %1s selected").arg(thing);
    QString msgString   = tr("You must select at least one %1 to be deleted").arg(thing);

    QMessageBox::warning(this, titleString, msgString);
  } else if (items.count() == 1){
    QString titleString = tr("Delete %1").arg(thing);
    QString question    = tr("Really delete %1 %2 ?").arg(thing).arg(items.first());

    int reply = QMessageBox::question(this, titleString, question,
                                      QMessageBox::Ok | QMessageBox::Cancel,
                                      QMessageBox::Cancel);

    if (reply == QMessageBox::Ok) {
      res = true;
    }
  } else {
    QString titleString = tr("Delete %1 %2s").arg(items.count()).arg(thing);
    QString question    = tr("Really delete %1s ").arg(thing);

    for (int i=0; i<items.count(); i++) {
      if (i == 0) {
        question.append(tr("%1").arg(items.value(i)));
      } else {
        question.append(tr(", %1").arg(items.value(i)));
      }
    }

    question.append("?");

    int reply = QMessageBox::question(this, titleString, question,
                                      QMessageBox::Ok | QMessageBox::Cancel,
                                      QMessageBox::Cancel);

    if (reply == QMessageBox::Ok) {
      res = true;
    }
  }

  return res;
}
