#ifndef QXRDHELPBROWSER_H
#define QXRDHELPBROWSER_H

#include "qxrdlib_global.h"
#include <QTextBrowser>
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdHelpBrowser : public QTextBrowser
{
  Q_OBJECT
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
