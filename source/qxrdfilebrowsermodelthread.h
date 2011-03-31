#ifndef QXRDFILEBROWSERMODELTHREAD_H
#define QXRDFILEBROWSERMODELTHREAD_H

#include <QThread>
#include "qxrdfilebrowsermodel.h"

class QxrdFileBrowserModelThread : public QThread
{
  Q_OBJECT
public:
  explicit QxrdFileBrowserModelThread(QObject *parent = 0);

  QxrdFileBrowserModel *fileBrowserModel() const;

protected:
  void run();

private:
  QAtomicPointer<QxrdFileBrowserModel> m_FileBrowserModel;
};

#endif // QXRDFILEBROWSERMODELTHREAD_H
