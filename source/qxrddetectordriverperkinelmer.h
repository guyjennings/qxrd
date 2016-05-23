#ifndef QXRDDETECTORDRIVERPERKINELMER_H
#define QXRDDETECTORDRIVERPERKINELMER_H

#include "qxrddetectordriver.h"

class QxrdDetectorDriverPerkinElmer : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPerkinElmer(QString name);
};

#endif // QXRDDETECTORDRIVERPERKINELMER_H
