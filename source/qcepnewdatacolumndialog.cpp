#include "qcepnewdatacolumndialog.h"
#include "ui_qcepnewdatacolumndialog.h"
#include "qcepdataobject.h"

QcepNewDataColumnDialog::QcepNewDataColumnDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new data column in %1").arg(obj->pathName()));
  }
}

QcepNewDataColumnDialog::~QcepNewDataColumnDialog()
{
}

void QcepNewDataColumnDialog::changeEvent(QEvent *e)
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
