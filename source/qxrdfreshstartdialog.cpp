#include "qxrdfreshstartdialog.h"
#include "ui_qxrdfreshstartdialog.h"
#include "qxrdexperimentsettings.h"
#include "qxrdacquisitionthread.h"
#include <stdio.h>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

QxrdFreshStartDialog::QxrdFreshStartDialog(QWidget *parent) :
    QDialog(parent)
{
  setupUi(this);

  int detectorType = 0;
  int debugLevel = 0;
  int specServer = 0;
  int specServerPort = 0;
  int simpleServer = 0;
  int simpleServerPort = 0;
  int defaultLayout = 0;
  double exposureTime = 0.0;

  {
    QxrdExperimentSettings settings;

    detectorType = settings.value("application/detectorType").toInt();
    debugLevel = settings.value("application/debug").toInt();
    specServer = settings.value("application/runSpecServer").toInt();
    specServerPort = settings.value("application/specServerPort").toInt();
    simpleServer = settings.value("application/runSimpleServer").toInt();
    simpleServerPort = settings.value("application/simpleServerPort").toInt();
    defaultLayout = settings.value("application/defaultLayout").toInt();
    exposureTime = settings.value("acquire/exposureTime").toDouble();
  }

  QStringList detectorTypes = QxrdAcquisitionThread::detectorTypeNames();

  m_DetectorTypeCombo -> addItems(detectorTypes);
  m_DetectorTypeCombo -> setCurrentIndex(detectorType);

  m_RunSpecServer -> setChecked(specServer);
  m_RunSimpleServer -> setChecked(simpleServer);
  m_SpecServerPort -> setRange(-1,65535);
  m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
  m_SpecServerPort -> setValue(specServerPort);
  m_SimpleServerPort -> setRange(0,65535);
  m_SimpleServerPort -> setValue(simpleServerPort);
  m_DefaultLayout ->setChecked(defaultLayout);
  m_ExposureTime -> setValue(exposureTime);

  setupDebugWidgets(debugLevel);
}

QxrdFreshStartDialog::~QxrdFreshStartDialog()
{
}

void QxrdFreshStartDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdFreshStartDialog::accept()
{
  QxrdExperimentSettings settings;

  settings.setValue("application/detectorType",    m_DetectorTypeCombo->currentIndex());
  settings.setValue("application/debug",           readDebugWidgets());
  settings.setValue("application/runSpecServer",   m_RunSpecServer->checkState());
  settings.setValue("application/specServerPort",  m_SpecServerPort->value());
  settings.setValue("application/runSimpleServer", m_RunSimpleServer->checkState());
  settings.setValue("application/simpleServerPort",m_SimpleServerPort->value());
  settings.setValue("application/defaultLayout",   m_DefaultLayout->checkState());
  settings.setValue("acquire/exposureTime",        m_ExposureTime->value());

  QDialog::accept();
}

void QxrdFreshStartDialog::setupDebugWidgets(int dbg)
{
  QGridLayout *grid = new QGridLayout(m_DebugWidgets);

  int mask = 1;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = new QCheckBox(gDebugStrings[i]);
    cb->setChecked(dbg & mask);

    mask <<= 1;

    m_DebugWidgetList.append(cb);
  }

  int ndebug = m_DebugWidgetList.count();
  int ncol   = ndebug - ndebug/2;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = m_DebugWidgetList[i];

    if (i < ncol) {
      grid->addWidget(cb, i, 0);
    } else {
      grid->addWidget(cb, i - ncol, 1);
    }
  }
}

int QxrdFreshStartDialog::readDebugWidgets()
{
  int mask = 1;
  int newDbg = 0;

  for (int i=0; gDebugStrings[i]; i++) {
    if (m_DebugWidgetList[i]->isChecked()) {
      newDbg |= mask;
    }

    mask <<= 1;
  }

  return newDbg;
}

