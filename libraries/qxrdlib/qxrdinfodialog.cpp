#include "qxrdinfodialog.h"
#include "ui_qxrdinfodialog.h"
#include "qcepproperty.h"
#include "qcepimagedata.h"
#include "qxrddebug.h"
#include "qxrdinfodialogsettings.h"

QxrdInfoDialog::QxrdInfoDialog(QxrdInfoDialogSettingsWPtr settings, QWidget *parent) :
  QDockWidget(parent),
  m_InfoDialogSettings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdInfoDialog::QxrdInfoDialog(%p)\n", this);
  }

  setupUi(this);
}

QxrdInfoDialog::~QxrdInfoDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdInfoDialog::~QxrdInfoDialog(%p)\n", this);
  }
}

void QxrdInfoDialog::onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr /*overflow*/)
{
  m_InfoText->clear();

  if (image) {
//    image->calculateRange();

    m_InfoText->append(tr("Information about %1").arg(image->get_Name()));
    m_InfoText->append(tr("Saved in %1").arg(image->get_FileName()));
    m_InfoText->append(tr("User Comment 1 %1").arg(image->get_UserComment1()));
    m_InfoText->append(tr("User Comment 2 %1").arg(image->get_UserComment2()));
    m_InfoText->append(tr("User Comment 3 %1").arg(image->get_UserComment3()));
    m_InfoText->append(tr("User Comment 4 %1").arg(image->get_UserComment4()));
    m_InfoText->append(tr("Acquired at %1").arg(image->get_DateString()));
    m_InfoText->append(tr("Acquired with %1 Version %2").arg(image->get_Creator()).arg(image->get_Version()));
    m_InfoText->append(tr("Acquired with Qt Version %1").arg(image->get_QtVersion()));
    m_InfoText->append(tr("Data type %1 (%2)").arg(image->get_DataType()).arg(image->get_DataTypeName()));
    m_InfoText->append(tr("Dimensions %1 x %2").arg(image->get_Width()).arg(image->get_Height()));

    m_InfoText->append(tr("H: %1 (%2) x V: %3 (%4")
                       .arg(image->get_HLabel()).arg(image->get_HUnits())
                       .arg(image->get_VLabel()).arg(image->get_VUnits()));

    m_InfoText->append(tr("H Scaling: Start:%1, Step:%2, V Scaling: Start:%3, Step:%4")
                       .arg(image->get_HStart()).arg(image->get_HStep())
                       .arg(image->get_VStart()).arg(image->get_VStep()));

    m_InfoText->append(tr("Minimum Value %1, Maximum Value %2").arg(image->get_MinimumValue()).arg(image->get_MaximumValue()));

    m_InfoText->append(tr("Detector gain setting %1").arg(image->get_CameraGain()));
    m_InfoText->append(tr("Exposure Time %1 sec").arg(image->get_ExposureTime()));
    m_InfoText->append(tr("Summed Exposures %1").arg(image->get_SummedExposures()));
    m_InfoText->append(tr("Image Sequence Number %1").arg(image->get_ImageSequenceNumber()));
    m_InfoText->append(tr("Image Number %1").arg(image->get_ImageNumber()));
    m_InfoText->append(tr("Image Triggered? %1").arg(image->get_Triggered()));
    m_InfoText->append(tr("Phase %1 of [0..%2]").arg(image->get_PhaseNumber()).arg(image->get_NPhases()-1));

    m_InfoText->append(tr("Normalization Data Size = %1").arg(image->get_Normalization().count()));

    for (int i=0; i<image->get_Normalization().count(); i++) {
      m_InfoText->append(tr("Normalization Data [%1] = %2").arg(i).arg(image->get_Normalization().value(i)));
    }

    m_InfoText->append(tr("Extra Inputs Data Size = %1").arg(image->get_ExtraInputs().count()));

    for (int i=0; i<image->get_ExtraInputs().count(); i++) {
      m_InfoText->append(tr("Extra Inputs Data [%1] = %2").arg(i).arg(image->get_ExtraInputs().value(i)));
    }

    QList<QByteArray> dynProps = image->dynamicPropertyNames();

    if (dynProps.count()) {
      m_InfoText->append("User Defined Properties:");

      foreach(QByteArray prop, dynProps) {
        m_InfoText->append(tr("%1 = %2").arg(QString(prop)).arg(image->property(prop).toString()));
      }
    }
  }
}
