#include "qxrdnewimagedialog.h"
#include "ui_qxrdnewimagedialog.h"
#include "qcepdataobject.h"

QxrdNewImageDialog::QxrdNewImageDialog(QcepDataObject *obj, QWidget *parent) :
  QDialog(parent),
  m_Object(obj)
{
  setupUi(this);

  if (obj) {
    setWindowTitle(tr("Create new 2-D image in %1").arg(obj->pathName()));
  }
}

QxrdNewImageDialog::~QxrdNewImageDialog()
{
}

void QxrdNewImageDialog::changeEvent(QEvent *e)
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
