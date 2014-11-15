//#include "widget.h"

#include <cmath>
#include <iomanip>
#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include <st_geom.h>
#include <nx_mesh.h>
#include <st_raydb.h>
#include <st_postprocess.h>

void help(char * myname);
void check_euler_angles();
void check_import_inpf(QString inpf_file, QString stage_file);
void check_make_vtk(QString inpf_file, QString ray_file, double raypower, int stageIndex, QString vtk_file);

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  //check_euler_angles();

  if (argc == 5) {
    // Generate a stage file.
    //QString inpf_file("C:/Users/nfette/Documents/swsllc/mirror_analysis/nx_meshes/INPF");
    //QString stage_file("C:/Users/nfette/Documents/swsllc/mirror_analysis/nx_meshes/inpf.stage");
    QString inpf_file = argv[2];
    QString stage_file = argv[4];
    check_import_inpf(inpf_file, stage_file);

  } else if (argc == 11) {
    // -inpf nx_meshes/INPF -rays results/mesh-rays.csv -raypower 1 -index 16 -vtk results/mesh-rays.vtk
    //QString inpf_file("C:/Users/nfette/Documents/swsllc/mirror_analysis/nx_meshes/INPF");
    //QString ray_file("C:/Users/nfette/Documents/swsllc/mirror_analysis/results/mesh-rays.csv");
    //QString vtk_file("C:/Users/nfette/Documents/swsllc/mirror_analysis/results/mesh-rays.vtk");
    //int stageIndex = 16;
    QString inpf_file = argv[2];
    QString ray_file = argv[4];
    double raypower = QString(argv[6]).toDouble();
    QString stageIndexString = argv[8];
    QString vtk_file = argv[10];
    int stageIndex = stageIndexString.toInt();
    check_make_vtk(inpf_file,ray_file,raypower,stageIndex,vtk_file);

  } else {
    QString full_name = argv[0];
    QFileInfo info(full_name);
    help(info.fileName().toLatin1().data());
  }

  // nothing to do
  // return a.exec();
  return 0;
}

void help(char *myname) {
  std::cout << "Usage: " << std::endl;
  std::cout << myname << " --help" << std::endl;
  std::cout << "  Generate this helpful message." << std::endl;
  std::cout << std::endl;
  std::cout << myname << " -inpf <file.inpf> -stage <file.stage>" << std::endl;
  std::cout << "  Read the NX mesh in <file.inpf> and " << std::endl
            << "  output a Soltrace stage in <file.stage>." << std::endl
            << "  You can then open a Soltrace project and import the stage." << std::endl;
  std::cout << std::endl;
  std::cout << myname << " -inpf <file.inpf> -rays <file.csv> -raypower <double> -index <n> -vtk <file.vtk>" << std::endl;
  std::cout << "  Read the NX mesh and Soltrace ray database, then " << std::endl
            << "  assuming stage at index n (starting from 0) is a " << std::endl
            << "  mesh stage generated from file.inpf, create a" << std::endl
            << "  file.vtk to display the ray trace results on the mesh." << std::endl
            << "  You can then open the file.vtk in ParaView." << std::endl
            << "  (Note that in SolTrace GUI the stages have an ID# starting from 1;" << std::endl
            << "  I am asking for index = ID - 1.)" << std::endl;
  std::cout << std::endl;
  std::cout << "Warning: I do not really read the -word tags, just follow the blueprint." << std::endl;
  std::cout << "Done." << std::endl;
}

// Read a mesh and a ray file, and write a VTK file for post-processing.
void check_make_vtk(QString inpf_file, QString ray_file, double raypower, int stageIndex, QString vtk_file)
{
  nx_mesh aMesh;
  ST_raydb aRayDB;

  if (aMesh.readINPF(inpf_file) && aRayDB.readCSV(ray_file)) {
    aRayDB.powerPerRay = raypower;
    qDebug() << "Read the mesh input file: " << inpf_file << endl;
    qDebug() << "Read the ray database: " << ray_file << endl;
    ST_postprocess aPost(&aMesh, &aRayDB, stageIndex);
    aPost.computeStats();
    if (aPost.writeVtk(vtk_file)) {
      qDebug() << "Wrote vtk file to " << vtk_file << endl;
    } else {
      qDebug() << "Error: couldn't write vtk file: " << vtk_file << endl;
    }
  } else {
    qDebug() << "Error reading mesh input or ray database." << endl;
  }
  std::cout << "Done." << std::endl;
}

// Read a mesh, create a stage, and write it to a stage file.
void check_import_inpf(QString inpf_file, QString stage_file)
{

  nx_mesh aMesh;
  if (aMesh.readINPF(inpf_file)) {
    std::cout << "Read the mesh input file." << std::endl;
    ST_stage aStage = aMesh.toStage();
    std::cout << "Created a stage without problems." << std::endl;
    if (aStage.writeFile(stage_file, 1)) {
      std::cout << "Wrote the stage to file without problems." << std::endl;
    }
  }
  std::cout << "Done." << std::endl;
}

void check_euler_angles()
{
  std::cout << "The answer is blowing in the wind ..." << std::endl;
  std::cout << ST_geom::pi << std::endl;
  std::cout << ST_geom::degPerRad << std::endl;
  std::cout << ST_geom::radPerDeg << std::endl;
  double origin[3] = {0,0,0};
  double aimpoint[3];
  //double zRotDeg = 0;
  double eulerAnglesRad[3];
  double alpha, beta, gamma = 0;
  int nu = 7, nv = 7;
  for (int i = 0; i <= nu; i++) {
    alpha = 0.1 * ST_geom::pi * i;
    for (int j = 0; j <= nv; j++) {
      beta = 0.1 * ST_geom::pi * j;
      aimpoint[0] = std::sin(alpha) * std::cos(beta);
      aimpoint[1] = std::sin(beta);
      aimpoint[2] = std::cos(alpha) * std::cos(beta);
      ST_geom::eulerY0X1Z0(origin, aimpoint, gamma, eulerAnglesRad);
      std::cout
          << std::fixed
          << std::setw(14) << ST_geom::degPerRad * alpha << " "
          << std::setw(14) << ST_geom::degPerRad * beta << " "
          << std::setw(14) << ST_geom::degPerRad * gamma << " "
          << std::setw(14) << aimpoint[0] << " "
          << std::setw(14) << aimpoint[1] << " "
          << std::setw(14) << aimpoint[2] << " "
          << std::endl;
      std::cout
          << std::setw(14) << ST_geom::degPerRad * eulerAnglesRad[0] << " "
          << std::setw(14) << ST_geom::degPerRad * eulerAnglesRad[1] << " "
          << std::setw(14) << ST_geom::degPerRad * eulerAnglesRad[2] << " "
          << std::endl;
      std::cout
          << "============== ============== ============== ============== ============== =============="
          << std::endl;
    }
  }
}
