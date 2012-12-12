#ifndef QXRDHELPBROWSER_H
#define QXRDHELPBROWSER_H

#include <QTextBrowser>
#include "qxrdexperiment-ptr.h"

class QxrdHelpBrowser : public QTextBrowser
{
  Q_OBJECT
public:
  explicit QxrdHelpBrowser(QWidget *parent = 0);
  virtual ~QxrdHelpBrowser();

  void init(QxrdExperimentWPtr expt);

  QVariant loadResource(int type, const QUrl &name);

signals:

public slots:

private:
  QxrdExperimentWPtr   m_Experiment;
};

#endif // QXRDHELPBROWSER_H
