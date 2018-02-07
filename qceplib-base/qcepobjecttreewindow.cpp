#include "qcepobjecttreewindow.h"
#include "ui_qcepobjecttreewindow.h"
#include "qcepobjecttreemodel.h"
#include "qcepobjectpropertiesmodel.h"

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
}

QcepObjectTreeWindow::~QcepObjectTreeWindow()
{
}
