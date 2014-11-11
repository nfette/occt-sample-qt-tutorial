#ifndef APPLICATIONTUT_H
#define APPLICATIONTUT_H

#include "DocumentTut.h"
#include "Application.h"

class ApplicationTut: public ApplicationWindow
{
    Q_OBJECT

public:
	
  ApplicationTut();
  ~ApplicationTut();

  static QString    getTutResourceDir();
  virtual void      updateFileActions();

public slots:
	
	void							onMakeBottleAction();

private:
	void              createMakeBottleOperation();

private:
	QToolBar*					myMakeBottleBar;

protected:
  DocumentCommon* createNewDocument();
};

#endif
