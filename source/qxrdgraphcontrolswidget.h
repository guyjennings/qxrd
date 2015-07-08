#ifndef QXRDGRAPHCONTROLSWIDGET_H
#define QXRDGRAPHCONTROLSWIDGET_H

#include <QWidget>
#include "qcepdataobject-ptr.h"

class QxrdDataObjectGraphWindow;

class QxrdGraphControlsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit QxrdGraphControlsWidget(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

signals:

public slots:

protected:
  QxrdDataObjectGraphWindow  *m_Window;
  int                         m_Mode;
  QcepDataObjectPtr           m_Object;
};

#endif // QXRDGRAPHCONTROLSWIDGET_H
