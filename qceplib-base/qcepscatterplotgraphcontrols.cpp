#include "qcepscatterplotgraphcontrols.h"
#include "ui_qcepscatterplotgraphcontrols.h"
#include "qcepscatterplotgraphmodel.h"
#include <stdio.h>
#include "qcepexperiment-ptr.h"
#include "qcepexperiment.h"
#include "qcepdataobjectgraphwindow.h"

QcepScatterPlotGraphControls::QcepScatterPlotGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object) :
  QcepGraphControlsWidget(window, mode, object)
{
  setupUi(this);

  m_Model = QcepScatterPlotGraphModelPtr(
        new QcepScatterPlotGraphModel(object));

  m_ScatterColumns -> setModel(m_Model.data());

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  m_ScatterColumns -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);
#else
  m_ScatterColumns -> horizontalHeader() -> setResizeMode(QHeaderView::ResizeToContents);
#endif

//  connect(m_ScatterColumns->selectionModel(), &QItemSelectionModel::selectionChanged,
//          this, &QcepScatterPlotGraphControls::onSelectionChanged);

  connect(m_ScatterColumns, &QAbstractItemView::clicked,
          this, &QcepScatterPlotGraphControls::onClicked);

  connect(m_Model.data(), &QAbstractItemModel::dataChanged,
          this,           &QcepScatterPlotGraphControls::onPlotDataChanged);
}

QcepScatterPlotGraphControls::~QcepScatterPlotGraphControls()
{
}

void QcepScatterPlotGraphControls::onClicked(const QModelIndex &index)
{

  m_Model -> toggle(index);
}

void QcepScatterPlotGraphControls::onPlotDataChanged(
    const QModelIndex &topLeft, const QModelIndex &bottomRight,
    const QVector<int> &roles)
{
  QcepExperimentPtr exp(m_Window->experiment());

  if (exp) {
    exp->printMessage(tr("QcepScatterPlotGraphControls::onPlotDataChanged([%1,%2],[%3,%4])")
                      .arg(topLeft.column()).arg(topLeft.row())
                      .arg(bottomRight.column()).arg(bottomRight.row()));
  }
}
