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

//   m_HelpText -> setReadOnly(true);

//   QFile file(":/qavrghelptext.html");

//   file.open(QIODevice::ReadOnly);

//   QString contents = file.readAll();

//   m_HelpText -> setHtml(contents);
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

