#include "st_postprocess.h"
#include "nx_tri3.h"

#include <QFile>
#include <QDebug>

#include <gp_Vec.hxx>

ST_postprocess::ST_postprocess(nx_mesh *mesh, ST_raydb *rays, int stageIndex)
  : mesh(mesh), rays(rays), stageIndex(stageIndex)
{
  cellAbsorbedPower.resize(mesh->theTriangles.size());
  cellIncidentPower.resize(mesh->theTriangles.size());
  cellRayHitsAbsorb.resize(mesh->theTriangles.size());
  cellRayHitsIncident.resize(mesh->theTriangles.size());
  cellIncidentIrradiance.resize(mesh->theTriangles.size());
  cellAbsorbedIrradiance.resize(mesh->theTriangles.size());
  cellIncidentSuns.resize(mesh->theTriangles.size());
  cellAbsorbedSuns.resize(mesh->theTriangles.size());
}

void ST_postprocess::computeStats()
{

  // Count ray hits.
  for (int elemIndex = 0; elemIndex < mesh->theTriangles.size(); elemIndex++) {
    rays->countHits(stageIndex, elemIndex,
                   cellRayHitsIncident[elemIndex], cellRayHitsAbsorb[elemIndex]);
    // Heat flows
    cellAbsorbedPower[elemIndex] = rays->powerPerRay * cellRayHitsAbsorb[elemIndex];
    cellIncidentPower[elemIndex] = rays->powerPerRay * cellRayHitsIncident[elemIndex];

    // Calculate cell area for flux per area.

    int node1index = mesh->theTriangles[elemIndex].node1Index;
    int node2index = mesh->theTriangles[elemIndex].node2Index;
    int node3index = mesh->theTriangles[elemIndex].node3Index;
    gp_Pnt node1 = mesh->theNodes[node1index];
    gp_Pnt node2 = mesh->theNodes[node2index];
    gp_Pnt node3 = mesh->theNodes[node3index];
    gp_Vec vec1(node1, node2);
    gp_Vec vec2(node1, node3);
    double area = 0.5 * vec1.CrossMagnitude(vec2);
    cellIncidentIrradiance[elemIndex] = cellIncidentPower[elemIndex] / area;
    cellAbsorbedIrradiance[elemIndex] = cellAbsorbedPower[elemIndex] / area;
    double DNI = 1000;
    cellIncidentSuns[elemIndex] = cellIncidentIrradiance[elemIndex] / DNI;
    cellAbsorbedSuns[elemIndex] = cellAbsorbedIrradiance[elemIndex] / DNI;
  }
}

bool ST_postprocess::writeVtk(QString &VTKfilename)
{
  QFile *VTKfile = new QFile(VTKfilename);
  if (VTKfile->open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream vtk(VTKfile);
    vtk << "# vtk DataFile Version 3.1" << endl;
    vtk << "This file generated from SolTrace results and an NX INPF mesh." << endl;
    vtk << "ASCII" << endl;
    vtk << "DATASET UNSTRUCTURED_GRID" << endl;
    vtk << "POINTS " << mesh->theNodes.size() << " FLOAT" << endl;
    for (int nodeIndex = 0; nodeIndex < mesh->theNodes.size(); nodeIndex++) {
      gp_Pnt node = mesh->theNodes[nodeIndex];
      vtk << node.X() << " "
          << node.Y() << " "
          << node.Z() << endl;
    }
    vtk << endl;

    int nodeSumOverElements = 3 * static_cast<int>(mesh->theTriangles.size());
    int entriesInCellsData = static_cast<int>(mesh->theTriangles.size()) + nodeSumOverElements;
    vtk << "CELLS " << mesh->theTriangles.size() << " " << entriesInCellsData << endl;
    for (int triIndex = 0; triIndex < mesh->theTriangles.size(); triIndex++) {
      nx_tri3 tri = mesh->theTriangles[triIndex];
      vtk << 3 << " "
          << tri.node1Index << " "
          << tri.node2Index << " "
          << tri.node3Index << endl;
    }
    vtk << endl;

    vtk << "CELL_TYPES " << mesh->theTriangles.size() << endl;
    for (int i = 0; i < mesh->theTriangles.size(); i++) {
      vtk << "5" << endl;
    }
    vtk << endl;

    vtk << "CELL_DATA " << mesh->theTriangles.size() << endl;

    vtk << "SCALARS " << "cellIndex" << " " << "unsigned_int" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellIndex << endl;
    }

    vtk << "SCALARS " << "rays_incident" << " " << "unsigned_int" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellRayHitsIncident[cellIndex] << endl;
    }

    vtk << "SCALARS " << "rays_absorbed" << " " << "unsigned_int" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellRayHitsAbsorb[cellIndex] << endl;
    }

    vtk << "SCALARS " << "power_incident_watts" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellIncidentPower[cellIndex] << endl;
    }

    vtk << "SCALARS " << "power_absorbed_watts" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellAbsorbedPower[cellIndex] << endl;
    }

    vtk << "SCALARS " << "irrad_incident_W_per_m2" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellIncidentIrradiance[cellIndex] << endl;
    }

    vtk << "SCALARS " << "irrad_absorb_W_per_m2" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellAbsorbedIrradiance[cellIndex] << endl;
    }

    vtk << "SCALARS " << "irrad_incident_suns" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellIncidentSuns[cellIndex] << endl;
    }

    vtk << "SCALARS " << "irrad_absorb_suns" << " " << "float" << endl;
    vtk << "LOOKUP_TABLE default" << endl;
    for (int cellIndex = 0; cellIndex < mesh->theTriangles.size(); cellIndex++) {
      vtk << cellAbsorbedSuns[cellIndex] << endl;
    }

    VTKfile->close();
    return true;

  } else {
    qDebug() << "Error: unable to open VTK file: " << VTKfilename;
    return false;
  }
}
