#include "qtestimagedata.h"

QTestImageData::QTestImageData(QString name, int width, int height, double def) :
  QcepDoubleImageData(name, width, height, def),
  m_Inherited(this, "inherited", "Inherited Property Value", "Inherited Property")
{
}

bool QTestImageData::writeImage(QString fileName)
{
  QcepImageDataBase::set_FileName(fileName);

  return true;
}
