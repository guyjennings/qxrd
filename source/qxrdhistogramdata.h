#ifndef QXRDHISTOGRAMDATA_H
#define QXRDHISTOGRAMDATA_H

#include "qcepmacros.h"
#include <QObject>
#include <QSharedPointer>

class QxrdHistogramData : public QObject
{
  Q_OBJECT
public:
  explicit QxrdHistogramData(QObject *parent = 0);

signals:

public slots:
};

#endif // QXRDHISTOGRAMDATA_H

class QxrdHistogramData;
typedef QSharedPointer<QxrdHistogramData> QxrdHistogramDataPtr;
