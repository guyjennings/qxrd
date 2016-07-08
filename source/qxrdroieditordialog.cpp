#include "qxrdroieditordialog.h"
#include "ui_qxrdroieditordialog.h"
#include "qxrdroicoordinates.h"
#include "qxrdroipolygon.h"
#include "qxrdroiellipse.h"
#include "qxrdroirectangle.h"
#include "qxrdroicenteredshape.h"
#include "qxrdpolygonpointsmodel.h"

QxrdROIEditorDialog::QxrdROIEditorDialog(QxrdROICoordinatesWPtr roi, QWidget *parent) :
  QDialog(parent),
  m_ROI(roi)
{
  setupUi(this);

  for (int i=0; i<QxrdROIShape::roiTypeCount(); i++) {
    QString name = QxrdROIShape::roiTypeName(i);
    m_InnerType->addItem(name);
    m_OuterType->addItem(name);
  }

  m_CenterX->setMinimum(-10000);
  m_CenterX->setMaximum(10000);
  m_CenterY->setMinimum(-10000);
  m_CenterY->setMaximum(10000);
  m_Rotation->setMinimum(-720);
  m_Rotation->setMaximum(720);

  m_InnerWidth->setMinimum(-100);
  m_InnerWidth->setMaximum(10000);
  m_InnerHeight->setMinimum(-100);
  m_InnerHeight->setMaximum(10000);

  m_OuterWidth->setMinimum(-100);
  m_OuterWidth->setMaximum(10000);
  m_OuterHeight->setMinimum(-100);
  m_OuterHeight->setMaximum(10000);

  QxrdROICoordinatesPtr roip(m_ROI);

  if (roip) {
    m_CenterX->setValue(roip->get_Center().x());
    m_CenterY->setValue(roip->get_Center().y());
    m_Rotation->setValue(roip->get_Rotation());

    m_InnerType->setCurrentIndex(roip->get_RoiInnerType());
    m_OuterType->setCurrentIndex(roip->get_RoiOuterType());

    QxrdROIShapePtr inner = roip->inner();

    QSharedPointer<QxrdROICenteredShape> innerc = qSharedPointerDynamicCast<QxrdROICenteredShape>(inner);
    QSharedPointer<QxrdROIPolygon>       innerp = qSharedPointerDynamicCast<QxrdROIPolygon>(inner);

    m_InnerWidth->setEnabled(innerc);
    m_InnerHeight->setEnabled(innerc);
    m_InnerPolygon->setEnabled(innerp);
    m_InnerPolygon->setModel(new QxrdPolygonPointsModel(QPolygonF()));

    if (innerc) {
      m_InnerWidth->setValue(innerc->get_HalfWidth());
      m_InnerHeight->setValue(innerc->get_HalfHeight());
    } else if (innerp) {
      m_InnerPolygon->setModel(new QxrdPolygonPointsModel(innerp->get_Poly()));
    } else {
    }

    QxrdROIShapePtr outer = roip->outer();

    QSharedPointer<QxrdROICenteredShape> outerc = qSharedPointerDynamicCast<QxrdROICenteredShape>(outer);
    QSharedPointer<QxrdROIPolygon>       outerp = qSharedPointerDynamicCast<QxrdROIPolygon>(outer);

    m_OuterWidth->setEnabled(outerc);
    m_OuterHeight->setEnabled(outerc);
    m_OuterPolygon->setEnabled(outerp);
    m_OuterPolygon->setModel(new QxrdPolygonPointsModel(QPolygonF()));

    if (outerc) {
      m_OuterWidth->setValue(outerc->get_HalfWidth());
      m_OuterHeight->setValue(outerc->get_HalfHeight());
    } else if (outerp) {
      m_OuterPolygon->setModel(new QxrdPolygonPointsModel(outerp->get_Poly()));
    } else {
    }
  }

  connect(m_InnerType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QxrdROIEditorDialog::newInnerType);
  connect(m_OuterType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QxrdROIEditorDialog::newOuterType);
}

QxrdROIEditorDialog::~QxrdROIEditorDialog()
{
}

QxrdROICoordinatesWPtr QxrdROIEditorDialog::roi()
{
  return m_ROI;
}

void QxrdROIEditorDialog::newInnerType(int n)
{
  m_InnerWidth->setEnabled(n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape);
  m_InnerHeight->setEnabled(n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape);
  m_InnerPolygon->setEnabled(n == QxrdROIShape::PolygonShape);
}

void QxrdROIEditorDialog::newOuterType(int n)
{
  m_OuterWidth->setEnabled(n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape);
  m_OuterHeight->setEnabled(n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape);
  m_OuterPolygon->setEnabled(n == QxrdROIShape::PolygonShape);
}

void QxrdROIEditorDialog::accept()
{

  QDialog::accept();
}
