#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qcepmainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <QMetaObject>
#include <QApplication>
#include <stdio.h>
#include <QLayout>
#include <QDockWidget>

QcepMainWindow::QcepMainWindow(QWidget *parent) : QMainWindow(parent)
{

}


void QcepMainWindow::possiblyClose()
{
  //   printf("QxrdWindow::possiblyClose()\n");
  if (wantToClose()) {
    close();
  }
}

bool QcepMainWindow::wantToClose()
{
  THREAD_CHECK;

  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window?"),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QcepMainWindow::setFontSize(int fontSize)
{
  if (QThread::currentThread()==thread()) {
    if (fontSize > 0) {
      QFont f = font();

      f.setPointSize(fontSize);

      setFont(f);
    } else {
      QFont f = QApplication::font(this);
      setFont(f);
    }
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "setFontSize",
                                           Qt::QueuedConnection,
                                           Q_ARG(int, fontSize)));
  }
}

void QcepMainWindow::setSpacing(int spacing)
{
  if (QThread::currentThread()==thread()) {
    setObjectSpacing(this, spacing);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "setSpacing",
                                           Qt::QueuedConnection,
                                           Q_ARG(int, spacing)));
  }
}

void QcepMainWindow::setObjectSpacing(QObject *obj, int spacing)
{
  QLayout *ly = qobject_cast<QLayout*>(obj);

  if (ly) {
    ly->setContentsMargins(spacing, spacing, spacing, spacing);

    QGridLayout *gl = qobject_cast<QGridLayout*>(ly);

    if (gl) {
      gl->setHorizontalSpacing(spacing);
      gl->setVerticalSpacing(spacing);
    }
  }
  //  QTableView *tv = qobject_cast<QTableView*>(obj);

  //  if (tv) {
  //    tv->setStyleSheet(tr("QTableWidget::item { padding: %1px }").arg(spacing));
  //  }

  foreach(QObject* chobj, obj->children()) {
    if (chobj) {
      setObjectSpacing(chobj, spacing);
    }
  }
}

void QcepMainWindow::shrinkPanels(int fontSize, int spacing)
{
//  if (QThread::currentThread()==thread()) {
//    shrinkObject(this, fontSize, spacing);
//  } else {
//    INVOKE_CHECK(QMetaObject::invokeMethod(this,
//                                           "shrinkPanels",
//                                           Qt::QueuedConnection,
//                                           Q_ARG(int, fontSize),
//                                           Q_ARG(int, spacing)));
//  }
  setFontSize(fontSize);
  setSpacing(spacing);
}

void QcepMainWindow::shrinkDockWidget(QDockWidget *dw, int fontSize, int spacing)
{
  shrinkObject(dw, fontSize, spacing);
}

void QcepMainWindow::shrinkObject(QObject *obj, int fontSize, int spacing)
{
  if (obj) {
    //    printf("shrinkObject %p[%s]\n", obj, qPrintable(obj->objectName()));

    QWidget *wid = qobject_cast<QWidget*>(obj);

    QVariant fsz = obj->property("defaultFontSize");
    QVariant mrga = obj->property("defaultAMargins");
    QVariant mrgb = obj->property("defaultBMargins");

    if (qcepDebug(DEBUG_WINDOW)) {
      printMessage(tr("QxrdWindow::shrinkObject(\"%1\",%2)").arg(obj->objectName()).HEXARG(obj));
      printMessage(tr("Default Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(fsz.toInt())
        .arg(mrga.toPoint().x()).arg(mrga.toPoint().y())
        .arg(mrgb.toPoint().x()).arg(mrgb.toPoint().y()));
    }

    if (wid) {
      int sz = wid->font().pointSize();

      int left, top, right, bottom;
      wid->getContentsMargins(&left, &top, &right, &bottom);

      if (qcepDebug(DEBUG_WINDOW)) {
        printMessage(tr("Existing Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(sz).arg(left).arg(top).arg(right).arg(bottom));
      }

      if (!fsz.isValid()) {
        wid->setProperty("defaultFontSize", sz);
        fsz = wid->property("defaultFontSize");
      }

      if (!mrga.isValid()) {
        wid->setProperty("defaultAMargins", QPoint(left, top));
        mrga = wid->property("defaultAMargins");
      }

      if (!mrgb.isValid()) {
        wid->setProperty("defaultBMargins", QPoint(right, bottom));
        mrgb = wid->property("defaultBMargins");
      }

      if (fontSize >= 0) {
        QFont f = wid->font();
        f.setPointSize(fontSize);
        wid->setFont(f);
      } else {
        QFont f = wid->font();
        f.setPointSize(fsz.toInt());
        wid->setFont(f);
      }

      if (spacing >= 0) {
        wid->setContentsMargins(spacing, spacing, spacing, spacing);
      } else {
        QPoint ma = mrga.toPoint();
        QPoint mb = mrgb.toPoint();

        if (!ma.isNull() && !mb.isNull()) {
          wid->setContentsMargins(ma.x(), ma.y(), mb.x(), mb.y());
        }
      }
    }

    if (qcepDebug(DEBUG_WINDOW)) {
      printMessage(tr("Default Font Size %1, Margins A [%2,%3], B [%4,%5]").arg(fsz.toInt())
        .arg(mrga.toPoint().x()).arg(mrga.toPoint().y())
        .arg(mrgb.toPoint().x()).arg(mrgb.toPoint().y()));
    }

    QLayout *ly = qobject_cast<QLayout*>(obj);

    if (ly) {
      if (spacing >= 0) {
        ly->setContentsMargins(spacing, spacing, spacing, spacing);
      } else {
        QPoint ma = mrga.toPoint();
        QPoint mb = mrgb.toPoint();

        if (!ma.isNull() && !mb.isNull()) {
          ly->setContentsMargins(ma.x(), ma.y(), mb.x(), mb.y());
        }
      }

      QGridLayout *gl = qobject_cast<QGridLayout*>(ly);

      if (gl) {
//        gl->setContentsMargins(spacing, spacing, spacing, spacing);
        QVariant spc = gl->property("defaultSpacing");

        if (qcepDebug(DEBUG_WINDOW)) {
          printMessage(tr("Default spacing [%1,%2]").arg(spc.toPoint().x()).arg(spc.toPoint().y()));
        }

        if (!spc.isValid()) {
          int hs = gl->horizontalSpacing();
          int vs = gl->verticalSpacing();

          if (qcepDebug(DEBUG_WINDOW)) {
            printMessage(tr("Existing spacing [%1,%2]").arg(hs).arg(vs));
          }

          gl->setProperty("defaultSpacing", QPoint(hs,vs));
          spc = gl->property("defaultSpacing");
        }

        if (spacing >= 0) {
          gl->setHorizontalSpacing(spacing);
          gl->setVerticalSpacing(spacing);
        } else {
          QPoint sp = spc.toPoint();
          if (!sp.isNull()) {
            gl->setHorizontalSpacing(sp.x());
            gl->setVerticalSpacing(sp.y());
          }
        }

        if (qcepDebug(DEBUG_WINDOW)) {
          printMessage(tr("Default spacing [%1,%2]").arg(spc.toPoint().x()).arg(spc.toPoint().y()));
        }
      }
    }

    foreach(QObject* chobj, obj->children()) {
      if (chobj) {
        shrinkObject(chobj, fontSize, spacing);
      }
    }
  }
}


