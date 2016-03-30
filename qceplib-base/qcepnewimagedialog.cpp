#include "qcepnewimagedialog.h"
#include "ui_qcepnewimagedialog.h"
#include "qcepdatasetmodel.h"

static QString s_ImageName;
static int s_ImageWidth  = 1024;
static int s_ImageHeight = 1024;

QcepNewImageDialog::QcepNewImageDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new 2-D image in %1").arg(m_Model->pathName(idx)));
  }

  m_ImageName -> setText(s_ImageName);
  m_ImageWidth -> setValue(s_ImageWidth);
  m_ImageHeight -> setValue(s_ImageHeight);
}

QcepNewImageDialog::~QcepNewImageDialog()
{
}

void QcepNewImageDialog::accept()
{
  s_ImageName = m_ImageName -> text();
  s_ImageWidth = m_ImageWidth -> value();
  s_ImageHeight = m_ImageHeight -> value();

  if (m_Model) {
    m_Model -> newImage(m_Index, s_ImageName, s_ImageWidth, s_ImageHeight);
  }

  QDialog::accept();
}
