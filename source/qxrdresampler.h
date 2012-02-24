#ifndef QXRDRESAMPLER_H
#define QXRDRESAMPLER_H

#include "qcepmacros.h"
#include <QObject>

class QxrdResampler : public QObject
{
  Q_OBJECT
public:
  explicit QxrdResampler(QObject *parent = 0);

signals:

public slots:
};

#endif // QXRDRESAMPLER_H

class QxrdResampler;
