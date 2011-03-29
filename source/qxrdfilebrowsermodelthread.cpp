#include "qxrdfilebrowsermodelthread.h"

QxrdFileBrowserModelThread::QxrdFileBrowserModelThread(QObject *parent) :
    QThread(),
    m_FileBrowserModel(NULL)
{
}

QxrdFileBrowserModel* QxrdFileBrowserModelThread::fileBrowserModel() const
{
  while (m_FileBrowserModel == NULL) {
    QThread::msleep(100);
  }

  return m_FileBrowserModel;
}

void QxrdFileBrowserModelThread::run()
{
//  QxrdFileBrowserModel *model = new QxrdFileBrowserModel();

//  m_FileBrowserModel.fetchAndStoreOrdered(model);

  exec();
}
