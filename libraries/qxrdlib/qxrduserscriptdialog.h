#ifndef QXRDUSERSCRIPTDIALOG_H
#define QXRDUSERSCRIPTDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include <QAbstractButton>
#include "qxrdexperiment-ptr.h"
#include "ui_qxrduserscriptdialog.h"

class QXRD_EXPORT QxrdUserScriptDialog : public QDialog, public Ui::QxrdUserScriptDialog
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
  QString m_Default;
};

#endif // QXRDUSERSCRIPTDIALOG_H
