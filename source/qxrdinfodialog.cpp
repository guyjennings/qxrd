#include "qxrdinfodialog.h"
#include "ui_qxrdinfodialog.h"
#include "qcepproperty.h"
#include "qxrdsettingssaver.h"
#include "qxrdimagedata.h"

QxrdInfoDialog::QxrdInfoDialog(QxrdInfoDialogSettingsWPtr settings, QWidget *parent) :
  QDockWidget(parent),
  m_InfoDialogSettings(settings)
{
  setupUi(this);
}

void QxrdInfoDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow)
{
  m_InfoText->clear();

  if (image) {
    image->calculateRange();

    m_InfoText->append(tr("Information about %1").arg(image->get_Title()));
    m_InfoText->append(tr("Saved in %1").arg(image->get_FileName()));
    m_InfoText->append(tr("User Comment 1 %1").arg(image->get_UserComment1()));
    m_InfoText->append(tr("User Comment 2 %1").arg(image->get_UserComment2()));
    m_InfoText->append(tr("User Comment 3 %1").arg(image->get_UserComment3()));
    m_InfoText->append(tr("User Comment 4 %1").arg(image->get_UserComment4()));
    m_InfoText->append(tr("Acquired at %1").arg(image->get_DateString()));
    m_InfoText->append(tr("Acquired with QXRD Version %1").arg(image->get_QxrdVersion()));
    m_InfoText->append(tr("Acquired with Qt Version %1").arg(image->get_QtVersion()));
    m_InfoText->append(tr("Data type %1 (%2)").arg(image->get_DataType()).arg(image->get_DataTypeName()));
    m_InfoText->append(tr("Dimensions %1 x %2").arg(image->get_Width()).arg(image->get_Height()));
    m_InfoText->append(tr("Minimum Value %1, Maximum Value %2").arg(image->minValue()).arg(image->maxValue()));

    m_InfoText->append(tr("Detector gain setting %1").arg(image->get_CameraGain()));
    m_InfoText->append(tr("Exposure Time %1 sec").arg(image->get_ExposureTime()));
    m_InfoText->append(tr("Summed Exposures %1").arg(image->get_SummedExposures()));
    m_InfoText->append(tr("Image Number %1").arg(image->get_ImageNumber()));
    m_InfoText->append(tr("Image Triggered? %1").arg(image->get_Triggered()));
    m_InfoText->append(tr("Phase %1 of [0..%2]").arg(image->get_PhaseNumber()).arg(image->get_NPhases()-1));
  }
}
