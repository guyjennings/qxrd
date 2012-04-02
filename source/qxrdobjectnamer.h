#ifndef QXRDOBJECTNAMER_H
#define QXRDOBJECTNAMER_H

#include <QObject>
#include <QString>

/**
  QxrdObjectNamer is a helper class used to call setObjectName during
  construction of a QObject derived object
*/

class QxrdObjectNamer {
public:
  QxrdObjectNamer(QObject *object, QString name);
};

#endif // QXRDOBJECTNAMER_H
