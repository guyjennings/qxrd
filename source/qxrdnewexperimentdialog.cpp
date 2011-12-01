#include "qxrdnewexperimentdialog.h"
#include "ui_qxrdnewexperimentdialog.h"
#include "qxrdapplication.h"
#include <QSignalMapper>
#include <QFileDialog>

QxrdNewExperimentDialog::QxrdNewExperimentDialog(QxrdApplication *app) :
  QDialog(NULL),
  m_ChosenKind(NoChoice),
  m_ChosenPath(""),
  m_Application(app),
  ui(new Ui::QxrdNewExperimentDialog)
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

QxrdNewExperimentDialog::~QxrdNewExperimentDialog()
{
    delete ui;
}

void QxrdNewExperimentDialog::changeEvent(QEvent *e)
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

bool QxrdNewExperimentDialog::choose()
{
  int dc = exec();

  bool result = false;

  switch (dc) {
  case QDialog::Accepted:
    result = true;
    break;

  case QDialog::Rejected:
    break;
  }

  return result;
}

void QxrdNewExperimentDialog::choosePerkinElmer()
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

void QxrdNewExperimentDialog::choosePilatus()
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

void QxrdNewExperimentDialog::chooseAnalysis()
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

void QxrdNewExperimentDialog::chooseRecent(QString path)
{
  ui->m_Chosen->setText(path);

  m_ChosenKind = Existing;
  m_ChosenPath = path;
}

int QxrdNewExperimentDialog::chosenKind()
{
  return m_ChosenKind;
}

QString QxrdNewExperimentDialog::chosenPath()
{
  return m_ChosenPath;
}
