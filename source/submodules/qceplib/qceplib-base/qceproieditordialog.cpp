#include "qceproieditordialog.h"
#include "qceproi.h"
#include "qceproipolygon.h"
#include "qceproiellipse.h"
#include "qceproirectangle.h"
#include "qceproicenteredshape.h"
#include "qceppolygonpointsmodel.h"

QcepROIEditorDialog::QcepROIEditorDialog(QcepROIWPtr roi, QWidget *parent) :
  QDialog(parent),
  m_ROI(roi)
{
  setupUi(this);

  for (int i=0; i<QcepROIShape::roiTypeCount(); i++) {
    QString name = QcepROIShape::roiTypeName(i);
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

  QcepROIPtr roip(m_ROI);

  if (roip) {
    m_CenterX->setValue(roip->get_Center().x());
    m_CenterY->setValue(roip->get_Center().y());
    m_Rotation->setValue(roip->get_Rotation());

    m_InnerType->setCurrentIndex(roip->get_RoiInnerType());
    m_OuterType->setCurrentIndex(roip->get_RoiOuterType());

    QcepROIShapePtr inner = roip->inner();

    QSharedPointer<QcepROICenteredShape> innerc = qSharedPointerDynamicCast<QcepROICenteredShape>(inner);
    QSharedPointer<QcepROIPolygon>       innerp = qSharedPointerDynamicCast<QcepROIPolygon>(inner);

    m_InnerPolygon->setModel(new QcepPolygonPointsModel(QPolygonF()));

    if (innerc) {
      m_InnerWidth->setValue(innerc->get_HalfWidth());
      m_InnerHeight->setValue(innerc->get_HalfHeight());
    } else if (innerp) {
      m_InnerPolygon->setModel(new QcepPolygonPointsModel(innerp->get_Poly()));
    } else {
    }

    newInnerType(roip->get_RoiInnerType());

    QcepROIShapePtr outer = roip->outer();

    QSharedPointer<QcepROICenteredShape> outerc = qSharedPointerDynamicCast<QcepROICenteredShape>(outer);
    QSharedPointer<QcepROIPolygon>       outerp = qSharedPointerDynamicCast<QcepROIPolygon>(outer);

    m_OuterPolygon->setModel(new QcepPolygonPointsModel(QPolygonF()));

    if (outerc) {
      m_OuterWidth->setValue(outerc->get_HalfWidth());
      m_OuterHeight->setValue(outerc->get_HalfHeight());
    } else if (outerp) {
      m_OuterPolygon->setModel(new QcepPolygonPointsModel(outerp->get_Poly()));
    } else {
    }

    newOuterType(roip->get_RoiOuterType());
  }

  connect(m_InnerType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepROIEditorDialog::newInnerType);
  connect(m_OuterType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepROIEditorDialog::newOuterType);

  connect(m_InnerAddPoint, &QAbstractButton::clicked, this, &QcepROIEditorDialog::innerAddPoint);
  connect(m_InnerDelPoint, &QAbstractButton::clicked, this, &QcepROIEditorDialog::innerDelPoint);
  connect(m_OuterAddPoint, &QAbstractButton::clicked, this, &QcepROIEditorDialog::outerAddPoint);
  connect(m_OuterDelPoint, &QAbstractButton::clicked, this, &QcepROIEditorDialog::outerDelPoint);
}

QcepROIEditorDialog::~QcepROIEditorDialog()
{
}

QcepROIWPtr QcepROIEditorDialog::roi()
{
  return m_ROI;
}

void QcepROIEditorDialog::newInnerType(int n)
{
  bool isCentered = n == QcepROIShape::RectangleShape || n == QcepROIShape::EllipseShape;
  bool isPoly     = n == QcepROIShape::PolygonShape;

  m_InnerWidth->setEnabled(isCentered);
  m_InnerHeight->setEnabled(isCentered);
  m_InnerHeightLabel->setEnabled(isCentered);
  m_InnerWidthLabel->setEnabled(isCentered);

  m_InnerPolygon->setEnabled(isPoly);
  m_InnerAddPoint->setEnabled(isPoly);
  m_InnerDelPoint->setEnabled(isPoly);
  m_InnerPolygonLabel->setEnabled(isPoly);
}

void QcepROIEditorDialog::newOuterType(int n)
{
  bool isCentered = n == QcepROIShape::RectangleShape || n == QcepROIShape::EllipseShape;
  bool isPoly     = n == QcepROIShape::PolygonShape;

  m_OuterWidth->setEnabled(isCentered);
  m_OuterHeight->setEnabled(isCentered);
  m_OuterWidthLabel->setEnabled(isCentered);
  m_OuterHeightLabel->setEnabled(isCentered);

  m_OuterPolygon->setEnabled(isPoly);
  m_OuterAddPoint->setEnabled(isPoly);
  m_OuterDelPoint->setEnabled(isPoly);
  m_OuterPolygonLabel->setEnabled(isPoly);
}

void QcepROIEditorDialog::innerAddPoint()
{
  QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_InnerPolygon->model());

  if (p) {
    p->addPoint(m_InnerPolygon->selectionModel());
  }
}

void QcepROIEditorDialog::innerDelPoint()
{
  QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_InnerPolygon->model());

  if (p) {
    p->delPoint(m_InnerPolygon->selectionModel());
  }
}

void QcepROIEditorDialog::outerAddPoint()
{
  QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_OuterPolygon->model());

  if (p) {
    p->addPoint(m_OuterPolygon->selectionModel());
  }
}

void QcepROIEditorDialog::outerDelPoint()
{
  QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_OuterPolygon->model());

  if (p) {
    p->delPoint(m_OuterPolygon->selectionModel());
  }
}

void QcepROIEditorDialog::accept()
{
  QcepROIPtr roip(m_ROI);

  if (roip) {
    int newInnerType = m_InnerType->currentIndex();
    int newOuterType = m_OuterType->currentIndex();

    if (newInnerType != roip->innerType()) {
      roip->changeInnerType(newInnerType);
    }

    if (newOuterType != roip->outerType()) {
      roip->changeOuterType(newOuterType);
    }

    roip->setCenterX(m_CenterX->value());
    roip->setCenterY(m_CenterY->value());
    roip->setRotation(m_Rotation->value());

    QcepROIShapePtr inner = roip->inner();

    QSharedPointer<QcepROICenteredShape> innerc = qSharedPointerDynamicCast<QcepROICenteredShape>(inner);
    QSharedPointer<QcepROIPolygon>       innerp = qSharedPointerDynamicCast<QcepROIPolygon>(inner);

    if (innerc) {
      innerc->set_HalfWidth(m_InnerWidth->value());
      innerc->set_HalfHeight(m_InnerHeight->value());
    }

    if (innerp) {
      QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_InnerPolygon->model());

      if (p) {
        innerp->clear();

        int n = p->rowCount();

        for (int i=0; i<n; i++) {
          innerp->append(p->polygonPoint(i));
        }
      }
    }

    QcepROIShapePtr outer = roip->outer();

    QSharedPointer<QcepROICenteredShape> outerc = qSharedPointerDynamicCast<QcepROICenteredShape>(outer);
    QSharedPointer<QcepROIPolygon>       outerp = qSharedPointerDynamicCast<QcepROIPolygon>(outer);

    if (outerc) {
      outerc->set_HalfWidth(m_OuterWidth->value());
      outerc->set_HalfHeight(m_OuterHeight->value());
    }

    if (outerp) {
      QcepPolygonPointsModel *p = qobject_cast<QcepPolygonPointsModel*>(m_OuterPolygon->model());

      if (p) {
        outerp->clear();

        int n = p->rowCount();

        for (int i=0; i<n; i++) {
          outerp->append(p->polygonPoint(i));
        }
      }
    }

    roip->changed();
  }

  QDialog::accept();
}
