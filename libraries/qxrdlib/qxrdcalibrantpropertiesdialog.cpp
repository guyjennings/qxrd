#include "qxrdcalibrantpropertiesdialog.h"
#include "ui_qxrdcalibrantpropertiesdialog.h"
#include "qxrdcalibrant.h"

QxrdCalibrantPropertiesDialog::QxrdCalibrantPropertiesDialog(QWidget *parent, QxrdCalibrantPtr cal) :
  QDialog(parent),
  m_Calibrant(cal)
{
  setupUi(this);

  if (m_Calibrant) {
    if (m_Calibrant->isLocked()) {
      setWindowTitle(tr("Calibrant %1 Properties (Locked)").arg(m_Calibrant->get_Name()));

      m_Name        -> setEnabled(false);
      m_Description -> setEnabled(false);
      m_Symmetry    -> setEnabled(false);
      m_Symmetry    -> setEditable(false);
      m_A           -> setEnabled(false);
      m_B           -> setEnabled(false);
      m_C           -> setEnabled(false);
      m_Alpha       -> setEnabled(false);
      m_Beta        -> setEnabled(false);
      m_Gamma       -> setEnabled(false);
    } else {
      setWindowTitle(tr("Calibrant %1 Properties").arg(m_Calibrant->get_Name()));
    }

    m_Symmetry    -> addItem("Hexagonal");
    m_Symmetry    -> addItem("R Hexagonal");
    m_Symmetry    -> addItem("Simple Cubic");
    m_Symmetry    -> addItem("BCC");
    m_Symmetry    -> addItem("FCC");
    m_Symmetry    -> addItem("Diamond");

    m_IsUsed      -> setChecked(m_Calibrant->get_IsUsed());
    m_Name        -> setText(m_Calibrant->get_Name());
    m_Description -> setText(m_Calibrant->get_Description());
    m_Symmetry    -> setCurrentIndex(m_Calibrant->get_Symmetry());
    m_A           -> setValue(m_Calibrant->get_A());
    m_B           -> setValue(m_Calibrant->get_B());
    m_C           -> setValue(m_Calibrant->get_C());
    m_Alpha       -> setValue(m_Calibrant->get_Alpha());
    m_Beta        -> setValue(m_Calibrant->get_Beta());
    m_Gamma       -> setValue(m_Calibrant->get_Gamma());
  }
}

QxrdCalibrantPropertiesDialog::~QxrdCalibrantPropertiesDialog()
{
}

void QxrdCalibrantPropertiesDialog::accept()
{
  if (m_Calibrant) {
    m_Calibrant -> set_IsUsed(m_IsUsed->isChecked());

    if (!m_Calibrant->isLocked()) {
      m_Calibrant -> set_Name       (m_Name        -> text());
      m_Calibrant -> set_Description(m_Description -> text());
      m_Calibrant -> set_Symmetry   (m_Symmetry    -> currentIndex());
      m_Calibrant -> set_A          (m_A           -> value());
      m_Calibrant -> set_B          (m_B           -> value());
      m_Calibrant -> set_C          (m_C           -> value());
      m_Calibrant -> set_Alpha      (m_Alpha       -> value());
      m_Calibrant -> set_Beta       (m_Beta        -> value());
      m_Calibrant -> set_Gamma      (m_Gamma       -> value());
    }
  }

  QDialog::accept();
}
