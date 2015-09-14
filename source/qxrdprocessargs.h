#ifndef QXRDPROCESSARGS_H
#define QXRDPROCESSARGS_H

#include <QString>
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QxrdProcessArgs {
public:
  QxrdProcessArgs(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QcepInt32ImageDataPtr image, QcepMaskDataPtr overflow)
    : m_FilePattern(filePattern),
      m_FileIndex(fileIndex),
      m_Phase(phase),
      m_NPhases(nPhases),
      m_Trig(trig),
      m_Image(image),
      m_Overflow(overflow) {}

public:
  QString               m_FilePattern;
  int                   m_FileIndex;
  int                   m_Phase;
  int                   m_NPhases;
  bool                  m_Trig;
  QcepInt32ImageDataPtr m_Image;
  QcepMaskDataPtr       m_Overflow;
};

#endif // QXRDPROCESSARGS_H