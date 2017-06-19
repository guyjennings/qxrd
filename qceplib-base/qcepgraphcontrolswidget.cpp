#include "qcepgraphcontrolswidget.h"
#include "qcepdataobjectgraphwindow.h"
#include "qcepdataobject.h"

QcepGraphControlsWidget::QcepGraphControlsWidget
  (QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QWidget(window),
  m_Window(window),
  m_Mode(mode),
  m_Object(object)
{
}

