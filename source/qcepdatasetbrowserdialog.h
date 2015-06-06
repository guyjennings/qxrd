#ifndef QCEPDATASETBROWSERDIALOG_H
#define QCEPDATASETBROWSERDIALOG_H

#include <QDockWidget>
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdataset.h"
#include "qcepdataset-ptr.h"
#include "qcepdatasetmodel-ptr.h"

class QcepDatasetBrowserDialog : public QDockWidget, public Ui::QcepDatasetBrowserDialog
{
  Q_OBJECT

public:
  explicit QcepDatasetBrowserDialog(QcepDatasetPtr ds, QWidget *parent = 0);
  virtual ~QcepDatasetBrowserDialog();

private:
  QcepDatasetPtr      m_Dataset;
  QcepDatasetModelPtr m_DatasetModel;
};

#endif // QCEPDATASETBROWSERDIALOG_H
