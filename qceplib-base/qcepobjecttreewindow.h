#ifndef QCEPOBJECTTREEWINDOW_H
#define QCEPOBJECTTREEWINDOW_H

#include "qceplib_global.h"
#include <QMainWindow>
#include "ui_qcepobjecttreewindow.h"
#include "qcepobjecttreemodel.h"
#include "qcepobjectpropertiesmodel.h"
#include "qcepobject-ptr.h"

class QCEP_EXPORT QcepObjectTreeWindow : public QMainWindow, public Ui::QcepObjectTreeWindow
{
  Q_OBJECT

public:
  explicit QcepObjectTreeWindow(QWidget *parent, QcepObjectWPtr obj);
  ~QcepObjectTreeWindow();

private:
  QcepObjectTreeModel       *m_ObjectTreeModel;
  QcepObjectPropertiesModel *m_ObjectPropertiesModel;
};

#endif // QCEPOBJECTTREEWINDOW_H
