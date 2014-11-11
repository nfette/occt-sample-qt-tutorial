#include "st_rayintersection.h"

ST_rayintersection::ST_rayintersection()
{
}

ST_rayintersection::ST_rayintersection(double x, double y, double z,
                                       double cosx, double cosy, double cosz,
                                       int elementindex, int stageindex, int rayindex,
                                       HIT_TYPE aHitType)
  : X(x), Y(y), Z(z),
    CosX(cosx), CosY(cosy), CosZ(cosz),
    ElementIndex(elementindex), StageIndex(stageindex), RayIndex(rayindex),
    myHitType(aHitType)
{

}
