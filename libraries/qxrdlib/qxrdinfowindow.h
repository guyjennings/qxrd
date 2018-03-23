#ifndef QXRDINFOWINDOW_H
#define QXRDINFOWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdinfowindow.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepobjecttreemodel.h"
#include "qcepobjectpropertiesmodel.h"

class QXRD_EXPORT QxrdInfoWindow : public QxrdMainWindow, public Ui::QxrdInfoWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdInfoWindow(QString name);
  explicit QxrdInfoWindow(QString name, QcepObjectWPtr rootObject);
  void initialize(QcepObjectWPtr parent);
  ~QxrdInfoWindow();

  void onProcessedImageAvailable(QcepDoubleImageDataPtr image,
                                 QcepMaskDataPtr overflow);

  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected);

protected:
  void changeEvent(QEvent *e);

private:
  QcepObjectWPtr             m_RootObject;
  QcepObjectTreeModel       *m_ObjectTreeModel;
  QcepObjectPropertiesModel *m_ObjectPropertiesModel;
  QItemSelectionModel       *m_ObjectSelection;
};

#endif // QXRDINFOWINDOW_H
