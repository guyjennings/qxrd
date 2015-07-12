#include "qcepnewimagedialog.h"
#include "ui_qcepnewimagedialog.h"
#include "qcepdataobject.h"

QcepNewImageDialog::QcepNewImageDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new 2-D image in %1").arg(obj->pathName()));
  }
}

QcepNewImageDialog::~QcepNewImageDialog()
{
}

void QcepNewImageDialog::changeEvent(QEvent *e)
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
