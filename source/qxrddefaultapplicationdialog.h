#ifndef QXRDDEFAULTAPPLICATIONDIALOG_H
#define QXRDDEFAULTAPPLICATIONDIALOG_H

#include <QDialog>

class QxrdDefaultApplication;

namespace Ui {
class QxrdDefaultApplicationDialog;
}

class QxrdDefaultApplicationDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdDefaultApplicationDialog(QxrdDefaultApplication *app = 0);
  ~QxrdDefaultApplicationDialog();

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
  QxrdDefaultApplication           *m_Application;
  Ui::QxrdDefaultApplicationDialog *ui;
};

#endif // QXRDDEFAULTAPPLICATIONDIALOG_H
