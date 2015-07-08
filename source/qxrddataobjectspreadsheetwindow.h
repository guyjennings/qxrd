#ifndef QXRDDATAOBJECTSPREADSHEETWINDOW_H
#define QXRDDATAOBJECTSPREADSHEETWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qxrddataobjectspreadsheetwindow.h"
#include "qxrdexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepspreadsheetmodel.h"

class QxrdDataObjectSpreadsheetWindow : public QMainWindow, public Ui::QxrdDataObjectSpreadsheetWindow
{
  Q_OBJECT
public:
  explicit QxrdDataObjectSpreadsheetWindow(
      QxrdExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:

private:
  QxrdExperimentWPtr                   m_Experiment;
  QcepDataObjectPtr                    m_Object;
  QSharedPointer<QcepSpreadsheetModel> m_Model;
};

#endif // QXRDDATAOBJECTSPREADSHEETWINDOW_H
