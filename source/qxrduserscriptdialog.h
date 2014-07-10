#ifndef QXRDUSERSCRIPTDIALOG_H
#define QXRDUSERSCRIPTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "qxrdexperiment-ptr.h"

namespace Ui {
class QxrdUserScriptDialog;
}

class QxrdUserScriptDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit QxrdUserScriptDialog(QString title,
                                QxrdExperimentWPtr experiment,
                                QString editable,
                                QString def,
                                QWidget *parent = 0);
  ~QxrdUserScriptDialog();
  
  QString userScript();

private slots:
  void onButtonClicked(QAbstractButton* button);

protected:
  void changeEvent(QEvent *e);
  
private:
  QxrdExperimentWPtr m_Experiment;
  Ui::QxrdUserScriptDialog *ui;
  QString m_Default;
};

#endif // QXRDUSERSCRIPTDIALOG_H
