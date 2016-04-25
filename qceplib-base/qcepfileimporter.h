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
  QcepFileImporter(QcepObjectWPtr parent,
                   QcepDatasetModelPtr model,
                   QModelIndexList &indexes,
                   QString path);

  virtual void exec();

  static QcepFileImporterPtr importFile(QcepObjectWPtr parent,
                                        QcepDatasetModelPtr model,
                                        QModelIndexList &indexes,
                                        QString path);

  static QcepFileImporterPtr importFiles(QcepObjectWPtr parent,
                                         QcepDatasetModelPtr model,
                                         QModelIndexList &indexes,
                                         QStringList paths);

signals:
  void importStarted();
  void importProgress(double pct);
  void importCompleted();

protected:
  QcepDatasetModelPtr m_Model;
  QModelIndexList    &m_Indexes;
  QString             m_Path;
};

#endif // QCEPFILEIMPORTER_H
