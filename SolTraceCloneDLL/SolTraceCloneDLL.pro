#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T04:42:24
#
#-------------------------------------------------

TARGET   = SolTraceCloneDLL
TEMPLATE = lib
QT       += core gui widgets
CONFIG   += staticlib

SOURCES  += st*.cpp \
    widget.cpp \
    nx_mesh.cpp \
    nx_tri3.cpp

HEADERS  += st*.h \
    widget.h \
    nx_mesh.h \
    nx_tri3.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
        #QMAKE_CXXFLAGS += /EHsc
}

FORMS   += widget.ui

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
        -L"$${PRODUCTS_PATH}/tbb42_20130725oss/lib/intel64/vc11"

LIBS += -L"C:/OpenCASCADE6.6.0/ros/win64/vc11/lib" \
        -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset \

DEFINES += WNT NO_COMMONSAMPLE_EXPORTS NO_IESAMPLE_EXPORTS
