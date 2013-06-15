#ifndef QTESTCEPLIBMAINWINDOW_H
#define QTESTCEPLIBMAINWINDOW_H

#include <QMainWindow>
#include "qcepproperty.h"
#include "qcepvector3dproperty.h"
#include "qcepmatrix3x3property.h"

namespace Ui {
  class QtestceplibMainWindow;
}

class QtestceplibMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibMainWindow(QWidget *parent = 0);
  ~QtestceplibMainWindow();
  
public slots:
  void doReadSettings();
  void doWriteSettings();

  void readSettings();
  void writeSettings();

  void readSettings(QString filePath);
  void writeSettings(QString filePath);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

private:
  Ui::QtestceplibMainWindow *ui;
  mutable QMutex m_Mutex;

public:
  Q_PROPERTY(int intProp READ get_IntProp WRITE set_IntProp)
  QCEP_INTEGER_PROPERTY(IntProp)

  Q_PROPERTY(double dblProp READ get_DblProp WRITE set_DblProp)
  QCEP_DOUBLE_PROPERTY(DblProp)

  Q_PROPERTY(QString strProp READ get_StrProp WRITE set_StrProp)
  QCEP_STRING_PROPERTY(StrProp)

  Q_PROPERTY(QcepStringList sListProp READ get_SListProp WRITE set_SListProp)
  QCEP_STRING_LIST_PROPERTY(SListProp)

  Q_PROPERTY(QVector3D vec3dProp READ get_Vec3dProp WRITE set_Vec3dProp)
  QCEP_VECTOR3D_PROPERTY(Vec3dProp)

  Q_PROPERTY(QMatrix3x3 mat3x3Prop READ get_Mat3x3Prop WRITE set_Mat3x3Prop)
  QCEP_MATRIX3X3_PROPERTY(Mat3x3Prop)
};

#endif // QTESTCEPLIBMAINWINDOW_H
