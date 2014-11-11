#include "st_element.h"
#include "st_geom.h"

#include <QString>
#include <QStringList>
#include <QDebug>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepMesh.hxx>
#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom_BSplineSurface.hxx>
//#include <Geom2d_TrimmedCurve.hxx>
#include <Geom2d_Curve.hxx>

/////////////////////////////////////////////////
// Static methods for ST_element::intentType //
/////////////////////////////////////////////////

QString ST_element::typeToString(const ST_element::intentType &it)
{
  switch (it) {
  case REFLECTION:
    return "Reflection";
  case REFRACTION:
    return "Refraction";
  default:
    throw std::invalid_argument("Not an intentType!");
  }
}

ST_element::intentType ST_element::stringToType(QString s)
{
  s = s.toLower();
  if (s == "reflection")
    return REFLECTION;
  if (s == "refraction")
    return REFRACTION;
  throw std::invalid_argument("Unable to match string to an intentType!");
}

//////////////////////////////
// Constructor / destructor //
//////////////////////////////

// Note that this constructor does not assign a surface.
ST_element::ST_element()
{
  isEnabled = true;
  xyz[0] = 0;
  xyz[1] = 0;
  xyz[2] = 0;
  aim[0] = 0;
  aim[1] = 0;
  aim[2] = 1;
  zrot = 0;
  myIntent = REFLECTION;
  opticsName = "";
}

ST_element::ST_element(QTextStream &in)
{
  QString line = in.readLine();
  QStringList tokens = line.split("\t");

  isEnabled = tokens[0].toInt();
  xyz[0] = tokens[1].toDouble();
  xyz[1] = tokens[2].toDouble();
  xyz[2] = tokens[3].toDouble();
  aim[0] = tokens[4].toDouble();
  aim[1] = tokens[5].toDouble();
  aim[2] = tokens[6].toDouble();
  zrot = tokens[7].toDouble();
  QStringList tokens1 = static_cast<QStringList>(tokens.mid(8,9));
  aperture = ST_aperture(tokens1);
  QStringList tokens2 = static_cast<QStringList>(tokens.mid(17,10));
  surface = ST_surface::readSurface(tokens2);
  opticsName = tokens[27];
  myIntent = static_cast<ST_element::intentType>(tokens[28].toInt());
  myName = tokens[29];
}

ST_element::~ST_element()
{
}

////////////////////////////
// TopoDS_Shape functions //
////////////////////////////

gp_Trsf ST_element::getTransform() const
{
  gp_Trsf T0, T1, T2, T3, Tfinal;
  gp_Ax2 ax0, ax1, ax2, ax3, ax4;
  gp_Pnt p0(0,0,0), p1(xyz[0],xyz[1],xyz[2]);

  double euler[3];
  ST_geom::eulerY0X1Z1(xyz, aim, zrot, euler);

  T0.SetRotation(gp_Ax1(p0,ax0.YDirection()), euler[0]);
  ax1 = ax0.Transformed(T0);

  T1.SetRotation(gp_Ax1(p0,ax1.XDirection().Reversed()), euler[1]);
  ax2 = ax1.Transformed(T1);

  T2.SetRotation(gp_Ax1(p0,ax2.Direction().Reversed()), euler[2]);
  ax3 = ax2.Transformed(T2);

  T3.SetTranslation(p0,p1);
  ax4 = ax3.Transformed(T3);

  //Tfinal = T3.Multiplied(T2).Multiplied(T1).Multiplied(T0);
  Tfinal.SetTransformation(ax4, ax0);
  return Tfinal;
}

TopoDS_Shape ST_element::toShape(const QDir &parent) const
{
  QVector<Handle(Geom2d_Curve)> curves;
  Bnd_Box2d box;
  aperture.makeBounds(curves, box);

  surface->refresh(parent);
  bool isSingleAxis = aperture.type() == ST_aperture::SINGLE_AXIS_CURVATURE_SECTION;
  Handle(Geom_BSplineSurface) aSurface = surface->makeSurface(box, isSingleAxis);
  //getBSplineSurfaceInfo(aSurface);

  // Profile : Define the Topology
  BRepBuilderAPI_MakeWire aMakeWire;
  for (int i = 0; i < curves.size(); i++) {
    aMakeWire.Add(BRepBuilderAPI_MakeEdge(curves.at(i),aSurface));
  }
  std::cout << "IsDone(): " << aMakeWire.IsDone() << std::endl;
  TopoDS_Wire aWire(aMakeWire);
  BRepBuilderAPI_MakeFace faceMaker(aSurface, aWire);
  std::cout << "IsDone(): " << faceMaker.IsDone() << std::endl;

  /* Make it a shape. */
  TopoDS_Shape myBowl(faceMaker.Face());

  /* Make it a solid by extrusion. */
  //gp_Vec extrudeVec(0, 0, -3);
  //BRepPrimAPI_MakePrism aMakePrism(myBowl, extrudeVec);
  //TopoDS_Shape aPrism(aMakePrism);

  BRepBuilderAPI_Transform theTransform(getTransform());
  theTransform.Perform(myBowl,true);
  TopoDS_Shape newShape = theTransform.ModifiedShape(myBowl);

  /* Make a mesh of things. */
  //BRepMesh::Mesh(myBowl, 10);

  //TopoDS_Compound aRes;
  //BRep_Builder aBuilder;
  //aBuilder.MakeCompound (aRes);
  //aBuilder.Add(aRes, myBowl);
  //aBuilder.Add(aRes, newShape);
  //aBuilder.Add(aRes, aPrism);
  //aBuilder.Add(aRes, getBSplineSurfaceNet(aSurface));
  //std::cout << "Created TopoDS_Shape!" << std::endl;

  return newShape;
}

void ST_element::write(int version, QTextStream &out)
{
  QString sep("\t");
  switch (version) {
  default:
    out << isEnabled << sep
        << xyz[0] << sep
        << xyz[1] << sep
        << xyz[2] << sep
        << aim[0] << sep
        << aim[1] << sep
        << aim[2] << sep
        << zrot << sep
        << aperture << sep
        << *surface << sep
        << opticsName << sep
        << myIntent << sep
        << myName << endl;
  }
}
