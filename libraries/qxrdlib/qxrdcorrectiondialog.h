#ifndef QXRDCORRECTIONDIALOG_H
#define QXRDCORRECTIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "ui_qxrdcorrectiondialog.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdcorrectiondialogpage-ptr.h"

class QXRD_EXPORT QxrdCorrectionDialog : public QDialog, public Ui::QxrdCorrectionDialog
{
  Q_OBJECT

private:
  typedef QDialog inherited;

public:
  explicit QxrdCorrectionDialog(QWidget *parent,
                                QxrdAcqCommonWPtr acqp,
                                QxrdProcessorWPtr procw);
  ~QxrdCorrectionDialog();

public slots:
  void doEditPreferences();

protected:
  void changeEvent(QEvent *e);

private:
  void accept();
  void appendDetectorPage(int i, QxrdDetectorSettingsWPtr det);
  void acceptDetectorPage(int i);
  void copyDetectorPage(int i);
  void applyToAll(bool toAll);

private:
  QxrdAcqCommonWPtr m_Acquisition;
  QxrdProcessorWPtr m_Processor;
  QVector<QxrdCorrectionDialogPage*> m_Pages;
};

#endif // QXRDCORRECTIONDIALOG_H
