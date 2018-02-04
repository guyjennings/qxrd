#include "qcepdataset.h"
#include "qcepmutexlocker.h"

QcepDataset::QcepDataset(QString name) :
  inherited(name)
{
}

void QcepDataset::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}

//void QcepDataset::writeSettings(QSettings *settings, QString section)
//{
//  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  if (settings) {
//    settings->beginGroup("ds");

//    QcepDataGroup::writeSettings(settings, section);

//    settings->endGroup();
//  }
//}

//void QcepDataset::readSettings(QSettings *settings, QString section)
//{
//  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

//  if (settings) {
//    settings->beginGroup("ds");

//    QcepDataGroup::readSettings(settings, section);

//    settings->endGroup();
//  }
//}
