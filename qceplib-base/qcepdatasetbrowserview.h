#ifndef QCEPDATASETBROWSERVIEW_H
#define QCEPDATASETBROWSERVIEW_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include <QTreeView>
#include "qcepdatasetmodel-ptr.h"
#include "qcepexperiment-ptr.h"

class QCEP_EXPORT QcepDatasetBrowserView : public QTreeView
{
  Q_OBJECT

private:
  typedef QTreeView inherited;

public:
  QcepDatasetBrowserView(QWidget *parent = 0);

  void setExperiment(QcepExperimentWPtr expt);
  void setDatasetModel(QcepDatasetModelWPtr model);

private slots:
  void onCustomContextMenuRequested(QPoint pt);
  void onDoubleClicked(QModelIndex idx);

  void newGroup(const QModelIndexList &idx);
  void newDataColumn(const QModelIndexList &idx);
  void newColumnScan(const QModelIndexList &idx);
  void newImage(const QModelIndexList &idx);
  void newArray(const QModelIndexList &idx);
  void concatenateData(const QModelIndex &idx);
  void duplicateData(const QModelIndexList &idx);
  void differentiateH(const QModelIndexList &idx);
  void differentiateV(const QModelIndexList &idx);
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
  void setRangeOfImage(const QModelIndexList &idx);
  void setDataValueRange(const QModelIndexList &idx);
  void fixupGainMap(const QModelIndexList &idx);
  void fixupDetectorPanelGains(const QModelIndex &idx);
  void scanDetectorPanelGains(const QModelIndex &idx);
  void vEdges(const QModelIndex &idx);
  void hEdges(const QModelIndex &idx);
  void readData(const QModelIndexList &idx);
  void saveData(const QModelIndexList &idx);
  void plotImage(const QModelIndex &idx);
  void openGraph(const QModelIndexList &idx);

#ifdef HAVE_DATAVIS
  void surfacePlot(const QModelIndexList &idx);
#endif

  void openSpreadsheet(const QModelIndexList &idx);
  void openProperties(const QModelIndexList &idx);
  void deleteData(const QModelIndexList &idx);

private:
  QcepExperimentWPtr   m_Experiment;
  QcepDatasetModelPtr  m_DatasetModel;
};

#endif // QCEPDATASETBROWSERVIEW_H
