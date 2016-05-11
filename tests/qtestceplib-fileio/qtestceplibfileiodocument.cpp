#include "qtestceplibfileiodocument.h"
#include "qcepimagedata.h"
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan.h"

QtestceplibFileIODocument::QtestceplibFileIODocument() :
  QcepObject("testFileIO", QcepObjectWPtr()),
  m_IntProp(this, "intProp", 42, "Integer Property"),
  m_DblProp(this, "dblProp", 42.0, "Double Property"),
  m_StrProp(this, "strProp", "42", "String Property"),
  m_SListProp(this, "sListProp", QcepStringList(), "String List Property"),
  m_Vec3dPropA(this, "vec3dPropA", QcepVector3D(1,2,3), "Vector 3D Property A"),
  m_Vec3dPropB(this, "vec3dPropB", 1,2,3, "Vector 3D Property B"),
  m_Mat3x3PropA(this, "mat3x3PropA", QcepMatrix3x3(), "Matrix 3x3 Property A"),
  m_Mat3x3PropB(this, "mat3x3PropB",
                1, 0, 0,
                0, 1, 0,
                0, 0, 1,
                "Matrix 3x3 Property B")
{
}

void QtestceplibFileIODocument::init()
{
  addChildPtr(new QcepDoubleImageData(QcepObjectWPtr(), "image1", 50, 50, 0));
  addChildPtr(new QcepUInt16ImageData(QcepObjectWPtr(), "image2", 100, 100, 0));
  QcepDataGroup *g1 = new QcepDataGroup(QcepObjectWPtr(), "group1");
  addChildPtr(g1);
  QcepDataGroup *g2 = new QcepDataGroup(QcepObjectWPtr(), "group2");
  g1->addChildPtr(g2);
  QStringList cols;

  cols<<"x"<<"y"<<"z";

  QcepDataColumnScan *s1 = new QcepDataColumnScan(QcepObjectWPtr(), "scan", cols, 100);

  g2->addChildPtr(s1);
}
