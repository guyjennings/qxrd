#ifndef QCEPNEWIMAGEDIALOG_H
#define QCEPNEWIMAGEDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepnewimagedialog.h"

#include "qcepdatasetmodel-ptr.h"

class QCEP_EXPORT QcepNewImageDialog : public QDialog, public Ui::QcepNewImageDialog
{
  Q_OBJECT

public:
  explicit QcepNewImageDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepNewImageDialog();

  void accept();

private:
  QcepDatasetModelPtr m_Model;
  QModelIndex         m_Index;
};

#endif // QCEPNEWIMAGEDIALOG_H
