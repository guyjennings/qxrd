#ifndef QXRDDARKACQUISITIONPARAMETERPACK_H
#define QXRDDARKACQUISITIONPARAMETERPACK_H

#include "qxrdlib_global.h"
#include <QString>

class QXRD_EXPORT QxrdDarkAcquisitionParameterPack
{
public:
  QxrdDarkAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int skipBefore)
    : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_SkipBefore(skipBefore) {}

  QString fileBase() { return m_FileBase; }
  double  exposure() { return m_Exposure; }
  int     nsummed()  { return m_NSummed; }
  int     skipBefore() { return m_SkipBefore; }

private:
  QString m_FileBase;
  double  m_Exposure;
  int     m_NSummed;
  int     m_SkipBefore;
};

#endif // QXRDDARKACQUISITIONPARAMETERPACK_H
