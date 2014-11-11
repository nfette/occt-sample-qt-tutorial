#include "st_geom.h"

#include <cmath>
#include <cerrno>
#include <gp_Dir.hxx>

/*
ST_geom::ST_geom()
{
}
*/

const double ST_geom::pi = 3.1415926535897932384626433;

const double ST_geom::degPerRad = 180. / ST_geom::pi;

const double ST_geom::radPerDeg = ST_geom::pi / 180.;

void ST_geom::eulerY0X0Z0(const double (&origin)[3], const double (&aimpoint)[3],
                    const double &zRotDeg, double (&eulerAnglesRad)[3])
{
  eulerAnglesRad[2] = zRotDeg * radPerDeg;
  double z0 = aimpoint[0] - origin[0],
      z1 = aimpoint[1] - origin[1],
      z2 = aimpoint[2] - origin[2];
  double oneOnR2 = 1. / std::sqrt(z0 * z0 + z1 * z1 + z2 * z2);
  z0 *= oneOnR2;
  z1 *= oneOnR2;
  z2 *= oneOnR2;
  double sinBeta = - z1, oneOnCosBeta;
  eulerAnglesRad[1] = std::asin(sinBeta);
  oneOnCosBeta = 1. / std::cos(sinBeta);
  double cosPart = z2 * oneOnCosBeta,
      sinPart = z0 * oneOnCosBeta;
  eulerAnglesRad[0] = std::atan2(sinPart, cosPart);
}

void ST_geom::eulerY0X1Z0(const double (&origin)[3], const double (&aimpoint)[3],
                    const double &zRotDeg, double (&eulerAnglesRad)[3])
{
  eulerAnglesRad[2] = zRotDeg * radPerDeg;
  double z0 = aimpoint[0] - origin[0],
      z1 = aimpoint[1] - origin[1],
      z2 = aimpoint[2] - origin[2];
  double oneOnR2 = 1. / std::sqrt(z0 * z0 + z1 * z1 + z2 * z2);
  z0 *= oneOnR2;
  z1 *= oneOnR2;
  z2 *= oneOnR2;
  double sinBeta = z1, oneOnCosBeta;
  eulerAnglesRad[1] = std::asin(sinBeta);
  oneOnCosBeta = 1. / std::cos(sinBeta);
  double cosPart = z2 * oneOnCosBeta,
      sinPart = z0 * oneOnCosBeta;
  eulerAnglesRad[0] = std::atan2(sinPart, cosPart);
}

void ST_geom::eulerY0X1Z1(const double (&origin)[3], const double (&aimpoint)[3],
                    const double &zRotDeg, double (&eulerAnglesRad)[3])
{
  eulerAnglesRad[2] = zRotDeg * radPerDeg;
  double z0 = aimpoint[0] - origin[0],
      z1 = aimpoint[1] - origin[1],
      z2 = aimpoint[2] - origin[2];
  double oneOnR2 = 1. / std::sqrt(z0 * z0 + z1 * z1 + z2 * z2);
  z0 *= oneOnR2;
  z1 *= oneOnR2;
  z2 *= oneOnR2;
  double sinBeta = z1, oneOnCosBeta;
  eulerAnglesRad[1] = std::asin(sinBeta);
  oneOnCosBeta = 1. / std::cos(sinBeta);
  double cosPart = z2 * oneOnCosBeta,
      sinPart = z0 * oneOnCosBeta;
  eulerAnglesRad[0] = std::atan2(sinPart, cosPart);
}

void ST_geom::eulerYXZ(const gp_Ax2 ax, double (&eulerAnglesRad)[3])
{
  // If z direction is at (0,1,0) or (0,-1,0) then formulae fail.
  // Then set alpha to 0 and solve again.

  double a, b;
  int err1, err2, err3;
  // alpha about +Y
  errno = 0;
  a = ax.Direction().X();
  b = ax.Direction().Z();
  eulerAnglesRad[0] = std::atan2(a,b);
  err1 = errno;
  // beta about +X
  errno = 0;
  a = ax.Direction().Y();
  eulerAnglesRad[1] = - std::asin(a);
  err2 = errno;
  // gamma about +Z
  errno = 0;
  a = ax.XDirection().Y();
  b = ax.YDirection().Y();
  eulerAnglesRad[2] = std::atan2(a,b);
  err3 = errno;

  bool failed = (err1 != 0) || (err2 != 0) || (err3 != 0);
  // In MSVC 2012, cmath appears to not set the errno variable.
  failed = (ax.Direction().X() == 0) && (ax.Direction().Z() == 0);
  if (failed) {
    eulerAnglesRad[0] = 0;
    eulerAnglesRad[1] = std::atan2(-ax.Direction().Y(), ax.Direction().Z());
    eulerAnglesRad[2] = std::atan2(-ax.YDirection().X(), ax.XDirection().X());
  }

}
