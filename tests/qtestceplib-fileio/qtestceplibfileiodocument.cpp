#include "qtestceplibfileiodocument.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan.h"

QtestceplibFileIODocument::QtestceplibFileIODocument() :
  QcepSerializableObject("testFileIO"),
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
  QcepDoubleImageDataPtr image1(new QcepDoubleImageData("image1", 50, 50, 0));
  QcepUInt16ImageDataPtr image2(new QcepUInt16ImageData("image2", 100, 100, 0));
  QcepDataGroupPtr       g1(new QcepDataGroup("group1"));
  QcepDataGroupPtr       g2(new QcepDataGroup("group2"));

  addChildPtr(image1);
  addChildPtr(image2);
  addChildPtr(g1);
  g1->addChildPtr(g2);

  QStringList cols;
  cols<<"x"<<"y"<<"z";

  QcepDataColumnScanPtr s1(new QcepDataColumnScan("scan", cols, 100));

  g2->addChildPtr(s1);
}
