#include "qxrdextraiowindow.h"
#include "ui_qxrdextraiowindow.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrdsynchronizedacquisition.h"
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

//      m_SyncAcqMode     -> addItem("None");
//      m_SyncAcqMode     -> addItem("Stepped Output");
//      m_SyncAcqMode     -> addItem("Continuous Output");

//      m_SyncAcqWfm      -> addItem("Square");
//      m_SyncAcqWfm      -> addItem("Sine");
//      m_SyncAcqWfm      -> addItem("Triangle");
//      m_SyncAcqWfm      -> addItem("Sawtooth");
//      m_SyncAcqWfm      -> addItem("Bipolar Triangle");

//      m_SyncAcqMinimum  -> setMinimum(-10.0);
//      m_SyncAcqMinimum  -> setMaximum(10.0);
//      m_SyncAcqMinimum  -> setSingleStep(0.1);

//      m_SyncAcqMaximum  -> setMinimum(-10.0);
//      m_SyncAcqMaximum  -> setMaximum(10.0);
//      m_SyncAcqMaximum  -> setSingleStep(0.1);

//      m_SyncAcqSymmetry -> setMinimum(-1.0);
//      m_SyncAcqSymmetry -> setMaximum(1.0);
//      m_SyncAcqSymmetry -> setSingleStep(0.1);

//      m_SyncAcqPhaseShift -> setMinimum(-100.0);
//      m_SyncAcqPhaseShift -> setMaximum(100.0);
//      m_SyncAcqPhaseShift -> setSingleStep(1);

//      QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

//      if (sync) {
//        QxrdNIDAQPtr nidaq(sync->nidaqPlugin());

//        if (nidaq) {
//          QStringList devices = nidaq->deviceNames();

//          foreach(QString device, devices) {
//            QString desc = nidaq->deviceType(device);
//            int     isSim = nidaq->deviceIsSimulated(device);

//            QString item = device+" : "+desc;

//            if (isSim) {
//              item += " [simulated]";
//            }

////            m_SyncAcqOutDevice->addItem(item, device);
//          }
//        }

//        sync -> prop_SyncAcquisitionMode()          -> linkTo(m_SyncAcqMode);
//        sync -> prop_SyncAcquisitionWaveform()      -> linkTo(m_SyncAcqWfm);
//        sync -> prop_SyncAcquisitionOutputDevice()  -> linkTo(m_SyncAcqOutDevice);
//        sync -> prop_SyncAcquisitionOutputChannel() -> linkTo(m_SyncAcqOutChan);

//        sync -> prop_SyncAcquisitionMinimum()       -> linkTo(m_SyncAcqMinimum);
//        sync -> prop_SyncAcquisitionMaximum()       -> linkTo(m_SyncAcqMaximum);
//        sync -> prop_SyncAcquisitionSymmetry()      -> linkTo(m_SyncAcqSymmetry);
//        sync -> prop_SyncAcquisitionPhaseShift()    -> linkTo(m_SyncAcqPhaseShift);
//        sync -> prop_SyncAcquisitionManualValue()   -> linkTo(m_ManualOutputVolts);

//        connect(sync -> prop_SyncAcquisitionOutputDevice(), &QcepStringProperty::valueChanged, this, &QxrdExtraIOWindow::deviceChanged);
//        connect(sync -> prop_SyncAcquisitionOutputChannel(), &QcepStringProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionMode(), &QcepIntProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionWaveform(), &QcepIntProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionMinimum(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionMaximum(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionSymmetry(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
//        connect(sync -> prop_SyncAcquisitionPhaseShift(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);

//        connect(m_ManualOutput, &QAbstractButton::clicked, sync.data(), &QxrdSynchronizedAcquisition::setManualOutput);
//        connect(m_ManualTrigger, &QAbstractButton::clicked, sync.data(), &QxrdSynchronizedAcquisition::triggerOnce);
//      }

      connect(acq->prop_ExposureTime(), &QcepDoubleProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);
      connect(acq->prop_PhasesInGroup(), &QcepIntProperty::valueChanged, this, &QxrdExtraIOWindow::waveformChanged);

      deviceChanged();
      waveformChanged();
    }

    connect(m_TestReadout, &QAbstractButton::clicked, this, &QxrdExtraIOWindow::initiateReadout);

    if (exp && acq) {
      //TODO: re-init
//      m_AcquisitionWaveforms -> init(QxrdAcquisitionExtraInputsPlotSettingsWPtr());

//      m_AcquisitionExtraInputs = acq->acquisitionExtraInputs();

//      if (m_AcquisitionExtraInputs) {

//        QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//        if (xtra) {
//          QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

//          if (nidaq) {
//            QStringList devices = nidaq->deviceNames();

//            foreach(QString device, devices) {
//              QString desc = nidaq->deviceType(device);
//              int     isSim = nidaq->deviceIsSimulated(device);

//              QString item = device+" : "+desc;

//              if (isSim) {
//                item += " [simulated]";
//              }

////              m_AcquisitionDevice->addItem(item, device);
//            }
//          }

////          xtra->prop_SampleRate()->linkTo(m_SampleRate);
////          xtra->prop_AcquireDelay()->linkTo(m_AcquisitionDelay);
////          xtra->prop_DeviceName()->linkTo(m_AcquisitionDevice);

////          connect(xtra->prop_DeviceName(), &QcepStringProperty::valueChanged,
////                  this, &QxrdExtraIOWindow::updateUi);

//          connect(xtra.data(), &QxrdAcquisitionExtraInputs::newDataAvailable,
//                  this, &QxrdExtraIOWindow::updateWaveforms);

////          connect(xtra.data(), &QxrdAcquisitionExtraInputs::channelCountChanged,
////                  this, &QxrdExtraIOWindow::updateUi);

////          updateUi();
//        }
//      }
    }
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
    m_AcquisitionWaveforms -> initialize(settings->extraInputsPlotWidgetSettings());
    m_WaveformPlot         -> initialize(settings->extraOutputsPlotWidgetSettings());
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

void QxrdExtraIOWindow::deviceChanged()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    QxrdNIDAQPtr nidaq(sync->nidaqPlugin());

//    if (nidaq) {
//      QStringList aoChannels = nidaq->deviceAOChannels(sync->get_SyncAcquisitionOutputDevice());

//      bool blocked = m_SyncAcqOutChan->blockSignals(true);
//      m_SyncAcqOutChan->clear();

//      foreach(QString chan, aoChannels) {
//        m_SyncAcqOutChan->addItem(chan, chan);
//      }

//      m_SyncAcqOutChan->blockSignals(blocked);
//    }
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

      //TODO: rewrite
//      m_WaveformPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
//      m_WaveformPlot->detachItems(QwtPlotItem::Rtti_PlotMarker);

//      if (sync -> get_SyncAcquisitionMode() && (parms->nphases() >= 1)) {
//        QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_WaveformPlot, "Output Waveform");

//        pc->setSamples(sync->outputTimes(), sync->outputVoltage());

//        pc->attach(m_WaveformPlot);
//      }

//      m_WaveformPlot->updateZoomer();
//      m_WaveformPlot->replot();

      sync->finishedAcquisition();
    }
  }
}

//void QxrdExtraIOWindow::setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch)
//{
//  if (ch) {
//    QCheckBox *cb = new QCheckBox();
//    ch->prop_Enabled()->linkTo(cb);

//    QCheckBox *cb2 = new QCheckBox();
//    ch->prop_Plotted()->linkTo(cb2);

//    QComboBox *nm = new QComboBox();

//    QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//    if (xtra) {
//      QString deviceName = xtra->get_DeviceName();
//      QxrdNIDAQPtr nidaq(xtra->nidaqPlugin());

//      if (nidaq) {
//        QStringList aiChannels = nidaq->deviceAIChannels(deviceName);
//        QStringList ctrChannels = nidaq->deviceCIChannels(deviceName);

//        nm->addItem("Analog Inputs");
//        foreach(QString chan, aiChannels) {
//          nm->addItem(chan, chan);
//        }

//        nm->addItem("Counter Inputs");
//        foreach(QString chan, ctrChannels) {
//          nm->addItem(chan, chan);
//        }
//      }
//    }

//    ch->prop_ChannelName()->linkTo(nm);

//    QComboBox *md = new QComboBox();

//    md->addItem("Sum");
//    md->addItem("Avg");
//    md->addItem("Max");
//    md->addItem("Min");

//    md->setItemData(0, "Value = Sum(wfm)", Qt::ToolTipRole);
//    md->setItemData(1, "Value = Avg(wfm)", Qt::ToolTipRole);
//    md->setItemData(2, "Value = Max(wfm)", Qt::ToolTipRole);
//    md->setItemData(3, "Value = Min(wfm)", Qt::ToolTipRole);

//    ch->prop_Mode()->linkTo(md);

//    QCheckBox *wf = new QCheckBox();
//    ch->prop_SaveWave()->linkTo(wf);

//    QDoubleSpinBox *min = new QDoubleSpinBox();
//    min->setMinimum(-20.0);
//    min->setMaximum(20.0);
//    min->setSingleStep(0.1);
//    ch->prop_Min()->linkTo(min);

//    QDoubleSpinBox *max = new QDoubleSpinBox();
//    max->setMinimum(-20.0);
//    max->setMaximum(20.0);
//    max->setSingleStep(0.1);
//    ch->prop_Max()->linkTo(max);

//    QDoubleSpinBox *stt = new QDoubleSpinBox();
//    stt->setMinimum(-0.2);
//    stt->setMaximum(10.0);
//    stt->setSingleStep(0.1);
//    ch->prop_Start()->linkTo(stt);

//    QDoubleSpinBox *end = new QDoubleSpinBox();
//    end->setMinimum(-0.2);
//    end->setMaximum(10.0);
//    end->setSingleStep(0.1);
//    ch->prop_End()->linkTo(end);

//    QComboBox  *tmode = new QComboBox();
//    tmode -> addItem("No Trigger");
//    tmode -> addItem("Positive Edge");
//    tmode -> addItem("Negative Edge");
//    tmode -> addItem("Positive Level");
//    tmode -> addItem("Negative Level");
//    ch->prop_TriggerMode()->linkTo(tmode);

//    QDoubleSpinBox *tlevel = new QDoubleSpinBox();
//    tlevel -> setMinimum(-20.0);
//    tlevel -> setMaximum(+20.0);
//    tlevel -> setSingleStep(0.1);
//    ch->prop_TriggerLevel()->linkTo(tlevel);

//    QDoubleSpinBox *thyst = new QDoubleSpinBox();
//    thyst -> setMinimum(0.0);
//    thyst -> setMaximum(+20.0);
//    thyst -> setSingleStep(0.1);
//    ch->prop_TriggerHysteresis()->linkTo(thyst);

//    QLabel *phy = new QLabel();
//    ch->prop_PhysicalChannel()->linkTo(phy);

//    QLabel *val = new QLabel();
//    ch->prop_Value()->linkTo(val);

//    QCheckBox *trig = new QCheckBox();
//    ch->prop_Triggered()->linkTo(trig);

//    int c = 0;

//    if (m_ChannelsInRows) {
//      m_ExtraInputsTable->setCellWidget(i, c++, cb);
//      m_ExtraInputsTable->setCellWidget(i, c++, cb2);
//      m_ExtraInputsTable->setCellWidget(i, c++, nm);
//      m_ExtraInputsTable->setCellWidget(i, c++, md);
//      m_ExtraInputsTable->setCellWidget(i, c++, wf);
//      m_ExtraInputsTable->setCellWidget(i, c++, min);
//      m_ExtraInputsTable->setCellWidget(i, c++, max);
//      m_ExtraInputsTable->setCellWidget(i, c++, stt);
//      m_ExtraInputsTable->setCellWidget(i, c++, end);
//      m_ExtraInputsTable->setCellWidget(i, c++, tmode);
//      m_ExtraInputsTable->setCellWidget(i, c++, tlevel);
//      m_ExtraInputsTable->setCellWidget(i, c++, thyst);
//      m_ExtraInputsTable->setCellWidget(i, c++, phy);
//      m_ExtraInputsTable->setCellWidget(i, c++, val);
//      m_ExtraInputsTable->setCellWidget(i, c++, trig);
//    } else {
//      m_ExtraInputsTable->setCellWidget(c++, i, cb);
//      m_ExtraInputsTable->setCellWidget(c++, i, cb2);
//      m_ExtraInputsTable->setCellWidget(c++, i, nm);
//      m_ExtraInputsTable->setCellWidget(c++, i, md);
//      m_ExtraInputsTable->setCellWidget(c++, i, wf);
//      m_ExtraInputsTable->setCellWidget(c++, i, min);
//      m_ExtraInputsTable->setCellWidget(c++, i, max);
//      m_ExtraInputsTable->setCellWidget(c++, i, stt);
//      m_ExtraInputsTable->setCellWidget(c++, i, end);
//      m_ExtraInputsTable->setCellWidget(c++, i, tmode);
//      m_ExtraInputsTable->setCellWidget(c++, i, tlevel);
//      m_ExtraInputsTable->setCellWidget(c++, i, thyst);
//      m_ExtraInputsTable->setCellWidget(c++, i, phy);
//      m_ExtraInputsTable->setCellWidget(c++, i, val);
//      m_ExtraInputsTable->setCellWidget(c++, i, trig);
//    }
//  }
//}

//void QxrdExtraIOWindow::updateUi()
//{
//  GUI_THREAD_CHECK;

//  QxrdAcqCommonPtr acqp(QxrdAcqCommon::findAcquisition(m_Parent));

//  if (acqp) {
//    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

//    if (m_AcquisitionExtraInputs) {

//      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//      if (xtra) {
//        int nInputs = xtra->channels().count();

//        m_ExtraInputsTable->clear();

//        QStringList labels;

//        labels << "Enabled" << "Plotted" << "Device Name" << "Mode" << "SaveWfm?"
//               << "Min V" << "Max V"
//               << "Start" << "End"
//               << "Trigger Mode" << "Trigger Level" << "Trigger Hysteresis"
//               << "Phys Chan" << "Value" << "Triggered?";

//        QStringList chanLabels;

//        for (int i=0; i<nInputs; i++) {
//          chanLabels << tr("Chan %1").arg(i);
//        }

//        if (m_ChannelsInRows) {
//          m_ExtraInputsTable->setRowCount(nInputs);
//          m_ExtraInputsTable->setColumnCount(labels.count());
//        } else {
//          m_ExtraInputsTable->setRowCount(labels.count());
//          m_ExtraInputsTable->setColumnCount(nInputs);
//        }

//        for (int i=0; i<nInputs; i++) {
//          setupUiChannel(i, xtra->channels().value(i));
//        }


//        if (m_ChannelsInRows) {
//          m_ExtraInputsTable->setHorizontalHeaderLabels(labels);
//          m_ExtraInputsTable->setVerticalHeaderLabels(chanLabels);
//        } else {
//          m_ExtraInputsTable->setHorizontalHeaderLabels(chanLabels);
//          m_ExtraInputsTable->setVerticalHeaderLabels(labels);
//        }

//        if (m_ExtraInputsTable->columnCount() && m_ExtraInputsTable->rowCount()) {
//          m_ExtraInputsTable->resizeColumnsToContents();
//          m_ExtraInputsTable->resizeRowsToContents();
//        }
//      }
//    }
//  }
//}

//void QxrdExtraIOWindow::addChannel()
//{
//  QxrdAcqCommonPtr acqp(QxrdAcqCommon::findAcquisition(m_Parent));

//  if (acqp) {
//    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

//    if (m_AcquisitionExtraInputs) {

//      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//      if (xtra) {
//        int nch = xtra->channels().count();

//        int n = (m_ChannelsInRows ?
//                   m_ExtraInputsTable->currentRow() :
//                   m_ExtraInputsTable->currentColumn());

//        int ch = (n<0 ? nch-1 : n);

//        QString titleString = tr("New Channel after Channel %1").arg(ch);
//        QString questString = tr("Do you really want to create a new extra input channel after channel %1").arg(ch);

//        int reply = QMessageBox::question(this, titleString, questString,
//                                          QMessageBox::Ok | QMessageBox::Cancel,
//                                          QMessageBox::Cancel);

//        if (reply== QMessageBox::Ok) {
//          if (ch < 0) {
//            xtra->appendChannel();
//          } else {
//            xtra->appendChannel(ch);
//          }

//          updateUi();
//        }
//      }
//    }
//  }
//}

//void QxrdExtraIOWindow::removeChannel()
//{
//  QxrdAcqCommonPtr acqp(QxrdAcqCommon::findAcquisition(m_Parent));

//  if (acqp) {
//    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

//    if (m_AcquisitionExtraInputs) {

//      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//      if (xtra) {
//        int nch = xtra->channels().count();

//        int n = (m_ChannelsInRows ?
//                   m_ExtraInputsTable->currentRow() :
//                   m_ExtraInputsTable->currentColumn());

//        int ch = (n<0 ? nch-1 : n);

//        QString titleString = tr("Delete Channel %1").arg(ch);
//        QString questString = tr("Do you really want to delete extra input channel %1").arg(ch);

//        int reply = QMessageBox::question(this, titleString, questString,
//                                          QMessageBox::Ok | QMessageBox::Cancel,
//                                          QMessageBox::Cancel);

//        if (reply== QMessageBox::Ok) {
//          if (n < 0) {
//            xtra->removeChannel();
//          } else {
//            xtra->removeChannel(n);
//          }

//          updateUi();
//        }
//      }
//    }
//  }
//}

void QxrdExtraIOWindow::initiateReadout()
{
//  QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//  if (xtra) {
//    xtra->initiate();
//  }
}

void QxrdExtraIOWindow::updateWaveforms()
{
  //TODO: reimplement
//  QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

//  if (xtra) {
//    int nchan = xtra->channels().count();

//    m_AcquisitionWaveforms->setNChannels(nchan);

//    for (int i=0; i<nchan; i++) {
//      QxrdAcquisitionExtraInputsChannelPtr chanp(xtra->channel(i));

//      if (chanp && chanp->get_Enabled() && chanp->get_Plotted()) {
//        m_AcquisitionWaveforms->plotChannel(i,
//                                            chanp->startIndex(),
//                                            chanp->endIndex(),
//                                            xtra->readXChannel(),
//                                            xtra->readChannel(i));
//      } else {
//        m_AcquisitionWaveforms->plotChannel(i, 0, 0, QcepDoubleVector(), QcepDoubleVector());
//      }
//    }

//    m_AcquisitionWaveforms->replot();
//  }
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
