#include "ApplicationTut.h"
#include "DocumentTut.h"

#include <QFileDialog>
#include <QStatusBar>
#include <QMdiSubWindow>

ApplicationTut::ApplicationTut()
    : ApplicationWindow( )
{
  createMakeBottleOperation();
}

ApplicationTut::~ApplicationTut()
{
}

void ApplicationTut::createMakeBottleOperation(){
	QPixmap MakeBottleIcon;
	QString dir = getTutResourceDir() + QString( "/" );
  MakeBottleIcon = QPixmap( dir+QObject::tr( "ICON_MAKE_BOTTLE" ) );
	
  QAction * MakeBottleAction = new QAction( MakeBottleIcon, QObject::tr("TBR_MAKEBOT"), this );
  MakeBottleAction->setToolTip( QObject::tr( "TBR_MAKEBOT" ) );
  MakeBottleAction->setStatusTip( QObject::tr("TBR_MAKEBOT") );
  MakeBottleAction->setShortcut( QObject::tr( "CTRL+M" ) );
  connect( MakeBottleAction, SIGNAL( triggered() ) , this, SLOT( onMakeBottleAction() ) );
	
	myMakeBottleBar = addToolBar( tr( "Make Bottle" ) );
  insertToolBar( getCasCadeBar(), myMakeBottleBar );
  myMakeBottleBar->addAction( MakeBottleAction );
	myMakeBottleBar->hide();
}

void ApplicationTut::updateFileActions()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  if ( getWorkspace()->subWindowList().isEmpty() )
#else
  if ( getWorkspace()->windowList().isEmpty() )
#endif
  {
	  if ( !isDocument() )
		{
      myMakeBottleBar->show();
    }
    else
    {
      myMakeBottleBar->hide();
    }
  }
  ApplicationWindow::updateFileActions();
}

void ApplicationTut::onMakeBottleAction()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  QMdiArea* ws = ApplicationCommonWindow::getWorkspace();
  DocumentTut* doc = (DocumentTut*)((MDIWindow*)ws->activeSubWindow()->widget())->getDocument();
#else
	QWorkspace* ws = ApplicationCommonWindow::getWorkspace();
  DocumentTut* doc = (DocumentTut*)((MDIWindow*)ws->activeWindow())->getDocument();
#endif

	statusBar()->showMessage( QObject::tr("INF_MAKE_BOTTLE"), 5000 );
	doc->onMakeBottle();
	statusBar()->showMessage(QObject::tr("INF_DONE"));
}

QString ApplicationTut::getTutResourceDir()
{
    static QString resDir( ::getenv( "CSF_TutorialResourcesDefaults" ) );
    return resDir;
}

DocumentCommon* ApplicationTut::createNewDocument()
{
  return new DocumentTut( ++getNbDocument(), this );
}
