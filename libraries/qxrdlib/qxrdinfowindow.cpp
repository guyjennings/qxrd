#include "qxrdinfowindow.h"
#include "qxrdappcommon.h"
#include <QThread>

QxrdInfoWindow::QxrdInfoWindow(QString name) :
  inherited(name),
  m_RootObject()
{

}

QxrdInfoWindow::QxrdInfoWindow(QString name, QcepObjectWPtr rootObject) :
  inherited(name),
  m_RootObject(rootObject)
{
}

void QxrdInfoWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  if (m_RootObject == NULL) {
    m_RootObject = QxrdAppCommon::findApplication(parent);
  }

  m_ObjectTreeModel =
      new QcepObjectTreeModel(this, m_RootObject);

  m_ObjectPropertiesModel =
      new QcepObjectPropertiesModel(this, m_RootObject);

  m_ObjectView     -> setModel(m_ObjectTreeModel);
  m_PropertiesView -> setModel(m_ObjectPropertiesModel);

  m_ObjectSelection =
      m_ObjectView -> selectionModel();

  connect(m_ObjectSelection, &QItemSelectionModel::selectionChanged,
          this,              &QxrdInfoWindow::selectionChanged);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  QHeaderView *oh =
    m_ObjectView -> header();

  if (oh) {
    oh -> setSectionResizeMode(0, QHeaderView::ResizeToContents);
  }

  QGridLayout *gl =
      qobject_cast<QGridLayout*>(
        m_GridView->layout());

  if (gl) {
    gl -> setColumnStretch(0, 0);
    gl -> setColumnStretch(1, 1);
  }

  QHeaderView *hv =
    m_PropertiesView -> header();

  if (hv) {
    hv -> setSectionResizeMode(0, QHeaderView::ResizeToContents);
    hv -> setSectionResizeMode(1, QHeaderView::ResizeToContents);
  }
}

QxrdInfoWindow::~QxrdInfoWindow()
{
}

void QxrdInfoWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdInfoWindow::selectionChanged(const QItemSelection &selected,
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

void QxrdInfoWindow::onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr /*overflow*/)
{
//  m_InfoText->clear();

//  if (image) {

//    m_InfoText->append(tr("Information about %1").arg(image->get_Name()));
//    m_InfoText->append(tr("Saved in %1").arg(image->get_FileName()));
//    m_InfoText->append(tr("User Comment 1 %1").arg(image->get_UserComment1()));
//    m_InfoText->append(tr("User Comment 2 %1").arg(image->get_UserComment2()));
//    m_InfoText->append(tr("User Comment 3 %1").arg(image->get_UserComment3()));
//    m_InfoText->append(tr("User Comment 4 %1").arg(image->get_UserComment4()));
//    m_InfoText->append(tr("Acquired at %1").arg(image->get_DateString()));
//    m_InfoText->append(tr("Acquired with %1 Version %2").arg(image->get_Creator()).arg(image->get_Version()));
//    m_InfoText->append(tr("Acquired with Qt Version %1").arg(image->get_QtVersion()));
//    m_InfoText->append(tr("Data type %1 (%2)").arg(image->get_DataType()).arg(image->get_DataTypeName()));
//    m_InfoText->append(tr("Dimensions %1 x %2").arg(image->get_Width()).arg(image->get_Height()));

//    m_InfoText->append(tr("H: %1 (%2) x V: %3 (%4")
//                       .arg(image->get_HLabel()).arg(image->get_HUnits())
//                       .arg(image->get_VLabel()).arg(image->get_VUnits()));

//    m_InfoText->append(tr("H Scaling: Start:%1, Step:%2, V Scaling: Start:%3, Step:%4")
//                       .arg(image->get_HStart()).arg(image->get_HStep())
//                       .arg(image->get_VStart()).arg(image->get_VStep()));

//    m_InfoText->append(tr("Minimum Value %1, Maximum Value %2").arg(image->get_MinimumValue()).arg(image->get_MaximumValue()));

//    m_InfoText->append(tr("Detector gain setting %1").arg(image->get_CameraGain()));
//    m_InfoText->append(tr("Exposure Time %1 sec").arg(image->get_ExposureTime()));
//    m_InfoText->append(tr("Summed Exposures %1").arg(image->get_SummedExposures()));
//    m_InfoText->append(tr("Image Sequence Number %1").arg(image->get_ImageSequenceNumber()));
//    m_InfoText->append(tr("Image Number %1").arg(image->get_ImageNumber()));
//    m_InfoText->append(tr("Image Triggered? %1").arg(image->get_Triggered()));
//    m_InfoText->append(tr("Phase %1 of [0..%2]").arg(image->get_PhaseNumber()).arg(image->get_NPhases()-1));

//    m_InfoText->append(tr("Normalization Data Size = %1").arg(image->get_Normalization().count()));

//    for (int i=0; i<image->get_Normalization().count(); i++) {
//      m_InfoText->append(tr("Normalization Data [%1] = %2").arg(i).arg(image->get_Normalization().value(i)));
//    }

//    m_InfoText->append(tr("Extra Inputs Data Size = %1").arg(image->get_ExtraInputs().count()));

//    for (int i=0; i<image->get_ExtraInputs().count(); i++) {
//      m_InfoText->append(tr("Extra Inputs Data [%1] = %2").arg(i).arg(image->get_ExtraInputs().value(i)));
//    }

//    QList<QByteArray> dynProps = image->dynamicPropertyNames();

//    if (dynProps.count()) {
//      m_InfoText->append("User Defined Properties:");

//      foreach(QByteArray prop, dynProps) {
//        m_InfoText->append(tr("%1 = %2").arg(QString(prop)).arg(image->property(prop).toString()));
//      }
//    }
//  }
}
