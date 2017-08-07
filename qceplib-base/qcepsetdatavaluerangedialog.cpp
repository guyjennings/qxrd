#include "qcepsetdatavaluerangedialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepsetdatavaluerangecommand.h"

QcepSetDataValueRangeDialog::QcepSetDataValueRangeDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(NULL),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Command = m_Experiment->setDataValueRangeCommand();
    }
  }
}

QcepSetDataValueRangeDialog::~QcepSetDataValueRangeDialog()
{
}

void QcepSetDataValueRangeDialog::changeEvent(QEvent *e)
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

void QcepSetDataValueRangeDialog::accept()
{
  if (m_Command) {
    QcepDoubleImageDataPtr input = m_Model->image(m_Index);

    if (input) {
      QcepDoubleImageDataPtr mod = m_Command->exec(input);

      QModelIndex p = m_Model->parent(m_Index);

      m_Model->append(p, mod);
    }
  }

  QDialog::accept();
}
