#include "qcepnewcolumnscandialog.h"
#include "ui_qcepnewcolumnscandialog.h"
#include "qcepdataobject.h"

QcepNewColumnScanDialog::QcepNewColumnScanDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new column scan in %1").arg(obj->pathName()));
  }
}

QcepNewColumnScanDialog::~QcepNewColumnScanDialog()
{
}

void QcepNewColumnScanDialog::changeEvent(QEvent *e)
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
