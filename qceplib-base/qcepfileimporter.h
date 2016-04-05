#ifndef QCEPFILEIMPORTER_H
#define QCEPFILEIMPORTER_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepfileimporter-ptr.h"
#include <QModelIndexList>

class QcepFileImporter : public QcepObject
{
  Q_OBJECT

public:
  QcepFileImporter(QcepDatasetModelPtr model,
                   QModelIndexList &indexes,
                   QString path);

  virtual void start();
  virtual void exec();

  static QcepFileImporterPtr importFile(QcepDatasetModelPtr model,
                                        QModelIndexList &indexes,
                                        QString path);

  static QcepFileImporterPtr importFiles(QcepDatasetModelPtr model,
                                         QModelIndexList &indexes,
                                         QStringList paths);
protected:
  QcepDatasetModelPtr m_Model;
  QModelIndexList    &m_Indexes;
  QString             m_Path;
};

#endif // QCEPFILEIMPORTER_H
