#ifndef QCEPDATAOBJECTPROPERTIESWINDOW_H
#define QCEPDATAOBJECTPROPERTIESWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectpropertieswindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qceppropertiesmodel.h"

class QcepDataObjectPropertiesWindow : public QMainWindow, public Ui::QcepDataObjectPropertiesWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectPropertiesWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectPtr  obj,
      QWidget *parent = 0);

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:

private:
  QcepExperimentWPtr                  m_Experiment;
  QcepDataObjectPtr                   m_Object;
  QSharedPointer<QcepPropertiesModel> m_Model;
};

#endif // QCEPDATAOBJECTPROPERTIESWINDOW_H
