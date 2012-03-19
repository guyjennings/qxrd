#include "qxrdwindowsettings.h"

QxrdWindowSettings::QxrdWindowSettings(QxrdSettingsSaverWPtr saver,
                                       QObject *parent) :
  QObject(parent),
  m_WindowGeometry(saver, this, "windowGeometry", QByteArray(), "Window Geometry Settings"),
  m_WindowState(saver, this, "windowState", QByteArray(), "Window State Settings"),
  m_Saver(saver),
  m_PlotSettings(saver, NULL),
  m_CenterFinderPlotSettings(saver, NULL),
  m_IntegratorPlotSettings(saver, NULL),
  m_InputFileBrowserSettings(saver, NULL),
  m_OutputFileBrowserSettings(saver, NULL),
  m_HistogramDialogSettings(saver, NULL),
  m_SliceDialogSettings(saver, NULL),
  m_InfoDialogSettings(saver, NULL)
{
}

void QxrdWindowSettings::readSettings(QSettings *settings, QString section)
{

  if (settings) {
    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    m_PlotSettings.readSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings.readSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings.readSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings.readSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings.readSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings.readSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings.readSettings(settings, section+"/sliceDialog");
    m_InfoDialogSettings.readSettings(settings, section+"/imageInfoDialog");

    m_SettingsLoaded = true;

    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      if (!expt->get_DefaultLayout()) {
        QByteArray geometry = get_WindowGeometry();
        QByteArray winstate = get_WindowState();

        if (!restoreGeometry(geometry)) {
          printf("Restore geometry failed\n");
        }

        if (!restoreState(winstate,2)) {
          printf("Restore state failed\n");
        }
      } else{
        expt->set_DefaultLayout(0);
      }
    }
  }
}

void QxrdWindowSettings::writeSettings(QSettings *settings, QString section)
{
  //    printf("QxrdWindow::writeSettings\n");

  if (settings) {
    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    m_PlotSettings.writeSettings(settings, section+"/plot");
    m_CenterFinderPlotSettings.writeSettings(settings, section+"/centerFinderPlot");
    m_IntegratorPlotSettings.writeSettings(settings, section+"/integratorPlot");
    m_InputFileBrowserSettings.writeSettings(settings, section+"/inputFileBrowser");
    m_OutputFileBrowserSettings.writeSettings(settings, section+"/outputFileBrowser");
    m_HistogramDialogSettings.writeSettings(settings, section+"/histogramDialog");
    m_SliceDialogSettings.writeSettings(settings, section+"/sliceDialog");
    m_ImageInfoDialogSettings.writeSettings(settings, section+"/imageInfoDialog");
  }
}

