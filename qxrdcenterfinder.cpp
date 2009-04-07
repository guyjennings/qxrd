#include "qxrdcenterfinder.h"

QxrdCenterFinder::QxrdCenterFinder
    (QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent)
  : QObject(parent),
    m_Imageplot(imgplot),
    m_CenterFinderPlot(cntplot),
    m_CenterFinderDialog(cfdialog)
{
}
