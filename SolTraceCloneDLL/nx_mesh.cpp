#include "nx_mesh.h"

#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QFile>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_Quaternion.hxx>

#include "nx_tri3.h"
#include "st_stage.h"
#include "st_geom.h"

nx_mesh::nx_mesh()
{

}

bool nx_mesh::readINPF(QString &filename)
{
  // First read the file.
  QString theText;

  QFile *file = new QFile(filename);
  if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(file);
    theText = in.readAll();
    file->close();
  } else {
    qDebug () << "Error: could not open file." << endl;
    return false;
  }

  // Read the nodes.
  int numNodes;
  QRegularExpression re;
  QRegularExpressionMatch match;
  QRegularExpressionMatchIterator iter;

  re = QRegularExpression("\\$ Thermal Nodes - User Model:\\s*(\\d+)");
  match = re.match(theText);

  if (match.hasMatch()) {
    numNodes = match.captured(1).toInt();
    qDebug() << "Number of nodes: " << numNodes;
    theText = theText.mid(match.capturedEnd());
    re = QRegularExpression("^\\s+(\\d+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)$");
    re.setPatternOptions(QRegularExpression::MultilineOption);

    theNodes.resize(numNodes);
    iter = re.globalMatch(theText);
    while (iter.hasNext()) {
      match = iter.next();
      int theNodeId, theNodeIndex;
      double x, y, z;
      // offset from 1...N to 0...N-1
      theNodeId = match.captured(1).toInt();
      theNodeIndex = theNodeId - 1;
      x = match.captured(2).toDouble();
      y = match.captured(3).toDouble();
      z = match.captured(4).toDouble();
      gp_Pnt p(x,y,z);
      theNodes[theNodeIndex] = p;
      // qDebug() << theNode << p.X() << p.Y() << p.Z();
    }
    qDebug() << "Finished reading nodes. Nodes read: " << theNodes.size();
  } else {
    qDebug() << "Error: could not find nodes in the file ...";
    return false;
  }

  // Read the elements.

  re = QRegularExpression("\\$ Thermal Elements - User Model");
  match = re.match(theText);

  if (match.hasMatch()) {
    qDebug() << "Found elements section.";
    theText = theText.mid(match.capturedEnd(1));

    re = QRegularExpression("\\s+(\\d+)\\s+O(\\d+)\\s+M(\\d+)\\s+O(\\d+)\\s+P(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)");
    iter = re.globalMatch(theText);
    int theElementId, topOpticsId, materialId, bottomOpticsId, propertyId, node1Id, node2Id, node3Id;
    int theElementIndex, node1Index, node2Index, node3Index;

    theTriangles.resize(match.lastCapturedIndex());
    while (iter.hasNext()) {
      match = iter.next();
      theElementId = match.captured(1).toInt();
      topOpticsId = match.captured(2).toInt();
      materialId = match.captured(3).toInt();
      bottomOpticsId = match.captured(4).toInt();
      propertyId = match.captured(5).toInt();
      node1Id = match.captured(6).toInt();
      node2Id = match.captured(7).toInt();
      node3Id = match.captured(8).toInt();

      theElementIndex = theElementId - 1;
      node1Index = node1Id - 1;
      node2Index = node2Id - 1;
      node3Index = node3Id - 1;
      nx_tri3 aTri3(node1Index, node2Index, node3Index);
      theTriangles.push_back(aTri3);
      // qDebug() << theElementId << aTri3.node1Id << aTri3.node2Id << aTri3.node3Id;
    }
    qDebug() << "Finished reading elements. Elements read: " << theTriangles.size();
  } else {
    qDebug() << "Error: could not find elements in the file ...";
    return false;
  }

  return true;
}

ST_stage nx_mesh::toStage() {
  // Create a stage and fill it with triangles representing the elements.

  ST_stage theStage;
  theStage.myName = "INPF mesh";

  for (int elemIndex = 0; elemIndex < theTriangles.size(); elemIndex++) {
    int theElementId = elemIndex + 1;
    nx_tri3 aTri3 = theTriangles[elemIndex];

    gp_Pnt node1 = theNodes[aTri3.node1Index];
    gp_Pnt node2 = theNodes[aTri3.node2Index];
    gp_Pnt node3 = theNodes[aTri3.node3Index];
    gp_Vec v12 = gp_Vec(node1, node2);
    gp_Vec v23 = gp_Vec(node2, node3);
    gp_Dir DX(v12);
    gp_Dir DZ(v12.Crossed(v23));
    gp_Ax2 ax(node1, DZ, DX);

    // Figure out a transform
    gp_Trsf trsf;
    trsf.SetTransformation(ax);

    // Load up the element.
    // Position and orientation.
    ST_element elem;
    node1.Coord(elem.xyz[0], elem.xyz[1], elem.xyz[2]);
    gp_Pnt aim = node1.Translated(DZ);
    aim.Coord(elem.aim[0], elem.aim[1], elem.aim[2]);
    double eulerAnglesRad[3];
    ST_geom::eulerYXZ(ax, eulerAnglesRad);
    elem.zrot = -eulerAnglesRad[2] * ST_geom::degPerRad;

    // Aperture
    double aperData[8];
    aperData[0] = node1.Transformed(trsf).X();
    aperData[1] = node1.Transformed(trsf).Y();
    aperData[2] = node2.Transformed(trsf).X();
    aperData[3] = node2.Transformed(trsf).Y();
    aperData[4] = node3.Transformed(trsf).X();
    aperData[5] = node3.Transformed(trsf).Y();
    aperData[6] = 0;
    aperData[7] = 0;
    elem.aperture = ST_aperture(ST_aperture::IRREGULAR_TRIANGLE, aperData);

    // Surface
    elem.surface = std::make_shared<ST_surface>();
    // Optics and intent
    elem.opticsName = "target";
    elem.myIntent = ST_element::REFLECTION;

    // Comment/Name
    elem.myName = QString("INPF.%1").arg(theElementId);

    theStage.rows.append(elem);
  }

  qDebug() << "Created the corresponding mesh stage";
  return theStage;
}
