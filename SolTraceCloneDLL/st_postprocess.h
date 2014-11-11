#ifndef ST_POSTPROCESS_H
#define ST_POSTPROCESS_H

#include <vector>

#include <QString>

#include "nx_mesh.h"
#include "st_raydb.h"

class ST_postprocess
{
public:
  // Step 1.
  ST_postprocess(nx_mesh *mesh, ST_raydb *rays, int stageIndex);
  // Step 2.
  void computeStats();
  // Step 3.
  bool writeVtk(QString &VTKfilename);

  int stageIndex;
  nx_mesh *mesh;
  ST_raydb *rays;

  std::vector<int> cellRayHitsIncident;
  std::vector<int> cellRayHitsAbsorb;
  std::vector<double> cellIncidentPower;
  std::vector<double> cellAbsorbedPower;
  std::vector<double> cellIncidentIrradiance;
  std::vector<double> cellAbsorbedIrradiance;
  std::vector<double> cellIncidentSuns;
  std::vector<double> cellAbsorbedSuns;

};

#endif // ST_POSTPROCESS_H
