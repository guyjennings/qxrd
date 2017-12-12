#include "qxrdfilewatcherdialog.h"

QxrdFileWatcherDialog::QxrdFileWatcherDialog(
    QxrdFileWatcherSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdFileWatcherDialog::accept()
{
  QxrdDetectorDialog::accept();
}
