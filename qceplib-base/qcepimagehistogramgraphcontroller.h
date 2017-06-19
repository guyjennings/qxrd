#ifndef QCEPIMAGEHISTOGRAMGRAPHCONTROLLER_H
#define QCEPIMAGEHISTOGRAMGRAPHCONTROLLER_H

#include "qcepdataobjectgraphcontroller.h"

class QcepImageHistogramGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepImageHistogramGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepImageHistogramGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QCEPIMAGEHISTOGRAMGRAPHCONTROLLER_H
