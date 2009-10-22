/******************************************************************
*
*  $Id: qxrdfilebrowser.cpp,v 1.1 2009/10/22 19:22:28 jennings Exp $
*
*******************************************************************/

#include "qxrdfilebrowser.h"
#include "qxrddataprocessor.h"
#include <QDirModel>
#include <QFileDialog>

QxrdFileBrowser::QxrdFileBrowser(QxrdDataProcessor *processor, QWidget *parent)
  : QWidget(parent),
    m_Processor(processor),
    m_Model(NULL),
    m_FileDialog(NULL)
{
  setupUi(this);

  m_Model = new QDirModel();
  m_FileBrowser -> setModel(m_Model);
  m_FileBrowser -> setRootIndex(m_Model->index(QDir::currentPath()));

  m_FileDialog = new QFileDialog();

  m_GridLayout -> addWidget(m_FileDialog);
}

/******************************************************************
*
*  $Log: qxrdfilebrowser.cpp,v $
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/
