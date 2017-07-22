#include "qxrdtestscangeneratorwindow.h"
#include "ui_qxrdtestscangeneratorwindow.h"
#include "qxrdtestscangeneratorthread.h"
#include "qxrdexperiment.h"
#include "qxrdtestscangenerator.h"
#include "qxrdtestscanwindowqwt.h"
#include "qxrdtestscanwindowdatavis.h"

QxrdTestScanGeneratorWindow::QxrdTestScanGeneratorWindow(QxrdExperimentWPtr doc, QWidget *parent) :
  QMainWindow(parent)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  m_Thread = QxrdTestScanGeneratorThreadPtr(
        new QxrdTestScanGeneratorThread(doc));

  m_Thread -> start();

  m_Generator = m_Thread -> generator();

  QxrdExperimentPtr        expt(doc);
  QxrdTestScanGeneratorPtr gen(m_Generator);

  if (expt) {
    if (gen) {
      expt->addChildPtr(gen);
    }

    expt->printMessage("Started Test Scan Generator Thread");
  }

  if (gen) {
    connect(m_GenerateStopButton,  &QPushButton::clicked, m_ActionStopGeneration,   &QAction::trigger);
    connect(m_GenerateOnceButton,  &QPushButton::clicked, m_ActionGenerateOnce,     &QAction::trigger);
    connect(m_Generate10HzButton,  &QPushButton::clicked, m_ActionGenerate10Hz,     &QAction::trigger);
    connect(m_Generate1SecButton,  &QPushButton::clicked, m_ActionGenerate1Sec,     &QAction::trigger);
    connect(m_Generate10SecButton, &QPushButton::clicked, m_ActionGenerate10Sec,    &QAction::trigger);
    connect(m_NewQWTButton,        &QPushButton::clicked, m_ActionNewQWTWindow,     &QAction::trigger);
    connect(m_NewDataVisButton,    &QPushButton::clicked, m_ActionNewDataVisWindow, &QAction::trigger);

    connect(m_ActionStopGeneration, &QAction::triggered, gen.data(), &QxrdTestScanGenerator::repRateZero);
    connect(m_ActionGenerateOnce,   &QAction::triggered, gen.data(), &QxrdTestScanGenerator::generateOne);
    connect(m_ActionGenerate10Hz,   &QAction::triggered, gen.data(), &QxrdTestScanGenerator::repRate10Hz);
    connect(m_ActionGenerate1Sec,   &QAction::triggered, gen.data(), &QxrdTestScanGenerator::repRate1Sec);
    connect(m_ActionGenerate10Sec,  &QAction::triggered, gen.data(), &QxrdTestScanGenerator::repRate10Sec);

    connect(m_ActionNewQWTWindow,     &QAction::triggered, this, &QxrdTestScanGeneratorWindow::doNewQWTWindow);
    connect(m_ActionNewDataVisWindow, &QAction::triggered, this, &QxrdTestScanGeneratorWindow::doNewDataVisWindow);
  }
}

QxrdTestScanGeneratorWindow::~QxrdTestScanGeneratorWindow()
{
}

void QxrdTestScanGeneratorWindow::doNewQWTWindow()
{
  auto win = new QxrdTestScanWindowQwt();

  QxrdTestScanGeneratorPtr gen(m_Generator);

  if (gen) {
    connect(gen.data(), &QxrdTestScanGenerator::newScanAvailable,
            win,        &QxrdTestScanWindowQwt::onNewScanAvailable);
  }

  win->show();
}

void QxrdTestScanGeneratorWindow::doNewDataVisWindow()
{
  auto win = new QxrdTestScanWindowDataVis();

  QxrdTestScanGeneratorPtr gen(m_Generator);

  if (gen) {
    connect(gen.data(), &QxrdTestScanGenerator::newScanAvailable,
            win,        &QxrdTestScanWindowDataVis::onNewScanAvailable);
  }

  win->show();
}
