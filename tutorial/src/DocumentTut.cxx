#include "DocumentTut.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_Transform.hxx>

TopoDS_Shape
MakeBottle(const Standard_Real myWidth , const Standard_Real myHeight , const Standard_Real myThickness);

DocumentTut::DocumentTut( const int theIndex, ApplicationCommonWindow* app )
: DocumentCommon( theIndex, app )
{
  myWidget = new Widget();
  connect(myWidget, SIGNAL(addShape(TopoDS_Shape)), this, SLOT(addShape(TopoDS_Shape)));
  myWidget->show();
}

DocumentTut::~DocumentTut()
{
}

void DocumentTut::onMakeBottle()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    TopoDS_Shape aBottle=MakeBottle(50,70,30);
    Handle(AIS_Shape) AISBottle=new AIS_Shape(aBottle);
    getContext()->SetMaterial(AISBottle,Graphic3d_NOM_GOLD);
    getContext()->SetDisplayMode(AISBottle,1,Standard_False);
    getContext()->Display(AISBottle, Standard_False);	
    getContext()->SetCurrentObject(AISBottle,Standard_False);
    emit selectionChanged();
    fitAll();
    QApplication::restoreOverrideCursor();
}

void DocumentTut::addShape(TopoDS_Shape shape)
{
  QApplication::setOverrideCursor( Qt::WaitCursor );

  gp_Trsf aTransform;
  aTransform.SetScale(gp_Pnt(), 1000.0);
  BRepBuilderAPI_Transform theTransform(aTransform);
  theTransform.Perform(shape,true);
  TopoDS_Shape newShape = theTransform.ModifiedShape(shape);

  Handle(AIS_Shape) theAIS_Shape = new AIS_Shape(newShape);
  getContext()->SetMaterial(theAIS_Shape,Graphic3d_NOM_GOLD);
  getContext()->SetDisplayMode(theAIS_Shape,1,Standard_False);
  getContext()->Display(theAIS_Shape, Standard_False);
  getContext()->SetCurrentObject(theAIS_Shape,Standard_False);
  emit selectionChanged();
  fitAll();

  QApplication::restoreOverrideCursor();
}
