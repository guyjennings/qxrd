#ifndef QXRDDEFAULTAPPLICATIONDIALOG_H
#define QXRDDEFAULTAPPLICATIONDIALOG_H

#include <QDialog>

class QxrdApplication;

namespace Ui {
class QxrdNewExperimentDialog;
}

class QxrdNewExperimentDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdNewExperimentDialog(QxrdApplication *app = 0);
  ~QxrdNewExperimentDialog();

  bool choose();

  int     chosenKind();
  QString chosenPath();

  typedef enum {
    NoChoice = -2,
    Existing = -1,
    Analysis = 0,
    PerkinElmer = 1,
    Pilatus = 2
  } Choice;

protected:
  void changeEvent(QEvent *e);

private slots:
  void choosePerkinElmer();
  void choosePilatus();
  void chooseAnalysis();
  void chooseRecent(QString path);

private:
  int                               m_ChosenKind;
  QString                           m_ChosenPath;
  QxrdApplication                  *m_Application;
  Ui::QxrdNewExperimentDialog      *ui;
};

#endif // QXRDDEFAULTAPPLICATIONDIALOG_H
