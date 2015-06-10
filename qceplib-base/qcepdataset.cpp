#include "qcepdataset.h"

QcepDataset::QcepDataset(QcepSettingsSaverWPtr saver, QString name, QcepDataObjectWPtr parent) :
  QcepDataGroup(saver, name, parent)
{
  set_Type("dataset");
}
