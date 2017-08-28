#ifndef QCEPIMAGEHISTOGRAMGRAPHCONTROLLER_H
#define QCEPIMAGEHISTOGRAMGRAPHCONTROLLER_H

#include "qceplib_global.h"
#include "qcepdataobjectgraphcontroller.h"

class QCEP_EXPORT QcepImageHistogramGraphController : public QcepDataObjectGraphController
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
