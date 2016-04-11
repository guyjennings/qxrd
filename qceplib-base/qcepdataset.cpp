#include "qcepdataset.h"

QcepDataset::QcepDataset(QcepSettingsSaverWPtr saver, QString name) :
  QcepDataGroup(saver, name)
{
  set_Type("dataset");
}
