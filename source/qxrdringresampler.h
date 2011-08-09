#ifndef QXRDRINGRESAMPLER_H
#define QXRDRINGRESAMPLER_H

#include "qxrdresampler.h"

class QxrdRingResampler : public QxrdResampler
{
  Q_OBJECT
public:
    QxrdRingResampler(QObject *parent=0);
};

#endif // QXRDRINGRESAMPLER_H
