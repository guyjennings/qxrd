#include "qcepnewdatagroupdialog.h"
#include "ui_qcepnewdatagroupdialog.h"
#include "qcepdataobject.h"

QcepNewDataGroupDialog::QcepNewDataGroupDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new data group in %1").arg(obj->pathName()));
  }
}

QcepNewDataGroupDialog::~QcepNewDataGroupDialog()
{
}

void QcepNewDataGroupDialog::changeEvent(QEvent *e)
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
