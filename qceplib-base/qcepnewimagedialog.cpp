#include "qcepnewimagedialog.h"
#include "ui_qcepnewimagedialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"

QcepNewImageDialog::QcepNewImageDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    setWindowTitle(tr("Create new 2-D image in %1").arg(m_Model->pathName(idx)));

    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      m_ImageName -> setText(expt->get_NewImageName());
      m_ImageWidth -> setValue(expt->get_NewImageWidth());
      m_ImageHeight -> setValue(expt->get_NewImageHeight());
    }
  }
}

QcepNewImageDialog::~QcepNewImageDialog()
{
}

void QcepNewImageDialog::accept()
{
  if (m_Model) {
    QcepExperimentPtr expt = m_Model -> experiment();

    if (expt) {
      QString newImageName = m_ImageName -> text();
      int     newImageWidth = m_ImageWidth -> value();
      int     newImageHeight = m_ImageHeight -> value();

      expt -> set_NewImageName(newImageName);
      expt -> set_NewImageWidth(newImageWidth);
      expt -> set_NewImageHeight(newImageHeight);

      m_Model -> newImage(m_Index, newImageName, newImageWidth, newImageHeight);
    }
  }

  QDialog::accept();
}
