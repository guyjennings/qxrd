#ifndef QCEPFILEIMPORTER_H
#define QCEPFILEIMPORTER_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepfileimporter-ptr.h"
#include <QModelIndexList>

class QCEP_EXPORT QcepFileImporter : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepFileImporter(QcepDatasetModelPtr model,
                   QModelIndexList &indexes,
                   QString path);

  virtual void exec();

  static QcepFileImporterPtr importFile(QcepDatasetModelPtr model,
                                        QModelIndexList &indexes,
                                        QString path);

  static QcepFileImporterPtr importFiles(QcepDatasetModelPtr model,
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
