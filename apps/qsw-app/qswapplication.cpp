#include "qswapplication.h"
#include "qswmainwindow.h"
#include "qswexperimentthread.h"
#include "qswexperiment.h"
#include "qcepplotwidgetsettings.h"
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
  return QStringLiteral("QSW");
}

QString QswApplication::applicationMnemonic()
{
  return QStringLiteral("qsw");
}

QString QswApplication::applicationVersion()
{
  return QStringLiteral(STR(QSW_VERSION));
}

QString QswApplication::applicationDescription()
{
  return QStringLiteral("QSW SpectraWiz Spectrometer Readout");
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
