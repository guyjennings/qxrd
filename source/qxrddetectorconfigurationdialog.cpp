#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrddetectorconfigurationdialog.h"
#include "ui_qxrddetectorconfigurationdialog.h"
#include "qxrddetectorproxy.h"
#include <QThread>
#include <stdio.h>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QPushButton>
#include <QFileDialog>

#include "qxrddetectorthread.h"

QxrdDetectorConfigurationDialog::QxrdDetectorConfigurationDialog(QxrdDetectorProxyPtr proxy, QWidget *parent) :
  QDialog(parent),
  m_Proxy(proxy),
  m_CurrentRow(0)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  if (m_Proxy) {
    int detNum = m_Proxy->property("detectorNumber").toInt();

    setWindowTitle(tr("Configure Det %1: %2 \"%3\" ")
                   .arg(detNum).arg(m_Proxy->detectorTypeName()).arg(m_Proxy->detectorName()));

    m_Proxy->pushPropertiesToDialog(this);
  }
}

QxrdDetectorConfigurationDialog::~QxrdDetectorConfigurationDialog()
{
}

void QxrdDetectorConfigurationDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdDetectorConfigurationDialog::accept()
{
  if (m_Proxy) {
    m_Proxy->pullPropertiesFromDialog(this);
  }

  QDialog::accept();
}

void QxrdDetectorConfigurationDialog::appendProperty(int type,
                                                     QString name,
                                                     QString description,
                                                     QVariant value)
{
  QLabel *propertyLabel = new QLabel();

  propertyLabel -> setText(description);

  QWidget *propertyEditor = NULL;
  QWidget *button = NULL;

  if (type == QxrdDetectorProxy::DetectorNumberProperty) {
    propertyEditor = new QLabel(value.toString());
  } else if (type == QxrdDetectorProxy::DetectorTypeProperty) {
    propertyEditor = new QLabel(QxrdDetectorThread::detectorTypeName(value.toInt()));

  } else if (type == QxrdDetectorProxy::StringProperty) {
    propertyEditor = new QLineEdit(value.toString());

  } else if (type == QxrdDetectorProxy::BooleanProperty) {
    QCheckBox *cb = new QCheckBox();
    cb->setCheckable(true);
    cb->setChecked(value.toBool());

    propertyEditor = cb;

  } else if (type == QxrdDetectorProxy::FixedIntegerProperty) {
    propertyEditor = new QLabel(value.toString());

  } else if (type == QxrdDetectorProxy::DirectoryProperty) {
    QLineEdit *le = new QLineEdit(value.toString());
    QPushButton *pb = new QPushButton("Browse");

    QSignalMapper *mapper = new QSignalMapper(pb);

    connect(pb, &QPushButton::clicked, mapper, (void (QSignalMapper::*)()) &QSignalMapper::map);
    mapper->setMapping(pb, m_CurrentRow);
    connect(mapper, (void (QSignalMapper::*)(int)) &QSignalMapper::mapped,
            this,   &QxrdDetectorConfigurationDialog::browseToDirectory);

    propertyEditor = le;
    button = pb;

  } else if (type == QxrdDetectorProxy::PEDetNumProperty) {
    QSpinBox *sb = new QSpinBox();
    sb -> setMinimum(-1);
    sb -> setMaximum(64);
    sb -> setValue(value.toInt());

    propertyEditor = sb;

  } else if (type == QxrdDetectorProxy::PESubTypeProperty) {
    QComboBox *cb = new QComboBox();
    QStringList sl = QxrdDetectorThread::detectorSubTypeNamesPE();
    cb -> addItems(sl);
    cb -> setCurrentIndex(value.toInt());

    propertyEditor = cb;

  } else if (type == QxrdDetectorProxy::PEGainProperty) {
    QComboBox *cb = new QComboBox();
    QStringList sl = QxrdDetectorThread::gainModeNamesPE();
    cb -> addItems(sl);
    cb -> setCurrentIndex(value.toInt());

    propertyEditor = cb;

  } else if (type == QxrdDetectorProxy::PEBinningProperty) {
    QComboBox *cb = new QComboBox();
    QStringList sl = QxrdDetectorThread::binningModeNamesPE();
    cb -> addItems(sl);
    cb -> setCurrentIndex(value.toInt());

    propertyEditor = cb;

  } else {
    printf("Unknown property type\n");
  }

  if (propertyEditor) {
    m_DetectorConfigGrid->addWidget(propertyLabel,  m_CurrentRow, 0);

    if (button) {
      m_DetectorConfigGrid->addWidget(propertyEditor, m_CurrentRow, 1);
      m_DetectorConfigGrid->addWidget(button, m_CurrentRow, 2);
    } else {
      m_DetectorConfigGrid->addWidget(propertyEditor, m_CurrentRow, 1, 1, 2);
    }

    m_CurrentRow++;
  }

  m_Widgets.append(propertyEditor);
}

QVariant QxrdDetectorConfigurationDialog::propertyValue(int propType, int i)
{
  QObject *w = m_Widgets.value(i);

  if (propType == QxrdDetectorProxy::DetectorNumberProperty) {
    return QVariant();
  } else if (propType == QxrdDetectorProxy::DetectorTypeProperty) {
    return QVariant();
  } else if (propType == QxrdDetectorProxy::StringProperty) {
    QLineEdit *le = qobject_cast<QLineEdit*>(w);

    if (le) {
      return le->text();
    }
  } else if (propType == QxrdDetectorProxy::BooleanProperty) {
    QCheckBox *cb = qobject_cast<QCheckBox*>(w);

    if (cb) {
      return cb->isChecked();
    }
  } else if (propType == QxrdDetectorProxy::PEDetNumProperty) {
    QSpinBox *sb = qobject_cast<QSpinBox*>(w);

    if (sb) {
      return sb->value();
    }
  } else if (propType == QxrdDetectorProxy::PESubTypeProperty) {
    QComboBox *cb = qobject_cast<QComboBox*>(w);

    if (cb) {
      return cb->currentIndex();
    }
  } else if (propType == QxrdDetectorProxy::PEGainProperty) {
    QComboBox *cb = qobject_cast<QComboBox*>(w);

    if (cb) {
      return cb->currentIndex();
    }
  } else if (propType == QxrdDetectorProxy::PEBinningProperty) {
    QComboBox *cb = qobject_cast<QComboBox*>(w);

    if (cb) {
      return cb->currentIndex();
    }
  } else {
    printf("Unknown property type\n");
  }

  return QVariant();
}

void QxrdDetectorConfigurationDialog::browseToDirectory(int i)
{
  QLineEdit *le = qobject_cast<QLineEdit*>(m_Widgets.value(i));

  if (le) {
    QString newPath = QFileDialog::getExistingDirectory(this, "Browse to Directory", le->text());

    if (newPath.length() > 0) {
      le->setText(newPath);
    }
  }
}
