#ifndef QXRDDATAOBJECTGRAPHCONTROLLER_H
#define QXRDDATAOBJECTGRAPHCONTROLLER_H

#include <QObject>
#include "qcepdataobject-ptr.h"

class QxrdDataObjectGraphWindow;

class QxrdDataObjectGraphController : public QObject
{
  Q_OBJECT
public:
  explicit QxrdDataObjectGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

  virtual ~QxrdDataObjectGraphController();
  virtual void activate();

signals:

public slots:
  virtual void updateDisplay();

protected:
  QxrdDataObjectGraphWindow *m_Window;
  int                        m_Mode;
  QcepDataObjectPtr          m_Object;
  QWidget                   *m_Widget;
};

#endif // QXRDDATAOBJECTGRAPHCONTROLLER_H
