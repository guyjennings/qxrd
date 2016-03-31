#ifndef QCEPDATAIMPORTDIALOG_H
#define QCEPDATAIMPORTDIALOG_H

#include <QDialog>
#include "ui_qcepdataimportdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include <QModelIndexList>

class QcepDataImportDialog : public QDialog, public Ui::QcepDataImportDialog
{
  Q_OBJECT

public:
  explicit QcepDataImportDialog(QcepDatasetModelPtr model, const QModelIndexList &idx);
  ~QcepDataImportDialog();

  void accept();

private:
  QcepDatasetModelPtr  m_Model;
  QModelIndexList      m_Indexes;
};

#endif // QCEPDATAIMPORTDIALOG_H
