#ifndef QCEPDATAOBJECTSPREADSHEETWINDOW_H
#define QCEPDATAOBJECTSPREADSHEETWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectspreadsheetwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepspreadsheetmodel.h"

class QcepDataObjectSpreadsheetWindow : public QMainWindow, public Ui::QcepDataObjectSpreadsheetWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectSpreadsheetWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:

private:
  QcepExperimentWPtr                   m_Experiment;
  QcepDataObjectPtr                    m_Object;
  QSharedPointer<QcepSpreadsheetModel> m_Model;
};

#endif // QCEPDATAOBJECTSPREADSHEETWINDOW_H
