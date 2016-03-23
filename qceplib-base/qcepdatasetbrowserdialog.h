#ifndef QCEPDATASETBROWSERDIALOG_H
#define QCEPDATASETBROWSERDIALOG_H

#include <QDockWidget>
#include "ui_qcepdatasetbrowserdialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"

class QcepDatasetBrowserDialog : public QDockWidget, public Ui::QcepDatasetBrowserDialog
{
  Q_OBJECT

public:
  explicit QcepDatasetBrowserDialog(QcepExperimentWPtr expt, QcepDatasetModelPtr ds, QWidget *parent = 0);
  virtual ~QcepDatasetBrowserDialog();

private slots:
  void onCustomContextMenuRequested(QPoint pt);
  void onDoubleClicked(QModelIndex idx);

  void newGroup(const QModelIndex &idx);
  void newDataColumn(const QModelIndex &idx);
  void newColumnScan(const QModelIndex &idx);
  void newImage(const QModelIndex &idx);
  void newArray(const QModelIndex &idx);
  void concatenateData(const QModelIndex &idx);
  void addData(const QModelIndex &idx);
  void subtractData(const QModelIndex &idx);
  void multiplyData(const QModelIndex &idx);
  void divideData(const QModelIndex &idx);
  void integrateData(const QModelIndexList &idx);
  void integrateParameters(const QModelIndexList &idx);
  void polarTransformData(const QModelIndexList &idx);
  void polarTransformParameters(const QModelIndexList &idx);
  void polarIntegrateData(const QModelIndexList &idx);
  void polarIntegrateParameters(const QModelIndexList &idx);
  void readData(const QModelIndex &idx);
  void saveData(const QModelIndex &idx);
  void openGraph(const QModelIndexList &idx);
  void openSpreadsheet(const QModelIndexList &idx);
  void openProperties(const QModelIndexList &idx);
  void deleteData(const QModelIndexList &idx);

private:
  QcepExperimentWPtr  m_Experiment;
  QcepDatasetModelPtr m_DatasetModel;
};

#endif // QCEPDATASETBROWSERDIALOG_H
