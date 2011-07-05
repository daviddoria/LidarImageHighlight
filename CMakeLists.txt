cmake_minimum_required(VERSION 2.6)

set(VTKAdditions /media/portable/VTK_Additions)
#INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${VTKAdditions} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(/media/portable/src/github/vtkPTXReader)

PROJECT(LidarImageHighlight)

FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Set your files and resources here
SET(LidarImageHighlightSrcs LidarImageHighlight.cpp SimpleView.cxx
/media/portable/src/github/vtkPTXReader/vtkPTXReader.cxx
)
SET(SimpleViewUI SimpleView.ui)
SET(SimpleViewHeaders SimpleView.h)

QT4_WRAP_UI(UISrcs ${SimpleViewUI})
QT4_WRAP_CPP(MOCSrcs ${SimpleViewHeaders} )

SOURCE_GROUP("Resources" FILES
  ${SimpleViewUI}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

ADD_EXECUTABLE(LidarImageHighlight ${LidarImageHighlightSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})
TARGET_LINK_LIBRARIES(LidarImageHighlight QVTK)

