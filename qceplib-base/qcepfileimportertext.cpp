#include "qcepfileimportertext.h"
#include <QTextStream>
#include "qcepdatasetmodel.h"
#include "qcepallocator.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"

QcepFileImporterText::QcepFileImporterText(QcepDatasetModelPtr model,
                                           QModelIndexList &indexes,
                                           QString path) :
  QcepFileImporter(model, indexes, path),
  m_ScanLine("#S\\s*(\\d+)\\s(.*)"),
  m_ColumnCountLine("#N\\s*(\\d+)(.*)"),
  m_ColumnNamesLine("#L\\s+(.*)")
{
}

void QcepFileImporterText::exec()
{
  QFile aFile(m_Path);

  if (aFile.open(QFile::ReadOnly)) {
    emit importStarted();

    QTextStream inp(&aFile);
    QString aLine;

    double fsiz = (double) aFile.size();
    double pct  = 0;

    do {
      double npct = ((double) aFile.pos()) / fsiz * 100.0;

      if (npct > pct + 5.0) {
        emit importProgress(npct);
        pct = npct;
      }

      aLine = inp.readLine();
      processLine(aLine);
    } while (!aLine.isNull());

    emit importCompleted();
  }
}

void QcepFileImporterText::processLine(QString aLine)
{
  if (aLine.startsWith("#F")) {
    processFile(aLine);
  } else if (aLine.startsWith("#E")) {
    processEpoch(aLine);
  } else if (aLine.startsWith("#S")) {
    processScan(aLine);
  } else if (aLine.startsWith("#D")) {
    processDate(aLine);
  } else if (aLine.startsWith("#N")) {
    processColumnCount(aLine);
  } else if (aLine.startsWith("#L")) {
    processColumnNames(aLine);
  } else if (aLine.startsWith("#T")) {
    processCountTime(aLine);
  } else if (aLine.startsWith("#C")) {
    processComment(aLine);
  } else if (aLine.startsWith("#")) {
    processHeaderLine(aLine);
  } else {
    processDataLine(aLine);
  }
}

void QcepFileImporterText::processFile(QString aLine)
{
}

void QcepFileImporterText::processEpoch(QString aLine)
{
}

void QcepFileImporterText::processScan(QString aLine)
{
  if (m_ScanLine.exactMatch(aLine)) {
    int     scanNum = m_ScanLine.cap(1).toInt();
    QString scanCmd = m_ScanLine.cap(2);
    QString name    = tr("scan_%1").arg(scanNum);

    m_LatestScan = QcepAllocator::newColumnScan(name, QStringList(), 0, QcepAllocator::NullIfNotAvailable);

    if (m_LatestScan) {
      m_LatestScan->set_Description(scanCmd);

      m_Model->append(QModelIndex(), m_LatestScan);
    }

    m_RowCount = 0;
    m_ColumnCount = 0;
  }
}

void QcepFileImporterText::processDate(QString aLine)
{
}

void QcepFileImporterText::processColumnCount(QString aLine)
{
  if (m_ColumnCountLine.exactMatch(aLine)) {
    int     ncols = m_ColumnCountLine.cap(1).toInt();

    m_ColumnCount = ncols;
  }
}

void QcepFileImporterText::processColumnNames(QString aLine)
{
  if (m_ColumnNamesLine.exactMatch(aLine)) {

    QStringList names = m_ColumnNamesLine.cap(1).split("  ");

    if (m_LatestScan) {
      foreach (QString col, names) {
        m_LatestScan->appendColumn(col);
      }
    }
  }
}

void QcepFileImporterText::processCountTime(QString aLine)
{
}

void QcepFileImporterText::processComment(QString aLine)
{
}

void QcepFileImporterText::processHeaderLine(QString aLine)
{
}

void QcepFileImporterText::processDataLine(QString aLine)
{
  QRegExp splitWs("\\s+");

  QStringList nums = aLine.split(splitWs, QString::SkipEmptyParts);

  if (m_LatestScan && (nums.count() >= 1)) {
    m_LatestScan->resizeRows(m_RowCount+1);

    for (int i=0; i<m_ColumnCount; i++) {
      double val = nums.value(i).toDouble();
      m_LatestScan->setValue(i, m_RowCount, val);
    }

    m_RowCount++;
  }
}
