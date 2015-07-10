#include "qcepnewdataarraydialog.h"
#include "ui_qcepnewdataarraydialog.h"
#include "qcepdataobject.h"

QcepNewDataArrayDialog::QcepNewDataArrayDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new data array in %1").arg(obj->pathName()));
  }
}

QcepNewDataArrayDialog::~QcepNewDataArrayDialog()
{
}

void QcepNewDataArrayDialog::changeEvent(QEvent *e)
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
