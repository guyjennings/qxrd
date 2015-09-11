#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrddetectorconfigurationdialog.h"
#include "ui_qxrddetectorconfigurationdialog.h"
#include "qxrddetectorproxy.h"
#include <QThread>
#include <stdio.h>

QxrdDetectorConfigurationDialog::QxrdDetectorConfigurationDialog(QxrdDetectorProxyPtr proxy, QWidget *parent) :
  QDialog(parent),
  m_Proxy(proxy)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  if (m_Proxy) {
    setWindowTitle(tr("Configure %1 Detector").arg(m_Proxy->detectorTypeName()));

    m_Proxy->pushPropertiesToDialog(this);
  }
}

QxrdDetectorConfigurationDialog::~QxrdDetectorConfigurationDialog()
{
}

void QxrdDetectorConfigurationDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdDetectorConfigurationDialog::accept()
{
  if (m_Proxy) {
    m_Proxy->pullPropertiesFromDialog(this);
  }

  QDialog::accept();
}
