#ifndef ST_RAYINTERSECTION_H
#define ST_RAYINTERSECTION_H

class ST_rayintersection
{
public:

  enum HIT_TYPE {MISS, REFLECT, ABSORB};

  ST_rayintersection();
  ST_rayintersection(double x, double y, double z,
                     double cosx, double cosy, double cosz,
                     int elementIndex, int stageIndex, int rayIndex,
                     HIT_TYPE aHitType);
  double X;
  double Y;
  double Z;
  double CosX;
  double CosY;
  double CosZ;
  int ElementIndex;
  int StageIndex;
  int RayIndex;
  HIT_TYPE myHitType;
};

#endif // ST_RAYINTERSECTION_H
