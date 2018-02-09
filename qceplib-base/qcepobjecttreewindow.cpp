#include "qcepobjecttreewindow.h"
#include "ui_qcepobjecttreewindow.h"
#include "qcepobjecttreemodel.h"
#include "qcepobjectpropertiesmodel.h"
#include <QItemSelection>

QcepObjectTreeWindow::QcepObjectTreeWindow(QWidget *parent, QcepObjectWPtr obj) :
  QMainWindow(parent)
{
  setupUi(this);

  m_ObjectTreeModel =
      new QcepObjectTreeModel(this, obj);

  m_ObjectPropertiesModel =
      new QcepObjectPropertiesModel(this, obj);

  m_ObjectTree -> setModel(m_ObjectTreeModel);

  m_ObjectProperties -> setModel(m_ObjectPropertiesModel);

  m_ObjectSelection =
      m_ObjectTree -> selectionModel();

  connect(m_ObjectSelection, &QItemSelectionModel::selectionChanged,
          this, &QcepObjectTreeWindow::selectionChanged);
}

QcepObjectTreeWindow::~QcepObjectTreeWindow()
{
}

void QcepObjectTreeWindow::selectionChanged(const QItemSelection &selected,
                                            const QItemSelection &deselected)
{
  QModelIndexList sel = selected.indexes();

  if (sel.count() == 0) {
    m_ObjectPropertiesModel -> deselect();
  } else {
    QModelIndex ind = sel.first();

    QcepObjectWPtr obj =
        m_ObjectTreeModel -> indexedObject(ind);

    m_ObjectPropertiesModel -> select(obj);
  }
}
