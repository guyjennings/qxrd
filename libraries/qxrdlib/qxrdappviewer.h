#ifndef QXRDAPPVIEWER_H
#define QXRDAPPVIEWER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qxrdappcommon.h"
#include "qxrdappviewersettings-ptr.h"

class QXRD_EXPORT QxrdAppViewer : public QxrdAppCommon
{
  Q_OBJECT

private:
  typedef QxrdAppCommon inherited;

public:
  QxrdAppViewer(int &argc, char **argv);
  virtual ~QxrdAppViewer();
  void initialize();

  //TODO: merge into 'initialize'
  bool init(int &argc, char **argv);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void setDefaultObjectData(QcepDataObject *obj);

  QxrdAppViewerSettingsPtr settings();

  void openExperiment(QString path);

  void readSettings();
  void writeSettings();

  void editGlobalPreferences();
  void createNewExperiment();
  void chooseExistingExperiment();

private:
  QString applicationDescription();

private:
//  QxrdAppViewerSettingsPtr      m_AppViewerSettings;
};

#endif // QXRDAPPVIEWER_H
