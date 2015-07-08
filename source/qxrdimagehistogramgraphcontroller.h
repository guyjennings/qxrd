#ifndef QXRDIMAGEHISTOGRAMGRAPHCONTROLLER_H
#define QXRDIMAGEHISTOGRAMGRAPHCONTROLLER_H

#include "qxrddataobjectgraphcontroller.h"

class QxrdImageHistogramGraphController : public QxrdDataObjectGraphController
{
  Q_OBJECT

public:
  QxrdImageHistogramGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

  virtual ~QxrdImageHistogramGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QXRDIMAGEHISTOGRAMGRAPHCONTROLLER_H
