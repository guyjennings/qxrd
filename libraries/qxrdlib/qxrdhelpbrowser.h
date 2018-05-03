#ifndef QXRDHELPBROWSER_H
#define QXRDHELPBROWSER_H

#include "qxrdlib_global.h"
#include "qceptextbrowser.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdHelpBrowser : public QcepTextBrowser
{
  Q_OBJECT

private:
  typedef QcepTextBrowser inherited;

public:
  explicit QxrdHelpBrowser(QWidget *parent = 0);
  virtual ~QxrdHelpBrowser();

  //TODO: change to QcepObjectWPtr...
  void initialize(QxrdExperimentWPtr expt);

  QVariant loadResource(int type, const QUrl &name);

signals:

public slots:

private:
  QxrdExperimentWPtr   m_Experiment;
};

#endif // QXRDHELPBROWSER_H
