#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include <QObject>

class QxrdImagePlot;
class QxrdCenterFinderPlot;
class QxrdCenterFinderDialog;

class QxrdCenterFinder : public QObject
{
  Q_OBJECT;

public:
  QxrdCenterFinder(QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent=0);

private:
  QxrdImagePlot             *m_Imageplot;
  QxrdCenterFinderPlot      *m_CenterFinderPlot;
  QxrdCenterFinderDialog    *m_CenterFinderDialog;
};

#endif // QXRDCENTERFINDER_H
