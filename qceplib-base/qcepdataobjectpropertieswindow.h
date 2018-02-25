#ifndef QCEPDATAOBJECTPROPERTIESWINDOW_H
#define QCEPDATAOBJECTPROPERTIESWINDOW_H

#include "qceplib_global.h"
#include <QObject>
#include <QWidget>
#include "ui_qcepdataobjectpropertieswindow.h"
#include "qcepdataobjectwindow.h"
#include "qcepexperiment-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qceppropertiesmodel.h"

class QCEP_EXPORT QcepDataObjectPropertiesWindow : public QcepDataObjectWindow, public Ui::QcepDataObjectPropertiesWindow
{
  Q_OBJECT

private:
  typedef QcepDataObjectWindow inherited;

public:
  explicit QcepDataObjectPropertiesWindow(
      QString name,
      QcepExperimentWPtr expt,
      QcepDataObjectWPtr  obj);
  virtual ~QcepDataObjectPropertiesWindow();

  void closeEvent (QCloseEvent * event);
  bool wantToClose();

  void saveExperimentAs();
  void saveExperimentCopy();
  void doEditPreferences();

signals:

public slots:

private:
  QcepExperimentWPtr                  m_Experiment;
  QcepDataObjectWPtr                  m_Object;
  QSharedPointer<QcepPropertiesModel> m_Model;
};

#endif // QCEPDATAOBJECTPROPERTIESWINDOW_H
