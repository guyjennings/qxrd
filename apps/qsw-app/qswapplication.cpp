#include "qswapplication.h"
#include "qswmainwindow.h"
#include "qswexperimentthread.h"
#include "qswexperiment.h"
#include "qcepplotwidgetsettings.h"
#include "qcepallocator.h"
#include "qswglobalsettings.h"

QswApplication::QswApplication(int &argc, char **argv)
  : inherited(argc, argv),
    m_MainWindow(),
    m_ExperimentThread(),
    m_Experiment(),
    m_PlotSettings()
{
  QcepProperty::registerMetaTypes();

  qRegisterMetaType< QVector<float> >("Qvector<float>");
}

void QswApplication::initializeRoot()
{
  inherited::initializeRoot();

  m_Allocator =
      QcepAllocatorPtr(
        new QcepAllocator("allocator"));
  m_Allocator -> initialize(sharedFromThis());

  m_PlotSettings =
      QcepPlotWidgetSettingsPtr(
        new QcepPlotWidgetSettings("plotSettings"));
  m_PlotSettings -> initialize(sharedFromThis());

  m_MainWindow =
      QswMainWindowPtr(
        new QswMainWindow("plotWindow"));

  m_MainWindow -> initialize(m_PlotSettings);

  m_MainWindow -> show();
  m_MainWindow -> raise();

  m_ExperimentThread =
      QswExperimentThreadPtr(
        NEWPTR(QswExperimentThread("experimentThread")));

  m_ExperimentThread -> initialize(sharedFromThis());

  m_ExperimentThread -> start();

  m_Experiment = m_ExperimentThread -> experiment();

  connect(m_Experiment.data(), &QswExperiment::dataAvailable,
          m_MainWindow.data(),  &QswMainWindow::onDataAvailable);
}

QString QswApplication::applicationName()
{
  return "QSW";
}

QString QswApplication::applicationMnemonic()
{
  return "qsw";
}

QString QswApplication::applicationVersion()
{
  return "0.1";
}

QString QswApplication::applicationDescription()
{
  return "QSW SpectraWiz Spectrometer Readout";
}

QIcon QswApplication::applicationIcon()
{
  QIcon res(":/qsw-images/gspectrum-48x48.png");

  return res;
}

QSettingsPtr QswApplication::applicationSettings()
{
  QSettingsPtr res(
        QSettingsPtr(
          new QswGlobalSettings()));

  return res;
}

void QswApplication::openExperiment(QString path)
{
}

void QswApplication::openRecentExperiment(QString path)
{
}

void QswApplication::editGlobalPreferences()
{
}

void QswApplication::createNewExperiment()
{
}

void QswApplication::chooseExistingExperiment()
{
}

QcepExperimentPtr QswApplication::experiment(int i) const
{
  return QcepExperimentPtr();
}
