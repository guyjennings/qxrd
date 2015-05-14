#ifndef QXRDCALIBRANTLIBRARYPTR_H
#define QXRDCALIBRANTLIBRARYPTR_H

#include <QSharedPointer>

class QxrdCalibrantLibrary;

typedef QSharedPointer<QxrdCalibrantLibrary> QxrdCalibrantLibraryPtr;
typedef QWeakPointer<QxrdCalibrantLibrary>   QxrdCalibrantLibraryWPtr;

#endif // QXRDCALIBRANTLIBRARYPTR_H

