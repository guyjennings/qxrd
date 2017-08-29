#ifndef QXRDACQUISITIONPARAMETERPACK_H
#define QXRDACQUISITIONPARAMETERPACK_H

#include <QString>

class QxrdAcquisitionParameterPack
{
public:
  QxrdAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int preTrigger, int postTrigger, int nphases, int skipBefore, int skipBetween)
    : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_PreTrigger(preTrigger), m_PostTrigger(postTrigger),
      m_NPhases(nphases), m_SkipBefore(skipBefore), m_SkipBetween(skipBetween) {}

  QString fileBase() { return m_FileBase; }
  double  exposure() { return m_Exposure; }
  int     nsummed()  { return m_NSummed; }
  int     preTrigger()   { return m_PreTrigger; }
  int     postTrigger()   { return m_PostTrigger; }
  int     nphases()  { return m_NPhases; }
  int     skipBefore() { return m_SkipBefore; }
  int     skipBetween() { return m_SkipBetween; }

  void    forceSingle();

private:
  QString m_FileBase;
  double  m_Exposure;
  int     m_NSummed;
  int     m_PreTrigger;
  int     m_PostTrigger;
  int     m_NPhases;
  int     m_SkipBefore;
  int     m_SkipBetween;
};

#endif // QXRDACQUISITIONPARAMETERPACK_H
