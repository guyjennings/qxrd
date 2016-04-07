#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include <QScriptEngine>

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QcepObject *parent) :
  QcepDataGroup(sav, name, parent),
  m_NumPoints(sav, this, "numPoints", 0, "Number of points in scan")
{
  set_Type("Data Column Scan");
}

QcepDataColumnScanPtr QcepDataColumnScan::newDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts, QcepObject *parent)
{
  QcepDataColumnScanPtr res(new QcepDataColumnScan(sav, name, parent));

  foreach (QString col, cols) {
    res -> append(QcepDataColumn::newDataColumn(sav, col, npts, parent));
  }

  res -> set_NumPoints(npts);

  return res;
}

QString QcepDataColumnScan::description() const
{
  return tr("%1 rows x %2 cols").arg(rowCount()).arg(columnCount());
}

QScriptValue QcepDataColumnScan::toColumnScanScriptValue(QScriptEngine *engine, const QcepDataColumnScanPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataColumnScan::fromColumnScanScriptValue(const QScriptValue &obj, QcepDataColumnScanPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataColumnScan *qdobj = qobject_cast<QcepDataColumnScan*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataColumnScanPtr cs = qSharedPointerDynamicCast<QcepDataColumnScan>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

QcepDataColumnPtr QcepDataColumnScan::column(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataColumn>(obj);
}

int QcepDataColumnScan::columnCount() const
{
  return childCount();
}

int QcepDataColumnScan::rowCount() const
{
  return get_NumPoints();
}

QcepDataColumnPtr QcepDataColumnScan::appendColumn(QString title)
{
  QcepDataColumnPtr col = QcepDataColumn::newDataColumn(saver(), title, 0, this);

  append(col);

  return col;
}

void QcepDataColumnScan::resizeRows(int nRows)
{
  int nc = columnCount();

  for (int i=0; i<nc; i++) {
    QcepDataColumnPtr c = column(i);

    if (c) {
      c -> resize(nRows);
    }
  }

  set_NumPoints(nRows);
}

void QcepDataColumnScan::setValue(int col, int row, double val)
{
  QcepDataColumnPtr c = column(col);

  if (c) {
    c -> setValue(row, val);
  }
}

double QcepDataColumnScan::value(int col, int row)
{
  QcepDataColumnPtr c = column(col);

  if (c) {
    return c->value(row);
  } else {
    return qQNaN();
  }
}

bool QcepDataColumnScan::checkCompatibility(QcepDataColumnScanPtr scan)
{
  if (scan == NULL) return false;

  int nCols = columnCount();
  int nRows = rowCount();
  int snCols = scan->columnCount();
  int snRows = scan->rowCount();

  if (nCols == 0) { // If 'this' is empty, copy columns from 'scan'
    for (int i=0; i<snCols; i++) {
      appendColumn(scan->column(i)->get_Name());
    }

    resizeRows(snRows);

    for (int i=0; i<snCols; i++) {
      QcepDataColumnPtr c = column(i);
      QcepDataColumnPtr sc = scan->column(i);

      if (c && sc) {
        c->set_ColumnType(sc->get_ColumnType());
        c->set_Column1(sc->get_Column1());
        c->set_Column2(sc->get_Column2());
      }
    }
  } else if (nCols != snCols || nRows != snRows) { // Size mismatch
    printMessage("Scan size mismatch - not compatible");

    return false;
  } else {
    for (int i=0; i<nCols; i++) {
      QcepDataColumnPtr c = column(i);
      QcepDataColumnPtr sc = scan->column(i);

      if (c && sc) {
        if (c->get_Name() != sc->get_Name()) {
          printMessage(tr("Column %1 name mismatch, [%2,%3]")
                       .arg(i).arg(c->get_Name()).arg(sc->get_Name()));
          return false;
        }

        if (c->get_ColumnType() != sc->get_ColumnType()) {
          printMessage(tr("Column %1 type mismatch, [%2,%3]")
                       .arg(i).arg(c->get_ColumnType()).arg(sc->get_ColumnType()));
          return false;
        }

        if (c->get_Column1() != sc->get_Column1()) {
          printMessage(tr("Column %1 1st dependent mismatch, [%2,%3]")
                       .arg(i).arg(c->get_Column1()).arg(sc->get_Column1()));
          return false;
        }

        if (c->get_Column2() != sc->get_Column2()) {
          printMessage(tr("Column %1 2nd dependent mismatch, [%2,%3]")
                       .arg(i).arg(c->get_Column1()).arg(sc->get_Column1()));
          return false;
        }
      } else {
        printMessage("Null column(s) present");
        return false;
      }
    }
  }

  return true;
}

void QcepDataColumnScan::add(QcepDataColumnScanPtr scan)
{
  if (checkCompatibility(scan)) {
    int nCols = columnCount();

    for (int i=0; i<nCols; i++) {
      QcepDataColumnPtr c = column(i);
      QcepDataColumnPtr sc = scan->column(i);

      if (c && sc) {
        switch (c->get_ColumnType()) {
        case QcepDataColumn::NormalColumn:
        case QcepDataColumn::CountsColumn:
        case QcepDataColumn::WeightColumn:
          c->add(sc);
          break;

        case QcepDataColumn::XValueColumn:
          c->copy(sc);
          break;

        case QcepDataColumn::RatioColumn:
          {
            QcepDataColumnPtr col1 = scan->column(c->get_Column1());
            QcepDataColumnPtr col2 = scan->column(c->get_Column2());
            c->copy(col1);
            c->divide(col2);
          }
          break;
        }
      }
    }
  }
}

void QcepDataColumnScan::subtract(QcepDataColumnScanPtr scan)
{
  if (checkCompatibility(scan)) {
    int nCols = columnCount();
    int nRows = rowCount();

    for (int i=0; i<nCols; i++) {
      QcepDataColumnPtr c = column(i);
      QcepDataColumnPtr sc = scan->column(i);

      if (c && sc) {
        switch (c->get_ColumnType()) {
        case QcepDataColumn::NormalColumn:
        case QcepDataColumn::CountsColumn:
        case QcepDataColumn::WeightColumn:
          c->subtract(sc);
          break;

        case QcepDataColumn::XValueColumn:
          c->copy(sc);
          break;

        case QcepDataColumn::RatioColumn:
          {
            QcepDataColumnPtr col1 = scan->column(c->get_Column1());
            QcepDataColumnPtr col2 = scan->column(c->get_Column2());
            c->copy(col1);
            c->divide(col2);
          }
          break;
        }
      }
    }
  }
}

void QcepDataColumnScan::concat(QcepDataColumnScanPtr scan)
{
  if (scan) {
    int nCols = columnCount();
    int snCols = scan->columnCount();
    int snRows = scan->rowCount();

    if (nCols == 0) {
      for (int i=0; i<snCols; i++) {
        appendColumn(scan->column(i)->get_Name());
      }

      resizeRows(snRows);

      for (int i=0; i<snCols; i++) {
        QcepDataColumnPtr c = column(i);
        QcepDataColumnPtr sc = scan->column(i);

        if (c && sc) {
          c->set_ColumnType(sc->get_ColumnType());
          c->set_Column1(sc->get_Column1());
          c->set_Column2(sc->get_Column2());
          c->copy(sc);
        }
      }
    } else if (nCols != snCols) {
      printMessage("Appended scan has different # columns");
    } else {
      for (int i=0; i<nCols; i++) {
        QcepDataColumnPtr c = column(i);
        QcepDataColumnPtr sc = scan->column(i);

        if (c && sc) {
          if (c->get_Name() != sc->get_Name()) {
            printMessage(tr("Column %1 name mismatch, [%2,%3]")
                         .arg(i).arg(c->get_Name()).arg(sc->get_Name()));
            return;
          }

          if (c->get_ColumnType() != sc->get_ColumnType()) {
            printMessage(tr("Column %1 type mismatch, [%2,%3]")
                         .arg(i).arg(c->get_ColumnType()).arg(sc->get_ColumnType()));
            return;
          }

          if (c->get_Column1() != sc->get_Column1()) {
            printMessage(tr("Column %1 1st dependent mismatch, [%2,%3]")
                         .arg(i).arg(c->get_Column1()).arg(sc->get_Column1()));
            return;
          }

          if (c->get_Column2() != sc->get_Column2()) {
            printMessage(tr("Column %1 2nd dependent mismatch, [%2,%3]")
                         .arg(i).arg(c->get_Column1()).arg(sc->get_Column1()));
            return;
          }
        } else {
          printMessage("Null column(s) present");
          return;
        }
      }

      for (int i=0; i<nCols; i++) {
        QcepDataColumnPtr c = column(i);
        QcepDataColumnPtr sc = scan->column(i);

        if (c && sc) {
          c->concat(sc);
        }
      }
    }
  }
}
