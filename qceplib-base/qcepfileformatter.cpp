#include "qcepfileformatter.h"
#include "qcepfileformatterhdf.h"
#include "qcepfileformatternexus.h"
#include "qcepfileformattertext.h"

QcepFileFormatter::QcepFileFormatter(QString filePath) :
  QcepObject("fileFormatter", QcepObjectWPtr()),
  m_FilePath(filePath)
{
}

QcepFileFormatterPtr QcepFileFormatter::defaultFormatter(QString filePath, QString exten)
{
#ifdef HAVE_HDF5
  if (exten == "h5") {
    return QcepFileFormatterPtr(new QcepFileFormatterHDF(filePath));
  }
#endif

#ifdef HAVE_NEXUS
  if (exten == "nxs") {
    return QcepFileFormatterPtr(new QcepFileFormatterNexus(filePath));
  }
#endif

  if (exten == "qxrdp2") {
    return QcepFileFormatterPtr(new QcepFileFormatterText(filePath));
  }

  return QcepFileFormatterPtr(new QcepFileFormatterText(filePath));
}
