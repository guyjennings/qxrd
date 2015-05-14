#include "qxrdcalibrant.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdsettingssaver.h"

QxrdCalibrant::QxrdCalibrant(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCalibrantLibraryWPtr lib)
  : QcepObject("calibrant", NULL),
    m_Description(saver, this, "description", "Description", "Calibrant Description"),
    m_Flags(saver, this, "flags", 0, "Calibrant flags"),
    m_Symmetry(saver, this, "symmetry", 0, "Calibrant Symmetry"),
    m_A(saver, this, "a", 1, "calibrant a-axis"),
    m_B(saver, this, "b", 1, "calibrant b-axis"),
    m_C(saver, this, "c", 1, "calibrant c-axis"),
    m_Alpha(saver, this, "alpha", 90, "calibrant alpha angle (deg)"),
    m_Beta(saver, this, "beta", 90, "calibrant beta angle (deg)"),
    m_Gamma(saver, this, "gamma", 90, "calibrant gamma angle (deg)"),
    m_Experiment(exp),
    m_CalibrantLibrary(lib)
{

}

QxrdCalibrant::~QxrdCalibrant()
{

}

QScriptValue QxrdCalibrant::toScriptValue(QScriptEngine *engine, const QxrdCalibrantWPtr &cal)
{
  return engine->newQObject(cal.data());
}

void QxrdCalibrant::fromScriptValue(const QScriptValue &obj, QxrdCalibrantWPtr &cal)
{
}
