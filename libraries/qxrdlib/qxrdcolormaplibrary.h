#ifndef QXRDCOLORMAPLIBRARY_H
#define QXRDCOLORMAPLIBRARY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdcolormap-ptr.h"
#include "qxrdmaskcolormap-ptr.h"

class QXRD_EXPORT QxrdColorMapLibrary : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdColorMapLibrary(QString name);

  static int colorMapCount();
  static QString colorMapName(int n);
  static QxrdColorMap* newImageColorMap(int n);
  static QxrdMaskColorMap* newOverflowColorMap(int n);
  static QxrdMaskColorMap* newMaskColorMap(int n);
};

#endif // QXRDCOLORMAPLIBRARY_H
