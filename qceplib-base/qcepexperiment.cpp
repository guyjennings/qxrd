#include "qcepexperiment.h"
#include "qcepsettingssaver.h"
#include <QFileInfo>
#include <QDir>
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"

QcepExperiment::QcepExperiment(QString path, QString name) :
  QcepSerializableObject(name),
  m_SettingsSaver(new QcepSettingsSaver(this)),
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

void QcepExperiment::initialize(QSettings *settings)
{
  m_DataExportParameters = QcepDataExportParametersPtr(
        new QcepDataExportParameters("exportParameters"));

  addChildPtr(m_DataExportParameters);

  m_DataImportParameters = QcepDataImportParametersPtr(
        new QcepDataImportParameters("importParameters"));

  addChildPtr(m_DataImportParameters);
}

void QcepExperiment::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  if (checkPointer<QcepDataExportParameters>(child, m_DataExportParameters)) {}
  else if (checkPointer<QcepDataImportParameters>(child, m_DataImportParameters)) {}
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

void QcepExperiment::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepObject::readSettings(settings, section);

    if (m_DataExportParameters) {
      m_DataExportParameters -> readSettings(settings, "exportParameters");
    }

    if (m_DataImportParameters) {
      m_DataImportParameters -> readSettings(settings, "importParameters");
    }
  }
}

void QcepExperiment::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepObject::writeSettings(settings, section);

    if (m_DataExportParameters) {
      m_DataExportParameters -> writeSettings(settings, "exportParameters");
    }

    if (m_DataImportParameters) {
      m_DataImportParameters -> writeSettings(settings, "importParameters");
    }
  }
}
