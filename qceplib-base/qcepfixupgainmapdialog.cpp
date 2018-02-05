#include "qcepfixupgainmapdialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepfixupgainmapcommand.h"

QcepFixupGainMapDialog::QcepFixupGainMapDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  inherited(NULL),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Command = m_Experiment->fixupGainMapCommand();
    }
  }

  if (m_Command) {
    m_ProgressIndicator->hide();

    m_AverageSubTiles->setChecked(m_Command->get_AverageSubTiles());
    m_AverageWidth->setRange(-qInf(), qInf());
    m_AverageWidth->setValue(m_Command->get_AverageWidth());
    m_AverageHeight->setRange(-qInf(), qInf());
    m_AverageHeight->setValue(m_Command->get_AverageHeight());
    m_GenerateAveragedImage->setChecked(m_Command->get_GenerateAveragedImage());
    m_GenerateAveragedDiff->setChecked(m_Command->get_GenerateAveragedDiff());
    m_FitParaboloid->setChecked(m_Command->get_FitParaboloid());
    m_GenerateParaboloid->setChecked(m_Command->get_GenerateParaboloid());
    m_GenerateParaboloidDiff->setChecked(m_Command->get_GenerateParaboloidDiff());
  }
}

QcepFixupGainMapDialog::~QcepFixupGainMapDialog()
{
}

void QcepFixupGainMapDialog::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);

  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QcepFixupGainMapDialog::accept()
{
  if (m_Command) {
    m_Command->set_AverageSubTiles(m_AverageSubTiles->isChecked());
    m_Command->set_AverageWidth(m_AverageWidth->value());
    m_Command->set_AverageHeight(m_AverageHeight->value());
    m_Command->set_GenerateAveragedImage(m_GenerateAveragedImage->isChecked());
    m_Command->set_GenerateAveragedDiff(m_GenerateAveragedDiff->isChecked());
    m_Command->set_FitParaboloid(m_FitParaboloid->isChecked());
    m_Command->set_GenerateParaboloid(m_GenerateParaboloid->isChecked());
    m_Command->set_GenerateParaboloidDiff(m_GenerateParaboloidDiff->isChecked());

    QcepDoubleImageDataPtr input = m_Model->image(m_Index);

    if (input) {
      connect(m_Command.data(), &QcepFixupGainMapCommand::progressMade,
              m_ProgressIndicator, &QProgressBar::setValue);

      m_ProgressIndicator->show();

      QcepDoubleImageDataPtr mod = m_Command->exec(input);

      QModelIndex p = m_Model->parent(m_Index);

      int n = m_Command->intermediateResultCount();

      for (int i=0; i<n; i++) {
        QcepDataObjectPtr r = m_Command->intermediateResult(i);

        m_Model->append(p, r);
      }

      m_Model->append(p, mod);
    }
  }

  inherited::accept();
}
