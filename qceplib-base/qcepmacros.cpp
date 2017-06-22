#include "qcepmacros.h"
#include <QString>
#include <QObject>

const char* qceplibVersion()
{
  return STR(QCEPLIB_VERSION);
}

#ifdef HAVE_HDF5

#include "hdf5.h"

QString _hdfVersion;

const char* qcepH5Version()
{
  unsigned maj,min,rel;

  H5get_libversion(&maj, &min, &rel);

  _hdfVersion = QObject::tr("%1.%2.%3").arg(maj).arg(min).arg(rel);

  return qPrintable(_hdfVersion);
}

#else

const char* qcepH5Version()
{
  return "?.?.?";
}

#endif
