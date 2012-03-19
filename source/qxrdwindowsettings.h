#ifndef QXRDWINDOWSETTINGS_H
#define QXRDWINDOWSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdimageplotsettings.h"
#include "qxrdcenterfinderplotsettings.h"
#include "qxrdfilebrowsersettings.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdinfodialogsettings.h"
#include "qxrdslicedialogsettings.h"

class QxrdWindowSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdWindowSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
public:
  Q_PROPERTY(QByteArray windowGeometry READ get_WindowGeometry WRITE set_WindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(WindowGeometry)

  Q_PROPERTY(QByteArray windowState READ get_WindowState WRITE set_WindowState)
  QCEP_BYTE_ARRAY_PROPERTY(WindowState)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
private:
  QxrdSettingsSaverWPtr                  m_Saver;
  QxrdImagePlotSettings                  m_PlotSettings;
  QxrdCenterFinderPlotSettings           m_CenterFinderPlotSettings;
  QxrdPlotSettings                       m_IntegratorPlotSettings;
  QxrdFileBrowserSettings                m_InputFileBrowserSettings;
  QxrdFileBrowserSettings                m_OutputFileBrowserSettings;
  QxrdHistogramDialogSettings            m_HistogramDialogSettings;
  QxrdSliceDialogSettings                m_SliceDialogSettings;
  QxrdInfoDialogSettings                 m_InfoDialogSettings;
};

#endif // QXRDWINDOWSETTINGS_H
