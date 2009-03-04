#include "qxrdwindow.h"
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <QPen>
#include <QFile>
#include <QCheckBox>
#include <QSignalMapper>

QxrdWindow::QxrdWindow(QxrdApplication *app, QWidget *parent)
  : QMainWindow(parent),
    m_Application(app),
    m_AcquisitionThread(NULL),
    m_Progress(NULL)
{
  setupUi(this);

  connect(m_ActionAutoScale, SIGNAL(triggered()), m_Plot, SLOT(autoScale()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(quit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), m_Application, SLOT(loadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), m_Application, SLOT(saveData()));

  connect(m_AcquireButton, SIGNAL(clicked()), m_Application, SLOT(doAcquire()));
  connect(m_CancelButton, SIGNAL(clicked()), m_Application, SLOT(doCancel()));

//   m_HelpText -> setReadOnly(true);

//   QFile file(":/qavrghelptext.html");

//   file.open(QIODevice::ReadOnly);

//   QString contents = file.readAll();

//   m_HelpText -> setHtml(contents);

  for (int i=0; i<8; i++) {
    m_ExposureTime -> addItem(tr("Item %1").arg(i));
    m_Exposures.append(0);
  }

  m_Progress = new QProgressBar(NULL);
  m_Progress -> setMinimumWidth(150);
  m_Progress -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  statusBar() -> addPermanentWidget(m_Progress);
}

void QxrdWindow::setAcquisitionThread(QxrdAcquisitionThread *acq)
{
  m_AcquisitionThread = acq;
}

void QxrdWindow::saveData()
{
}

void QxrdWindow::loadData()
{
}

void QxrdWindow::printMessage(QString msg)
{
  m_Messages -> append(msg.trimmed());
}

void QxrdWindow::setCancelButton()
{
  m_AcquireButton -> setEnabled(false);
  m_CancelButton -> setEnabled(true);
}

void QxrdWindow::setAcquireButton()
{
  m_AcquireButton -> setEnabled(true);
  m_CancelButton -> setEnabled(false);
}

void QxrdWindow::setIntegrationTime(int n, double t)
{
  while (n >= m_ExposureTime->count()) {
    m_ExposureTime -> addItem("");
    m_Exposures.append(0);
  }

  m_ExposureTime -> setItemText(n, tr("%1 msec").arg(t/1e3,0,'f',0));
  m_Exposures[n] = t;
}

void QxrdWindow::setExposureTime(double t)
{
  int best=0;

  for (int i=1; i<8; i++) {
    if (fabs(t-m_Exposures.at(i)) < fabs(t-m_Exposures.at(best))) {
      best = i;
    }
  }

  setIntegrationMode(best);
}

void QxrdWindow::setIntegrationMode(int mode)
{
  m_ExposureTime->setCurrentIndex(mode);
}

void QxrdWindow::setNSummed(int nsummed)
{
  m_SummedFrames->setValue(nsummed);
}

void QxrdWindow::setNFrames(int nframes)
{
  m_SequenceFrames->setValue(nframes);
}

void QxrdWindow::setFileIndex(int index)
{
  m_FileIndexNumber->setValue(index);
}

void QxrdWindow::setFilePattern(QString pattern)
{
  m_SaveFilePattern->setText(pattern);
}

double  QxrdWindow::exposureTime()
{
  int choice = m_ExposureTime->currentIndex();

  return m_Exposures.at(choice);
}

int     QxrdWindow::integrationMode()
{
  return m_ExposureTime->currentIndex();
}

int     QxrdWindow::nSummed()
{
  return m_SummedFrames->value();
}

int     QxrdWindow::nFrames()
{
  return m_SequenceFrames->value();
}

int     QxrdWindow::fileIndex()
{
  return m_FileIndexNumber->value();
}

QString QxrdWindow::filePattern()
{
  return m_SaveFilePattern->text();
}

void QxrdWindow::acquiredFrame(int isum, int nsum, int iframe, int nframe)
{
  int totalframes = nsum*nframe;
  int thisframe = iframe*nsum+isum;

  //  printf("%d %% progress\n", thisframe*100/totalframes);

  m_Progress -> setValue(thisframe*100/totalframes);
}
