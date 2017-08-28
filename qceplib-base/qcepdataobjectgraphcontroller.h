#ifndef QCEPDATAOBJECTGRAPHCONTROLLER_H
#define QCEPDATAOBJECTGRAPHCONTROLLER_H

#include "qceplib_global.h"
#include <QObject>
#include "qcepdataobject-ptr.h"

class QcepDataObjectGraphWindow;

class QCEP_EXPORT QcepDataObjectGraphController : public QObject
{
  Q_OBJECT
public:
  explicit QcepDataObjectGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepDataObjectGraphController();
  virtual void activate();

signals:

public slots:
  virtual void updateDisplay();

protected:
  QcepDataObjectGraphWindow *m_Window;
  int                        m_Mode;
  QcepDataObjectWPtr         m_Object;
  QWidget                   *m_Widget;
};

#endif // QCEPDATAOBJECTGRAPHCONTROLLER_H
