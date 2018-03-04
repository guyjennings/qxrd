#include "qxrdanalysiswindowsettings.h"
#include "qxrdanalysiswindow.h"
#include <QThread>

QxrdAnalysisWindowSettings::QxrdAnalysisWindowSettings(QString name, QString desc)
  : inherited(name, desc)
{

}

void QxrdAnalysisWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdAnalysisWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdAnalysisWindow("Analysis"),
        &QObject::deleteLater);

  return m_Window;
}
