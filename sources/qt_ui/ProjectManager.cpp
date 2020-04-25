#include "ProjectManager.h"

#include <iostream>

#include <QtWidgets>

#include "DataStore.h"
#include "Project.h"

#include "ProjectListModel.h"
#include "LineEditDialog.h"


ProjectManager::ProjectManager( QWidget* pParent )
    : QWidget( pParent )
      , m_pProjectListModel( new ProjectListModel( this ))
      , m_pLabel( new QLabel( tr( "Project:" ), this ))
      , m_pProjectsComboBox( new QComboBox( this ))
      , m_pAddProjectButton( new QPushButton( tr( "Create" ), this ))
      , m_pEditProjectButton( new QPushButton( tr( "Rename" ), this ))
      , m_pRemoveProjectButton( new QPushButton( tr( "Remove" ), this ))
      , m_pCreateProjectDialog( new LineEditDialog( this ))
      , m_pEditCurrentProjectDialog( new LineEditDialog( this ))
      , m_pMessageBox( new QMessageBox( this ))
{

}

void ProjectManager::initialize()
{
    m_pProjectsComboBox->setModel( m_pProjectListModel );

    auto pLayout = new QHBoxLayout( this );

    pLayout->addWidget( m_pLabel, 2 );
    pLayout->addWidget( m_pProjectsComboBox, 12 );
    pLayout->addWidget( m_pAddProjectButton, 1 );
    pLayout->addWidget( m_pEditProjectButton, 1 );
    pLayout->addWidget( m_pRemoveProjectButton, 1 );

    QObject::connect( m_pAddProjectButton, &QPushButton::clicked,
        this, &ProjectManager::showCreateNewProjectDialog );

    QObject::connect( m_pEditProjectButton, &QPushButton::clicked,
        this, &ProjectManager::showEditCurrentProjectDialog );

    QObject::connect( m_pRemoveProjectButton, &QPushButton::clicked,
        this, &ProjectManager::removeCurrentProject );

    QObject::connect( m_pProjectsComboBox, SIGNAL( currentIndexChanged( int )),
        this, SLOT( handleCurrentProjectIndexChanged( int )));

    QObject::connect( m_pCreateProjectDialog, &LineEditDialog::textSaved,
        this, &ProjectManager::createNewProject );

    QObject::connect( m_pEditCurrentProjectDialog, &LineEditDialog::textSaved,
        this, &ProjectManager::renameCurrentProject );

    m_pCreateProjectDialog->setWindowTitle( tr( "Project" ));
    m_pCreateProjectDialog->setLabelText( tr( "Enter name of new project" ));

    m_pEditCurrentProjectDialog->setWindowTitle( tr( "Project" ));
    m_pEditCurrentProjectDialog->setLabelText( tr( "Rename the project" ));
}

void ProjectManager::showCreateNewProjectDialog()
{
    m_pCreateProjectDialog->show();
}

void ProjectManager::showEditCurrentProjectDialog()
{
    if ( auto pProject = getCurrentProject(); pProject != nullptr )
    {
        auto strName = pProject->getStrName();

        m_pEditCurrentProjectDialog->setText( QString::fromStdString( strName ));
        m_pEditCurrentProjectDialog->show();
    }
}

void ProjectManager::removeCurrentProject()
{
    if ( m_pDataStore->getProjectsCount() == 0 )
        return;

    if ( m_pDataStore->getProjectsCount() > 1 )
    {
        auto nIndex = m_pProjectsComboBox->currentIndex();

        if ( nIndex < 0 )
            return;

        emit currentProjectChanged( nullptr );

        try
        {
            m_pDataStore->removeProject( static_cast<std::size_t>( nIndex ));

            if ( m_pDataStore->getProjectsCount() > 0 )
            {
                handleCurrentProjectIndexChanged( 0 );
                m_pProjectsComboBox->setCurrentIndex( 0 );
            }
        }
        catch ( const std::logic_error& e )
        {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
    else
    {
        emit currentProjectChanged( nullptr );
        m_pDataStore->removeProject( 0 );
    }

    m_pProjectListModel->updateAll();
    m_pProjectsComboBox->repaint();
}

void ProjectManager::setComboBoxData()
{
    m_pProjectListModel->setDataStore( m_pDataStore );
}

Project* ProjectManager::getCurrentProject()
{
    auto nIndex = m_pProjectsComboBox->currentIndex();
    if ( nIndex < 0 )
        return nullptr;

    return m_pDataStore->getProject( static_cast<std::size_t>( nIndex ));
}

void ProjectManager::createNewProject( const QString& strName )
{
    if ( const auto stdName = strName.toStdString(); !m_pDataStore->isHas( stdName ))
    {
        const auto[pProject, nPosition] = m_pDataStore->createProject( stdName );

        if ( pProject )
        {
            emit currentProjectChanged( pProject );
            m_pProjectsComboBox->setCurrentIndex( static_cast<int>( nPosition));
            m_pProjectListModel->update( static_cast<int>( nPosition));
            m_pProjectsComboBox->repaint();
        }
    }
    else
    {
        m_pMessageBox->setText( tr( "Project \"%1\" already exist" ).arg( strName ));
        m_pMessageBox->show();
    }

}

void ProjectManager::renameCurrentProject( const QString& strName )
{
    auto nIndex = m_pProjectsComboBox->currentIndex();

    if ( nIndex < 0 )
        return;

    const auto &[ isRenamed, nPosition ] = m_pDataStore->renameProject(
        static_cast<std::size_t>( nIndex ), strName.toStdString());

    if ( isRenamed )
    {
        m_pProjectsComboBox->setCurrentIndex( nPosition );
        m_pProjectsComboBox->repaint();
    }
}

void ProjectManager::handleCurrentProjectIndexChanged( int nIndex )
{
    if ( nIndex < 0 )
        return;

    try
    {
        const auto pProject = m_pDataStore->getProject( static_cast<std::size_t>( nIndex ));
        emit currentProjectChanged( pProject );
    }
    catch ( const std::logic_error& e )
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void ProjectManager::setDataStore( DataStore* pDataStore )
{
    m_pDataStore = pDataStore;
    setComboBoxData();
}
