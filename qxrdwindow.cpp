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
    m_AcquisitionThread(NULL)
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
  }
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
  m_Messages -> append(msg);
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
  }

  m_ExposureTime -> setItemText(n, tr("%1: = %2").arg(n).arg(t));
}
