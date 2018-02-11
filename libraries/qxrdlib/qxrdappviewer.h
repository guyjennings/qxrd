#ifndef QXRDAPPVIEWER_H
#define QXRDAPPVIEWER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qxrdappcommon.h"

class QXRD_EXPORT QxrdAppViewer : public QxrdAppCommon
{
  Q_OBJECT

private:
  typedef QxrdAppCommon inherited;

public:
  explicit QxrdAppViewer(int &argc, char **argv);
  virtual ~QxrdAppViewer();
  void initialize(QcepObjectWPtr parent);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  void setDefaultObjectData(QcepDataObject *obj);

  void openExperiment(QString path);

  void readApplicationSettings();
  void writeApplicationSettings();

  void editGlobalPreferences();
  void createNewExperiment();
  void chooseExistingExperiment();

private:
  QString applicationDescription();
};

#endif // QXRDAPPVIEWER_H
