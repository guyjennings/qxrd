#include "qtestimagedata.h"

QTestImageData::QTestImageData(QcepSettingsSaverWPtr saver, int width, int height, double def) :
  QcepImageData<double>(saver, width, height, def),
  m_Inherited(saver, this, "inherited", "Inherited Property Value", "Inherited Property")
{
}

bool QTestImageData::writeImage(QString fileName)
{
  QcepImageDataBase::set_FileName(fileName);

  return true;
}
