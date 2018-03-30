#ifndef QXRDALLIEDVISIONPLUGIN_H
#define QXRDALLIEDVISIONPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugin.h"
#include "VimbaCPP/Include/VimbaCPP.h"

class QxrdAlliedVisionPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "alliedvision.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

public:
    QxrdAlliedVisionPlugin();

    void registerMetaTypes();

    void initialize(QcepObjectWPtr parent);
    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq);

private:
    AVT::VmbAPI::VimbaSystem& m_Vimba;
};

Q_DECLARE_METATYPE(QxrdAlliedVisionPlugin*)

#endif // QXRDALLIEDVISIONPLUGIN_H
