#ifndef QXRDFILESAVER_PTR_H
#define QXRDFILESAVER_PTR_H

#include <QSharedPointer>

class QxrdFileSaver;

typedef QSharedPointer<QxrdFileSaver> QxrdFileSaverPtr;
typedef QWeakPointer<QxrdFileSaver> QxrdFileSaverWPtr;

#endif
