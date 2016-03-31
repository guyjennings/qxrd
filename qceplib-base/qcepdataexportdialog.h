#ifndef QCEPDATAEXPORTDIALOG_H
#define QCEPDATAEXPORTDIALOG_H

#include <QDialog>
#include "ui_qcepdataexportdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include <QModelIndexList>

class QcepDataExportDialog : public QDialog, public Ui::QcepDataExportDialog
{
  Q_OBJECT

public:
  explicit QcepDataExportDialog(QcepDatasetModelPtr model, const QModelIndexList &idx);
  ~QcepDataExportDialog();

  void accept();

private:
  QcepDatasetModelPtr  m_Model;
  QModelIndexList      m_Indexes;
};

#endif // QCEPDATAEXPORTDIALOG_H
