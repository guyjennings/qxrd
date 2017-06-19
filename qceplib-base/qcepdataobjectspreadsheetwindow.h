#ifndef QCEPDATAOBJECTSPREADSHEETWINDOW_H
#define QCEPDATAOBJECTSPREADSHEETWINDOW_H

#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectspreadsheetwindow.h"
#include "qcepdataobjectwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepspreadsheetmodel.h"

class QcepDataObjectSpreadsheetWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectSpreadsheetWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectSpreadsheetWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj,
      QWidget *parent = 0);
  virtual ~QcepDataObjectSpreadsheetWindow();

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:

private:
  QcepExperimentWPtr                   m_Experiment;
  QcepDataObjectWPtr                   m_Object;
  QSharedPointer<QcepSpreadsheetModel> m_Model;
};

#endif // QCEPDATAOBJECTSPREADSHEETWINDOW_H
