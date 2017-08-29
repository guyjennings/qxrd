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

    m_InnerPolygon->setModel(new QxrdPolygonPointsModel(QPolygonF()));

    if (innerc) {
      m_InnerWidth->setValue(innerc->get_HalfWidth());
      m_InnerHeight->setValue(innerc->get_HalfHeight());
    } else if (innerp) {
      m_InnerPolygon->setModel(new QxrdPolygonPointsModel(innerp->get_Poly()));
    } else {
    }

    newInnerType(roip->get_RoiInnerType());

    QxrdROIShapePtr outer = roip->outer();

    QSharedPointer<QxrdROICenteredShape> outerc = qSharedPointerDynamicCast<QxrdROICenteredShape>(outer);
    QSharedPointer<QxrdROIPolygon>       outerp = qSharedPointerDynamicCast<QxrdROIPolygon>(outer);

    m_OuterPolygon->setModel(new QxrdPolygonPointsModel(QPolygonF()));

    if (outerc) {
      m_OuterWidth->setValue(outerc->get_HalfWidth());
      m_OuterHeight->setValue(outerc->get_HalfHeight());
    } else if (outerp) {
      m_OuterPolygon->setModel(new QxrdPolygonPointsModel(outerp->get_Poly()));
    } else {
    }

    newOuterType(roip->get_RoiOuterType());
  }

  connect(m_InnerType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QxrdROIEditorDialog::newInnerType);
  connect(m_OuterType, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QxrdROIEditorDialog::newOuterType);

  connect(m_InnerAddPoint, &QAbstractButton::clicked, this, &QxrdROIEditorDialog::innerAddPoint);
  connect(m_InnerDelPoint, &QAbstractButton::clicked, this, &QxrdROIEditorDialog::innerDelPoint);
  connect(m_OuterAddPoint, &QAbstractButton::clicked, this, &QxrdROIEditorDialog::outerAddPoint);
  connect(m_OuterDelPoint, &QAbstractButton::clicked, this, &QxrdROIEditorDialog::outerDelPoint);
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
  bool isCentered = n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape;
  bool isPoly     = n == QxrdROIShape::PolygonShape;

  m_InnerWidth->setEnabled(isCentered);
  m_InnerHeight->setEnabled(isCentered);
  m_InnerHeightLabel->setEnabled(isCentered);
  m_InnerWidthLabel->setEnabled(isCentered);

  m_InnerPolygon->setEnabled(isPoly);
  m_InnerAddPoint->setEnabled(isPoly);
  m_InnerDelPoint->setEnabled(isPoly);
  m_InnerPolygonLabel->setEnabled(isPoly);
}

void QxrdROIEditorDialog::newOuterType(int n)
{
  bool isCentered = n == QxrdROIShape::RectangleShape || n == QxrdROIShape::EllipseShape;
  bool isPoly     = n == QxrdROIShape::PolygonShape;

  m_OuterWidth->setEnabled(isCentered);
  m_OuterHeight->setEnabled(isCentered);
  m_OuterWidthLabel->setEnabled(isCentered);
  m_OuterHeightLabel->setEnabled(isCentered);

  m_OuterPolygon->setEnabled(isPoly);
  m_OuterAddPoint->setEnabled(isPoly);
  m_OuterDelPoint->setEnabled(isPoly);
  m_OuterPolygonLabel->setEnabled(isPoly);
}

void QxrdROIEditorDialog::innerAddPoint()
{
  QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_InnerPolygon->model());

  if (p) {
    p->addPoint(m_InnerPolygon->selectionModel());
  }
}

void QxrdROIEditorDialog::innerDelPoint()
{
  QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_InnerPolygon->model());

  if (p) {
    p->delPoint(m_InnerPolygon->selectionModel());
  }
}

void QxrdROIEditorDialog::outerAddPoint()
{
  QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_OuterPolygon->model());

  if (p) {
    p->addPoint(m_OuterPolygon->selectionModel());
  }
}

void QxrdROIEditorDialog::outerDelPoint()
{
  QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_OuterPolygon->model());

  if (p) {
    p->delPoint(m_OuterPolygon->selectionModel());
  }
}

void QxrdROIEditorDialog::accept()
{
  QxrdROICoordinatesPtr roip(m_ROI);

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

    QxrdROIShapePtr inner = roip->inner();

    QSharedPointer<QxrdROICenteredShape> innerc = qSharedPointerDynamicCast<QxrdROICenteredShape>(inner);
    QSharedPointer<QxrdROIPolygon>       innerp = qSharedPointerDynamicCast<QxrdROIPolygon>(inner);

    if (innerc) {
      innerc->set_HalfWidth(m_InnerWidth->value());
      innerc->set_HalfHeight(m_InnerHeight->value());
    }

    if (innerp) {
      QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_InnerPolygon->model());

      if (p) {
        innerp->clear();

        int n = p->rowCount();

        for (int i=0; i<n; i++) {
          innerp->append(p->polygonPoint(i));
        }
      }
    }

    QxrdROIShapePtr outer = roip->outer();

    QSharedPointer<QxrdROICenteredShape> outerc = qSharedPointerDynamicCast<QxrdROICenteredShape>(outer);
    QSharedPointer<QxrdROIPolygon>       outerp = qSharedPointerDynamicCast<QxrdROIPolygon>(outer);

    if (outerc) {
      outerc->set_HalfWidth(m_OuterWidth->value());
      outerc->set_HalfHeight(m_OuterHeight->value());
    }

    if (outerp) {
      QxrdPolygonPointsModel *p = qobject_cast<QxrdPolygonPointsModel*>(m_OuterPolygon->model());

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
