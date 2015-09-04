#include "qcepdataset.h"

QcepDataset::QcepDataset(QcepSettingsSaverWPtr saver, QString name, QcepObject *parent) :
  QcepDataGroup(saver, name, parent)
{
  set_Type("dataset");
}
