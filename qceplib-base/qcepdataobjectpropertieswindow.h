#ifndef QCEPDATAOBJECTPROPERTIESWINDOW_H
#define QCEPDATAOBJECTPROPERTIESWINDOW_H

#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectpropertieswindow.h"
#include "qcepdataobjectwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qceppropertiesmodel.h"

class QcepDataObjectPropertiesWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectPropertiesWindow
{
  Q_OBJECT
public:
  explicit QcepDataObjectPropertiesWindow(
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj,
      QWidget *parent = 0);
  virtual ~QcepDataObjectPropertiesWindow();

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

signals:

public slots:

private:
  QcepExperimentWPtr                  m_Experiment;
  QcepDataObjectWPtr                  m_Object;
  QSharedPointer<QcepPropertiesModel> m_Model;
};

#endif // QCEPDATAOBJECTPROPERTIESWINDOW_H
