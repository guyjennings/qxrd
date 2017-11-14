#ifndef QXRDPLOTCOMMAND_H
#define QXRDPLOTCOMMAND_H

#include "qcepserializableobject.h"

class QxrdPlotCommand : public QcepSerializableObject
{
public:
  QxrdPlotCommand(QString name);

  virtual QToolButton *toolButton() = 0;
};

#endif // QXRDPLOTCOMMAND_H
