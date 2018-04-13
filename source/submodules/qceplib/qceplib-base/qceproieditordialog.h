#ifndef QCEPROIEDITORDIALOG_H
#define QCEPROIEDITORDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "qceproi-ptr.h"
#include "ui_qceproieditordialog.h"

class QCEP_EXPORT QcepROIEditorDialog : public QDialog, public Ui::QcepROIEditorDialog
{
  Q_OBJECT

public:
  explicit QcepROIEditorDialog(QcepROIWPtr roi, QWidget *parent = 0);
  ~QcepROIEditorDialog();

public slots:
  void accept();

public:
  QcepROIWPtr roi();

private slots:
  void newInnerType(int n);
  void newOuterType(int n);

  void innerAddPoint();
  void innerDelPoint();
  void outerAddPoint();
  void outerDelPoint();

private:
  QcepROIWPtr m_ROI;
};

#endif // QCEPROIEDITORDIALOG_H
