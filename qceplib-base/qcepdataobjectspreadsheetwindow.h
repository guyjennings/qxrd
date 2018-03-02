#ifndef QCEPDATAOBJECTSPREADSHEETWINDOW_H
#define QCEPDATAOBJECTSPREADSHEETWINDOW_H

#include "qceplib_global.h"
#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectspreadsheetwindow.h"
#include "qcepdataobjectwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepDataObjectSpreadsheetWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectSpreadsheetWindow
{
  Q_OBJECT

private:
  typedef QcepDataObjectWindow inherited;

public:
  explicit QcepDataObjectSpreadsheetWindow(
      QString name,
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj);
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
