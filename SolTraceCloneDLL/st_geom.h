#ifndef ST_GEOM_H
#define ST_GEOM_H

#include <gp_Ax2.hxx>

class ST_geom
{
public:

  static const double pi;

  static const double degPerRad;

  static const double radPerDeg;

  static void eulerY0X0Z0(const double (&origin)[3], const double (&aimpoint)[3],
    const double &zRotDeg, double (&eulerAnglesRad)[3]);

  static void eulerY0X1Z0(const double (&origin)[3], const double (&aimpoint)[3],
    const double &zRotDeg, double (&eulerAnglesRad)[3]);

  static void eulerY0X1Z1(const double (&origin)[3], const double (&aimpoint)[3],
    const double &zRotDeg, double (&eulerAnglesRad)[3]);

  // Return the euler angles in radians, using short formulae,
  // for intrinsic rotations about +Y, +X, then +Z axes.
  // (You may need to flip signs about X and Z for SolTrace.)
  // If z axis is at (0,1,0) or (0,-1,0), there is not a unique solution.
  // By convention, alpha is zero in this case.
  static void eulerYXZ(const gp_Ax2, double (&eulerAnglesRad)[3]);

  //ST_geom();
};

#endif // ST_GEOM_H
