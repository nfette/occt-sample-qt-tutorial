#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T05:09:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SolTraceCloneExe
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../SolTraceCloneDLL

CONFIG(debug, debug|release) {
#build_pass:CONFIG(debug,debug|release) {
  LIBS += -L../build-SolTraceCloneDLL-Desktop_Qt_5_1_0_MSVC2012_OpenGL_64bit-Debug/debug -lSolTraceCloneDLL
  PRE_TARGETDEPS += ../build-SolTraceCloneDLL-Desktop_Qt_5_1_0_MSVC2012_OpenGL_64bit-Debug/debug/SolTraceCloneDLL.lib
} else {
  LIBS += -L../build-SolTraceCloneDLL-Desktop_Qt_5_1_0_MSVC2012_OpenGL_64bit-Release/release -lSolTraceCloneDLL
  PRE_TARGETDEPS += ../build-SolTraceCloneDLL-Desktop_Qt_5_1_0_MSVC2012_OpenGL_64bit-Release/release/SolTraceCloneDLL.lib
}

# Extra junk required for OpenCascade.

#QMAKE_TARGET.arch = x86_64

DEFINES += CSFDB

CONFIG(debug, debug|release) {
  DEFINES += _DEBUG
} else {
  DEFINES += NDEBUG
}

PRODUCTS_PATH = "C:/OpenCASCADE6.6.0/3rdparty"
INCLUDEPATH += "C:/OpenCASCADE6.6.0/ros/inc" \
               "C:/Tcl/include" \
               "$${PRODUCTS_PATH}/freetype-2.5.0.1-vc11-64/include" \
               "$${PRODUCTS_PATH}/freeimage-vc11-64/include" \
               "$${PRODUCTS_PATH}/gl2ps-1.3.8-vc11-64/include" \
               "$${PRODUCTS_PATH}/tbb42_20130725oss/include"

LIBS += -L"C:/Tcl/lib" \
        -L"$${PRODUCTS_PATH}/freetype-2.5.0.1-vc11-64/lib" \
        -L"$${PRODUCTS_PATH}/freeimage-vc11-64/lib" \
        -L"$${PRODUCTS_PATH}/gl2ps-1.3.8-vc11-64/lib" \
        -L"$${PRODUCTS_PATH}/tbb42_20130725oss/lib/intel64/vc11" \
        -L"C:/Tcl/bin" \
        -L"$${PRODUCTS_PATH}/freetype-2.5.0.1-vc11-64/bin" \
        -L"$${PRODUCTS_PATH}/freeimage-vc11-64/bin" \
        -L"$${PRODUCTS_PATH}/tbb42_20130725oss/bin/intel64/vc11"

LIBS += -L"C:/OpenCASCADE6.6.0/ros/win64/vc11/lib" \
        -L"C:/OpenCASCADE6.6.0/ros/win64/vc11/bin" \
        -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset \

DEFINES += WNT NO_COMMONSAMPLE_EXPORTS NO_IESAMPLE_EXPORTS
