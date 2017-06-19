#include "qcepscatterplotgraphcontrols.h"
#include "ui_qcepscatterplotgraphcontrols.h"
#include "qcepscatterplotgraphmodel.h"
#include <stdio.h>
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
          m_Model.data(),   &QcepScatterPlotGraphModel::toggle);

//  connect(m_Model.data(), &QAbstractItemModel::dataChanged,
//          this,           &QcepScatterPlotGraphControls::onPlotDataChanged);
}

QcepScatterPlotGraphControls::~QcepScatterPlotGraphControls()
{
}

void QcepScatterPlotGraphControls::onClicked(const QModelIndex &index)
{
  m_Model -> toggle(index);
}

QcepScatterPlotGraphModelPtr QcepScatterPlotGraphControls::model()
{
  return m_Model;
}
