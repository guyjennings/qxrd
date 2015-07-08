#include "qxrdgraphcontrolswidget.h"
#include "qxrddataobjectgraphwindow.h"
#include "qcepdataobject.h"

QxrdGraphControlsWidget::QxrdGraphControlsWidget
  (QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object) :
  QWidget(window),
  m_Window(window),
  m_Mode(mode),
  m_Object(object)
{

}

