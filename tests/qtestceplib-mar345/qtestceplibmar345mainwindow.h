#ifndef QTESTCEPLIBMAR345MAINWINDOW_H
#define QTESTCEPLIBMAR345MAINWINDOW_H

#include <QMainWindow>
#include "qcepproperty.h"
#include "qcepvector3dproperty.h"
#include "qcepmatrix3x3property.h"

namespace Ui {
  class QtestceplibMar345MainWindow;
}

class QtestceplibMar345MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibMar345MainWindow(QWidget *parent = 0);
  ~QtestceplibMar345MainWindow();
  
public slots:
  void doReadSettings();
  void doWriteSettings();

  void readSettings();
  void writeSettings();

  void readSettings(QString filePath);
  void writeSettings(QString filePath);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void doLoadImage();

  void printMessage(QString msg);

private:
  Ui::QtestceplibMar345MainWindow *ui;
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

  Q_PROPERTY(QcepVector3D vec3dPropA READ get_Vec3dPropA WRITE set_Vec3dPropA)
  QCEP_VECTOR3D_PROPERTY(Vec3dPropA)

  Q_PROPERTY(QcepVector3D vec3dPropB READ get_Vec3dPropB WRITE set_Vec3dPropB)
  QCEP_VECTOR3D_PROPERTY(Vec3dPropB)

  Q_PROPERTY(QcepMatrix3x3 mat3x3PropA READ get_Mat3x3PropA WRITE set_Mat3x3PropA)
  QCEP_MATRIX3X3_PROPERTY(Mat3x3PropA)

  Q_PROPERTY(QcepMatrix3x3 mat3x3PropB READ get_Mat3x3PropB WRITE set_Mat3x3PropB)
  QCEP_MATRIX3X3_PROPERTY(Mat3x3PropB)
};

#endif // QTESTCEPLIBMAR345MAINWINDOW_H
