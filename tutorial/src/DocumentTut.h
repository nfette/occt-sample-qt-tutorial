#ifndef DOCUMENTTUT_H
#define DOCUMENTTUT_H

#include "DocumentCommon.h"
#include "ApplicationTut.h"

#include <widget.h>

#include <Geom_BSplineSurface.hxx>
#include <TopoDS_Shape.hxx>

//class SurfConstruction;
//class ApplicationCommon;

class DocumentTut : public DocumentCommon
{
	Q_OBJECT

public:
   DocumentTut( const int, ApplicationCommonWindow* );
	~DocumentTut();

	void                           onMakeBottle();

public slots:

  void addShape(TopoDS_Shape shape);

private:
  //SurfConstruction*			         mySurfConstructor;
  Widget*                        myWidget;

};

#endif
