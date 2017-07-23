#include "qxrdtestimagegeneratorwindow.h"
#include "ui_qxrdtestimagegeneratorwindow.h"
#include "qxrdtestimagegeneratorthread.h"
#include "qxrdexperiment.h"
#include "qxrdtestimagegenerator.h"
#include "qxrdtestimagewindowqwt.h"
#include "qxrdtestimagewindowdatavis.h"

QxrdTestImageGeneratorWindow::QxrdTestImageGeneratorWindow(QxrdExperimentWPtr doc, QWidget *parent) :
  QMainWindow(parent)
{
  setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

  m_Thread = QxrdTestImageGeneratorThreadPtr(
        new QxrdTestImageGeneratorThread(doc));

  m_Thread -> start();

  m_Generator = m_Thread -> generator();

  QxrdExperimentPtr         expt(doc);
  QxrdTestImageGeneratorPtr gen(m_Generator);

  if (expt) {
    if (gen) {
      expt->addChildPtr(gen);
    }

    expt->printMessage("Started Test Image Generator Thread");
  }

  if (gen) {
    connect(m_GenerateStopButton,   &QPushButton::clicked, m_ActionStopGeneration,   &QAction::trigger);
    connect(m_GenerateOnceButton,   &QPushButton::clicked, m_ActionGenerateOnce,     &QAction::trigger);
    connect(m_Generate10HzButton,   &QPushButton::clicked, m_ActionGenerate10Hz,     &QAction::trigger);
    connect(m_Generate1SecButton,   &QPushButton::clicked, m_ActionGenerate1Sec,     &QAction::trigger);
    connect(m_Generate10SecButton,  &QPushButton::clicked, m_ActionGenerate10Sec,    &QAction::trigger);
    connect(m_NewQWTButton,         &QPushButton::clicked, m_ActionNewQWTWindow,     &QAction::trigger);
    connect(m_NewDataVisButton,     &QPushButton::clicked, m_ActionNewDataVisWindow, &QAction::trigger);

    connect(m_ActionStopGeneration, &QAction::triggered, gen.data(), &QxrdTestImageGenerator::repRateZero);
    connect(m_ActionGenerateOnce,   &QAction::triggered, gen.data(), &QxrdTestImageGenerator::generateOne);
    connect(m_ActionGenerate10Hz,   &QAction::triggered, gen.data(), &QxrdTestImageGenerator::repRate10Hz);
    connect(m_ActionGenerate1Sec,   &QAction::triggered, gen.data(), &QxrdTestImageGenerator::repRate1Sec);
    connect(m_ActionGenerate10Sec,  &QAction::triggered, gen.data(), &QxrdTestImageGenerator::repRate10Sec);

    connect(m_ActionNewQWTWindow,     &QAction::triggered, this, &QxrdTestImageGeneratorWindow::doNewQWTWindow);
    connect(m_ActionNewDataVisWindow, &QAction::triggered, this, &QxrdTestImageGeneratorWindow::doNewDataVisWindow);
  }
}

QxrdTestImageGeneratorWindow::~QxrdTestImageGeneratorWindow()
{
}

static int s_ImageWindowCount = 0;

void QxrdTestImageGeneratorWindow::doNewQWTWindow()
{
  auto win = new QxrdTestImageWindowQwt();

  QxrdTestImageGeneratorPtr gen(m_Generator);

  if (gen && win) {
    connect(gen.data(), &QxrdTestImageGenerator::newImageAvailable,
            win,        &QxrdTestImageWindowQwt::onNewImageAvailable);
  }

  win->setWindowTitle(tr("Test Image Generator QWT Window %1").arg(s_ImageWindowCount++));

  win->show();
}

void QxrdTestImageGeneratorWindow::doNewDataVisWindow()
{
  auto win = new QxrdTestImageWindowDataVis();

  QxrdTestImageGeneratorPtr gen(m_Generator);

  if (gen && win) {
    connect(gen.data(), &QxrdTestImageGenerator::newImageAvailable,
            win,        &QxrdTestImageWindowDataVis::onNewImageAvailable);
  }

  win->setWindowTitle(tr("Test Image Generator Data Vis Window %1").arg(s_ImageWindowCount++));

  win->show();
}
