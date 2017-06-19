#ifndef QTESTCEPLIBNEXUSDOCUMENT_H
#define QTESTCEPLIBNEXUSDOCUMENT_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepvector3dproperty.h"
#include "qcepmatrix3x3property.h"

class QtestceplibNexusDocument : public QcepObject
{
public:
  QtestceplibNexusDocument();

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

#endif // QTESTCEPLIBNEXUSDOCUMENT_H
