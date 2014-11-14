#ifndef APPLICATIONCOMMON_H
#define APPLICATIONCOMMON_H

#include "DocumentCommon.h"

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  #include <QMdiArea>
#else
  #include <QWorkspace>
#endif
#include <QList>

class COMMONSAMPLE_EXPORT ApplicationCommonWindow: public QMainWindow
{
    Q_OBJECT

public:
	enum { FileNewId, FileCloseId, FileQuitId, ViewToolId, ViewStatusId, HelpAboutId };
  enum { ToolWireframeId, ToolShadingId, ToolColorId, ToolMaterialId, ToolTransparencyId, ToolDeleteId };

  ApplicationCommonWindow();
  ~ApplicationCommonWindow();

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  static QMdiArea*              getWorkspace();
#else
	static QWorkspace*              getWorkspace();
#endif
	static ApplicationCommonWindow* getApplication();
	static QString                  getResourceDir();

	virtual void                     updateFileActions();
  QList<QAction*>*                 getToolActions();
  QList<QAction*>*                 getMaterialActions();
	
protected:
  virtual DocumentCommon*          createNewDocument();
  int&                             getNbDocument();

public slots:
	
  DocumentCommon*                 onNewDoc();
  void                            onCloseWindow();
	virtual void                    onCloseDocument( DocumentCommon* theDoc );
  virtual void                    onSelectionChanged();
  virtual void                    onAbout();
    void                            onViewToolBar();
	void                            onViewStatusBar();
  void                            onToolAction();
	void                            onCreateNewView();
  void                            onWindowActivated ( QMdiSubWindow * w );
	void                            windowsMenuAboutToShow();
  void                            windowsMenuActivated( bool checked/*int id*/ );
	void                            onSetMaterial( int theMaterial );

protected:
  virtual void                    resizeEvent( QResizeEvent* );
  bool                            isDocument();
  QMenu*                          getFilePopup();
  QAction*                        getFileSeparator();
  QToolBar*                       getCasCadeBar();

private:
	void                            createStandardOperations();
	void                            createCasCadeOperations();
	void                            createWindowPopup();

private:
  int                             myNbDocuments;
	bool                            myIsDocuments;

	QList<QAction*>                 myStdActions;
  QList<QAction*>                 myToolActions;
  QList<QAction*>                 myMaterialActions;
  QList<DocumentCommon*>          myDocuments;

	QToolBar*                       myStdToolBar;
	QToolBar*                       myCasCadeBar;
	QMenu*                          myFilePopup;
	QMenu*                          myWindowPopup;
  QAction*                        myFileSeparator;
};

#endif


