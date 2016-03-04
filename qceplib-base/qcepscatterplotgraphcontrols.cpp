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
}

QcepScatterPlotGraphControls::~QcepScatterPlotGraphControls()
{
}

//void QcepScatterPlotGraphControls::onSelectionChanged(
//    const QItemSelection &selected, const QItemSelection &deselected)
//{
//  QcepExperimentPtr exp(m_Window->experiment());

//  if (exp) {
//    exp->printMessage(tr("QcepScatterPlotGraphControls::onSelectionChanged: %1 selected, %2 deselected")
//                      .arg(selected.count()).arg(deselected.count()));

//    for (int i=0; i<selected.count(); i++) {
//      QItemSelectionRange s = selected.value(i);

//      exp->printMessage(tr("Selected [%1,%2]-[%3,%4]\n")
//             .arg(s.left()).arg(s.top()).arg(s.right()).arg(s.bottom()));
//    }

//    for (int i=0; i<deselected.count(); i++) {
//      QItemSelectionRange s = deselected.value(i);

//      exp->printMessage(tr("Deselected [%1,%2]-[%3,%4]\n")
//             .arg(s.left()).arg(s.top()).arg(s.right()).arg(s.bottom()));
//    }
//  }
//}

void QcepScatterPlotGraphControls::onClicked(const QModelIndex &index)
{
  QcepExperimentPtr exp(m_Window->experiment());

  if (exp) {
    exp->printMessage(tr("QcepScatterPlotGraphControls::onClicked(%1,%2)").arg(index.column()).arg(index.row()));
  }

  m_Model -> toggle(index);
}
