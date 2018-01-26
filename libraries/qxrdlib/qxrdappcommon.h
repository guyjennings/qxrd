#ifndef QXRDAPPCOMMON_H
#define QXRDAPPCOMMON_H

#include "qxrdlib_global.h"
#include "qcepproperty.h"
#include "qcepapplication.h"
#include "qxrdappcommonsettings-ptr.h"
#include "qxrdsplashscreen-ptr.h"
#include <QTimer>

class QXRD_EXPORT QxrdAppCommon : public QcepApplication
{
  Q_OBJECT

private:
  typedef QcepApplication inherited;

public:
  QxrdAppCommon(int &argc, char **argv);
  virtual ~QxrdAppCommon();

  virtual bool init(int &argc, char **argv);
  virtual void finish();

  void parseCommandLine(bool wantFullOptions);

  void splashMessage(QString msg);

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

protected:
  QxrdAppCommonSettingsPtr settings();
  void setSettings(QxrdAppCommonSettingsPtr set);

  virtual QString applicationDescription() = 0;

private:
  void hideSplash();
  void setupTiffHandlers();

  QxrdAppCommonSettingsPtr        m_ApplicationSettings;
  QTimer                          m_SplashTimer;
  QxrdSplashScreenPtr             m_Splash;
};

#endif // QXRDAPPCOMMON_H
