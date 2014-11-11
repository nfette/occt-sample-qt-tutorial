#include "st_stage.h"
#include "st_geom.h"

#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QDebug>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_XYZ.hxx>
#include <TopoDS_Compound.hxx>

ST_stage::ST_stage()
{
  xyz[0] = 0;
  xyz[1] = 0;
  xyz[2] = 0;
  aim[0] = 0;
  aim[1] = 0;
  aim[2] = 1;
  zrot = 0;
  isVirtual = false;
  isMultiHit = true;
  isTraceThrough = false;
  myName = "New Stage";
}

ST_stage::ST_stage(QTextStream &in)
{
  QString line;
  QStringList tokens;
  int numElements;
  //line = in.readLine();
  //tokens = line.split("\t");

  QString dummy;
  in >> dummy >> dummy >> xyz[0] >> xyz[1] >> xyz[2];
  in >> dummy >> aim[0] >> aim[1] >> aim[2];
  in >> dummy >> zrot;
  int aBool;
  in >> dummy >> aBool;
  isVirtual = aBool;
  in >> dummy >> aBool;
  isMultiHit = aBool;
  in >> dummy >> numElements;
  in >> dummy >> aBool;
  isTraceThrough = aBool;

  dummy = in.readLine();
  line = in.readLine();
  myName = line;

  for (int i = 0; i < numElements; i++) {
    ST_element aRow(in);
    rows.append(aRow);
  }
}

void ST_stage::write(QTextStream &out, int version)
{
  QString sep("\t");
  switch (version) {
  case 1:
    // Used for file.stage
    // STAGE.1 XYZ 0 0 0 AXAYAZ 0 0 1 ZR 0 NELM 0 VIR 0 MUL 1 TRA 0
    out << "STAGE.1" << sep
        << "XYZ" << sep
        << xyz[0] << sep
        << xyz[1] << sep
        << xyz[2] << sep
        << "AXAYAZ" << sep
        << aim[0] << sep
        << aim[1] << sep
        << aim[2] << sep
        << "ZR" << sep
        << zrot << sep
        << "NELM" << sep
        << rows.size() << sep
        << "VIR" << sep
        << isVirtual << sep
        << "MUL" << sep
        << isMultiHit << sep
        << "TRA" << sep
        << isTraceThrough << endl;
    for (int i = 0; i < rows.size(); i++) {
      rows[i].write(0, out);
    }
    return;
  default:
    // Used for file.stinput
    // STAGE	XYZ	0	0	0	AIM	0	0	1	ZROT	0	VIRTUAL	0	MULTIHIT	0	ELEMENTS	18	TRACETHROUGH	1
    out << "STAGE" << sep
        << "XYZ" << sep
        << xyz[0] << sep
        << xyz[1] << sep
        << xyz[2] << sep
        << "AIM" << sep
        << aim[0] << sep
        << aim[1] << sep
        << aim[2] << sep
        << "ZROT" << sep
        << zrot << sep
        << "VIRTUAL" << sep
        << isVirtual << sep
        << "MULTIHIT" << sep
        << isMultiHit << sep
        << "ELEMENTS" << sep
        << rows.size() << sep
        << "TRACETHROUGH" << sep
        << isTraceThrough << endl;
    out << myName << endl;
    for (int i = 0; i < rows.size(); i++) {
      rows[i].write(0, out);
    }
    return;
  }
}

bool ST_stage::writeFile(QString &filename, int version)
{
    QFile *fileOut = new QFile(filename);
    if (fileOut->open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream fileStream(fileOut);
      write(fileStream, version);
      fileOut->close();
      qDebug() << "Mesh stage written to " << filename;
      return true;
    } else {
      qDebug() << "Error: could not open the output file " << filename;
      return false;
    }
}

gp_Trsf ST_stage::getTransform() const
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

// Return the given row with respect to global coordinates.
TopoDS_Shape ST_stage::toShape(const QDir &parent, int whichRow) const
{
  TopoDS_Shape origShape = rows.at(whichRow).toShape(parent);
  BRepBuilderAPI_Transform theTransform(getTransform());
  theTransform.Perform(origShape,true);
  TopoDS_Shape newShape = theTransform.ModifiedShape(origShape);
  return newShape;
}

// Return all of the row shapes in a compound with respect to global coordinates.
TopoDS_Shape ST_stage::toShape(const QDir &parent) const
{
  TopoDS_Compound aCompound;
  BRep_Builder aBuilder;
  aBuilder.MakeCompound (aCompound);
  foreach (ST_element theRow, rows)
  {
    aBuilder.Add(aCompound, theRow.toShape(parent));
  }
  BRepBuilderAPI_Transform theTransform(getTransform());
  theTransform.Perform(aCompound,true);
  TopoDS_Shape newShape = theTransform.ModifiedShape(aCompound);
  return newShape;
}
