#ifndef NX_MESH_H
#define NX_MESH_H

#include <vector>

#include <QString>

#include <gp_Pnt.hxx>

#include "nx_tri3.h"
#include "st_stage.h"

class nx_mesh
{
public:
  nx_mesh();
  bool readINPF(QString &filename);

  ST_stage toStage();

  std::vector<gp_Pnt> theNodes;
  std::vector<nx_tri3> theTriangles;
};

#endif // NX_MESH_H
