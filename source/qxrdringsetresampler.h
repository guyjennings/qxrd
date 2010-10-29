#ifndef QXRDRINGSETRESAMPLER_H
#define QXRDRINGSETRESAMPLER_H

#include "qxrdresampler.h"
#include <QObject>

class QxrdRingSetResampler : public QxrdResampler
{
  Q_OBJECT;
public:
  explicit QxrdRingSetResampler(QObject *parent = 0);

signals:

public slots:
};

#endif // QXRDRINGSETRESAMPLER_H
