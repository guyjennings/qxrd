#include "qxrdpolarnormalization.h"

QxrdPolarNormalization::QxrdPolarNormalization(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp) :
  QcepObject("polarNormalization", NULL),
  m_Source(saver, this, "source", "Polar/image", "Source for normalization operation"),
  m_Destination(saver, this, "destination", "Polar/normed", "Destination for normalization operation"),
  m_Saver(saver),
  m_Experiment(exp)
{
}

void QxrdPolarNormalization::execute()
{
}
