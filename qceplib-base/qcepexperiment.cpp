#include "qcepexperiment.h"
#include <QFileInfo>
#include <QDir>
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include "qcepsetdatavaluerangecommand.h"
#include "qcepsetrangeofimagecommand.h"
#include "qcepfixupgainmapcommand.h"
#include "qcepmainwindowsettings.h"

QcepExperiment::QcepExperiment(QString path, QString name) :
  QcepObject(name),
//  m_SettingsSaver(new QcepSettingsSaver(this)),
  m_ExperimentKind        (this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory   (this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName    (this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName        (this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription (this, "experimentDescription", "", "Experiment Description"),
  m_NewGroupName          (this, "newGroupName", "group", "Default name of newly created data group"),
  m_NewScanName           (this, "newScanName", "scan", "Default name of newly created scans"),
  m_NewScanColumns        (this, "newScanColumns", QStringList(), "Column names for new scan"),
  m_NewScanSize           (this, "newScanSize", 0, "Number of points in new scan"),
  m_NewArrayName          (this, "newArrayName", "array", "Default name of newly created array"),
  m_NewArrayNumDimensions (this, "newArrayNumDimensions", 2, "Dimensionality of newly created array"),
  m_NewArrayXDimension    (this, "newArrayXDimension", 2048, "X Dimension of new array"),
  m_NewArrayYDimension    (this, "newArrayYDimension", 2048, "Y Dimension of new array"),
  m_NewArrayZDimension    (this, "newArrayZDimension", 1, "Z Dimension of new array"),
  m_NewArrayTDimension    (this, "newArrayTDimension", 1, "T Dimension of new array"),
  m_NewColumnName         (this, "newColumnName", "column", "Default name of newly created column"),
  m_NewColumnSize         (this, "newColumnSize", 1024, "Size of newly created column"),
  m_NewImageName          (this, "newImageName", "image", "Name of newly created image"),
  m_NewImageWidth         (this, "newImageWidth", 2048, "Width of new image"),
  m_NewImageHeight        (this, "newImageHeight", 2048, "Height of new image")
{
}

void QcepExperiment::initialize()
{
  addChildPtr(QcepDataExportParametersPtr(
                new QcepDataExportParameters("exportParameters")));

  addChildPtr(QcepDataImportParametersPtr(
                new QcepDataImportParameters("importParameters")));

  addChildPtr(QcepSetDataValueRangeCommandPtr(
                new QcepSetDataValueRangeCommand()));

  addChildPtr(QcepSetRangeOfImageCommandPtr(
                new QcepSetRangeOfImageCommand()));

  addChildPtr(QcepFixupGainMapCommandPtr(
                new QcepFixupGainMapCommand()));
}

void QcepExperiment::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QcepDataExportParameters>(child, m_DataExportParameters)) {}
  else if (checkPointer<QcepDataImportParameters>(child, m_DataImportParameters)) {}
  else if (checkPointer<QcepSetDataValueRangeCommand>(child, m_SetDataValueRangeCommand)) {}
  else if (checkPointer<QcepSetRangeOfImageCommand>(child, m_SetRangeOfImageCommand)) {}
  else if (checkPointer<QcepFixupGainMapCommand>(child, m_FixupGainMapCommand)) {}
}

QString QcepExperiment::defaultExperimentDirectory(QString path) const
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QcepExperiment::defaultExperimentFileName(QString path) const
{
  QFileInfo info(path);
  QString baseName = info.completeBaseName();
//  QString suffix   = info.suffix();

  if (baseName.length() == 0) {
    baseName = "Untitled";
  }
//  if (suffix == "qxrdp") {
//    return info.fileName();
//  } else {
//    return info.fileName()+".qxrdp";
//  }

  return baseName + ".qxrdp";
}

QString QcepExperiment::defaultExperimentName(QString path) const
{
  QFileInfo info(path);
  QString baseName = info.completeBaseName();
//  QString suffix   = info.suffix();

  if (baseName.length() == 0) {
    baseName = "Untitled";
  }

//  if (info.suffix() == "qxrdp") {
//    return info.completeBaseName();
//  } else {
//    return info.fileName();
//  }

  return baseName;
}

QUndoStack *QcepExperiment::undoStack()
{
  return &m_UndoStack;
}

QcepDataExportParametersWPtr QcepExperiment::dataExportParameters()
{
  return m_DataExportParameters;
}

QcepDataImportParametersWPtr QcepExperiment::dataImportParameters()
{
  return m_DataImportParameters;
}

QcepSetDataValueRangeCommandWPtr QcepExperiment::setDataValueRangeCommand()
{
  return m_SetDataValueRangeCommand;
}

QcepSetRangeOfImageCommandWPtr QcepExperiment::setRangeOfImageCommand()
{
  return m_SetRangeOfImageCommand;
}

QcepFixupGainMapCommandWPtr QcepExperiment::fixupGainMapCommand()
{
  return m_FixupGainMapCommand;
}

void QcepExperiment::readSettings(QSettings *settings)
{
  if (settings) {
    QcepObject::readSettings(settings);

    if (m_DataExportParameters) {
      settings->beginGroup("exportParameters");
      m_DataExportParameters -> readSettings(settings);
      settings->endGroup();
    }

    if (m_DataImportParameters) {
      settings->beginGroup("importParameters");
      m_DataImportParameters -> readSettings(settings);
      settings->endGroup();
    }

    if (m_SetDataValueRangeCommand) {
      settings->beginGroup("setDataValueRange");
      m_SetDataValueRangeCommand -> readSettings(settings);
      settings->endGroup();
    }

    if (m_SetRangeOfImageCommand) {
      settings->beginGroup("setRangeOfImage");
      m_SetRangeOfImageCommand -> readSettings(settings);
      settings->endGroup();
    }

    if (m_FixupGainMapCommand) {
      settings->beginGroup("fixupGainMap");
      m_FixupGainMapCommand -> readSettings(settings);
      settings->endGroup();
    }

    int n = settings->beginReadArray("windowSettings");

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      QcepObjectPtr obj = QcepObject::readObject(settings);

      addChildPtr(obj);

      QcepMainWindowSettingsPtr set =
          qSharedPointerDynamicCast<QcepMainWindowSettings>(obj);

      if (set) {
        m_WindowSettings.append(set);
      }
    }

    settings->endArray();
  }
}

void QcepExperiment::writeSettings(QSettings *settings)
{
  if (settings) {
    QcepObject::writeSettings(settings);

    if (m_DataExportParameters) {
      settings->beginGroup("exportParameters");
      m_DataExportParameters -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_DataImportParameters) {
      settings->beginGroup("importParameters");
      m_DataImportParameters -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_SetDataValueRangeCommand) {
      settings->beginGroup("setDataValueRange");
      m_SetDataValueRangeCommand -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_SetRangeOfImageCommand) {
      settings->beginGroup("setRangeOfImage");
      m_SetRangeOfImageCommand -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_FixupGainMapCommand) {
      settings->beginGroup("fixupGainMap");
      m_FixupGainMapCommand -> writeSettings(settings);
      settings->endGroup();
    }

    settings->beginWriteArray("windowSettings");

    for (int i=0; i<m_WindowSettings.count(); i++) {
      settings->setArrayIndex(i);
      QcepMainWindowSettingsPtr set = windowSettings(i);

      if (set) {
        set->writeSettings(settings);
      }
    }

    settings->endArray();
  }
}

int QcepExperiment::windowSettingsCount()
{
  return m_WindowSettings.count();
}

QcepMainWindowSettingsPtr QcepExperiment::windowSettings(int n)
{
  return m_WindowSettings.value(n);
}

void QcepExperiment::appendWindowSettings(QcepMainWindowSettingsPtr settings)
{
  if (settings) {
    for (int i=0; i<windowSettingsCount(); i++) {
      QcepMainWindowSettingsPtr set = windowSettings(i);

      if (set && set->className() == settings->className()) {
        return;
      }
    }

    m_WindowSettings.append(settings);
  }
}