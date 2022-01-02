cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)
project(PendulumPainter)

find_package(VTK
  COMPONENTS
    CommonCore
    GUISupportQt
    InfovisCore
    RenderingFreeType
    RenderingAnnotation
    ViewsQt)
if (NOT VTK_FOUND)
  message("Skipping example: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif ()

find_package(Qt5 COMPONENTS Widgets Gui)
if (NOT TARGET Qt5::Widgets OR NOT TARGET Qt5::Gui)
  message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Set your files and resources here
set(Srcs
  PendulumPainter.cxx
  PendulumPainter_Test.cxx
  main.cxx)

set(Hdrs
  PendulumPainter.h
  PendulumPainter_Test.h)

set(UIs
  PendulumPainter.ui)

set(QRCs
  Icons/icons.qrc)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

source_group("Resources" FILES
  ${UIs}
  ${QRCs})

add_executable(PendulumPainter MACOSX_BUNDLE
  ${Srcs} ${Hdrs} ${UIs} ${QRCs})
  	  
target_link_libraries(PendulumPainter
  PRIVATE
    ${VTK_LIBRARIES}
    Qt5::Gui
    Qt5::Widgets)
	
vtk_module_autoinit(
    TARGETS PendulumPainter
    MODULES ${VTK_LIBRARIES})
