#include "qxrddefaultapplicationdialog.h"
#include "ui_qxrddefaultapplicationdialog.h"
#include "qxrddefaultapplication.h"
#include <QSignalMapper>
#include <QFileDialog>

QxrdDefaultApplicationDialog::QxrdDefaultApplicationDialog(QxrdDefaultApplication *app) :
  QDialog(NULL),
  m_ChosenKind(NoChoice),
  m_ChosenPath(""),
  m_Application(app),
  ui(new Ui::QxrdDefaultApplicationDialog)
{
  ui->setupUi(this);

  QStringList prev = app->get_RecentExperiments();
  QFormLayout *layout = ui->m_FormLayout;

  connect(ui->m_PerkinElmer,       SIGNAL(clicked()), this, SLOT(choosePerkinElmer()));
  connect(ui->m_Pilatus,           SIGNAL(clicked()), this, SLOT(choosePilatus()));
  connect(ui->m_Analysis,          SIGNAL(clicked()), this, SLOT(chooseAnalysis()));

  connect(ui->m_ChoosePerkinElmer, SIGNAL(clicked()), this, SLOT(choosePerkinElmer()));
  connect(ui->m_ChoosePilatus,     SIGNAL(clicked()), this, SLOT(choosePilatus()));
  connect(ui->m_ChooseAnalysis,    SIGNAL(clicked()), this, SLOT(chooseAnalysis()));

  foreach (QString exp, prev) {
    QRadioButton *rb = new QRadioButton(exp, this);
    QSignalMapper *sm = new QSignalMapper(this);

    sm -> setMapping(rb, exp);

    connect(rb, SIGNAL(clicked()), sm, SLOT(map()));
    connect(sm, SIGNAL(mapped(QString)), this, SLOT(chooseRecent(QString)));

    layout->addRow(rb);
  }
}

QxrdDefaultApplicationDialog::~QxrdDefaultApplicationDialog()
{
    delete ui;
}

void QxrdDefaultApplicationDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString QxrdDefaultApplicationDialog::choose()
{
  int dc = exec();

  QString result;

  switch (dc) {
  case QDialog::Accepted:
    break;

  case QDialog::Rejected:
    break;
  }

  return result;
}

void QxrdDefaultApplicationDialog::choosePerkinElmer()
{
  ui->m_Chosen->setText("Perkin Elmer");
  ui->m_PerkinElmer->setChecked(true);

  QString newExperiment = QFileDialog::getSaveFileName(this,
                                                       "New Perkin Elmer Experiment",
                                                       m_Application->get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() > 0) {
    m_ChosenKind = PerkinElmer;
    m_ChosenPath = newExperiment;
  }
}

void QxrdDefaultApplicationDialog::choosePilatus()
{
  ui->m_Chosen->setText("Pilatus");
  ui->m_Pilatus->setChecked(true);

  QString newExperiment = QFileDialog::getSaveFileName(this,
                                                       "New Pilatus Experiment",
                                                       m_Application->get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() > 0) {
    m_ChosenKind = Pilatus;
    m_ChosenPath = newExperiment;
  }
}

void QxrdDefaultApplicationDialog::chooseAnalysis()
{
  ui->m_Chosen->setText("Analysis");
  ui->m_Analysis->setChecked(true);

  QString newExperiment = QFileDialog::getSaveFileName(this,
                                                       "New Pilatus Experiment",
                                                       m_Application->get_CurrentExperiment(),
                                                       "QXRD Experiments (*.qxrdp)");

  if (newExperiment.length() > 0) {
    m_ChosenKind = Analysis;
    m_ChosenPath = newExperiment;
  }
}

void QxrdDefaultApplicationDialog::chooseRecent(QString path)
{
  ui->m_Chosen->setText(path);

  m_ChosenKind = Existing;
  m_ChosenPath = path;
}
