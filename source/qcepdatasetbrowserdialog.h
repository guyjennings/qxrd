#ifndef QCEPDATASETBROWSERDIALOG_H
#define QCEPDATASETBROWSERDIALOG_H

#include <QDockWidget>
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qxrdexperiment-ptr.h"

class QcepDatasetBrowserDialog : public QDockWidget, public Ui::QcepDatasetBrowserDialog
{
  Q_OBJECT

public:
  explicit QcepDatasetBrowserDialog(QxrdExperimentWPtr expt, QcepDatasetModelPtr ds, QWidget *parent = 0);
  virtual ~QcepDatasetBrowserDialog();

private slots:
  void onCustomContextMenuRequested(QPoint pt);
  void onDoubleClicked(QModelIndex idx);

  void newGroup(const QModelIndex &idx);
  void newDataColumn(const QModelIndex &idx);
  void newColumnScan(const QModelIndex &idx);
  void newImage(const QModelIndex &idx);
  void newArray(const QModelIndex &idx);
  void readData(const QModelIndex &idx);
  void saveData(const QModelIndex &idx);
  void openGraph(const QModelIndex &idx);
  void openSpreadsheet(const QModelIndex &idx);
  void openProperties(const QModelIndex &idx);
  void deleteData(const QModelIndex &idx);

private:
  QxrdExperimentWPtr  m_Experiment;
  QcepDatasetModelPtr m_DatasetModel;
};

#endif // QCEPDATASETBROWSERDIALOG_H
