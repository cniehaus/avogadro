set(QT_USE_QTOPENGL true)
set(QT_USE_QTNETWORK true)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include(${QT_USE_FILE})

# Ensure the Avogadro include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
  "${libavogadro_BINARY_DIR}/include;${tmp_include_dirs}")
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${EIGEN2_INCLUDE_DIR}
  ${OPENBABEL2_INCLUDE_DIR}
)

# I think this is necessary now in order to tell the linker where openbabel is
link_directories(${OPENBABEL2_LIBRARY_DIRS})

file(GLOB libavogadro_SRCS "*.cpp")

# Only headers with exported symbols should actually be installed
set(libavogadro_HDRS
  animation.h
  atom.h
  bond.h
  camera.h
  color.h
  colorbutton.h
  cube.h
  elementtranslator.h
  engine.h
  extension.h
  filetreeitem.h
  fragment.h
  glgraphicsview.h
  glwidget.h
  global.h
  idlist.h
  plotaxis.h # dlonie: Are these necessary here?
  plotobject.h
  plotpoint.h
  plotwidget.h
  line.h
  mesh.h
  meshgenerator.h
  molecule.h
  moleculefile.h
  navigate.h
  neighborlist.h
  openbabelwrapper.h
  painter.h
  painterdevice.h
  periodictableview.h
  plugin.h
  pluginmanager.h
  point.h
  primitive.h
  primitiveitemmodel.h
  primitivelist.h
  protein.h
  residue.h
  tool.h
  toolgroup.h
  undosequence.h
  zmatrix.h
)

set(libavogadro_SRCS ${libavogadro_SRCS} ${libavogadro_UIS})
# Also add in the qtiocompressor class
#set(libavogadro_SRCS ${libavogadro_SRCS} ../qtiocompressor/qtiocompressor.cpp)

# Statically link the core engines and tools so that users have a working
# system even if no plugins can be found.
set(bsdyengine_SRCS engines/bsdyengine.cpp)
qt4_wrap_ui(bsdyengine_UIS engines/bsdysettingswidget.ui)
set(libavogadro_SRCS ${libavogadro_SRCS} ${bsdyengine_SRCS} ${bsdyengine_UIS})

set(elementcolor_SRCS colors/elementcolor.cpp)
set(libavogadro_SRCS ${libavogadro_SRCS} ${elementcolor_SRCS})

set(AVO_LINK_LIBRARIES ${OPENBABEL2_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${OPENGL_LIBRARIES}
                       )

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
  link_directories(${GLEW_LIBRARY_DIR})
  set(AVO_LINK_LIBRARIES ${AVO_LINK_LIBRARIES} ${GLEW_LIBRARY})
endif(GLEW_FOUND)

# If python dependencies aren't found, remove the Avogadro Python source files
if(NOT ENABLE_PYTHON OR NOT ALL_PYTHON_FOUND)
  set(PYTHON_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterpreter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscript.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythontool_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonengine_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonextension_p.cpp)
  foreach(P_ITEM ${PYTHON_SRCS})
    list(REMOVE_ITEM libavogadro_SRCS "${P_ITEM}")
  endforeach(P_ITEM ${PYTHON_SRCS})
endif(NOT ENABLE_PYTHON OR NOT ALL_PYTHON_FOUND)

qt4_automoc(${libavogadro_SRCS})

# you have to add link_directories before you add the target
if(ENABLE_PYTHON AND ALL_PYTHON_FOUND)
  include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_PATH})
  qt4_add_resources(pythontool_RC_SRCS pythontool.qrc)
  # The Python headers, only installed if Python is enabled
  set(python_HDRS
    pythoninterpreter.h
    pythonscript.h
    pythonerror.h
  )

  message(STATUS "Setting new boost python libraries")
  set(AVO_LINK_LIBRARIES ${AVO_LINK_LIBRARIES} ${PYTHON_LIBRARIES}
                         ${Boost_LIBRARIES})
  set(libavogadro_HDRS ${libavogadro_HDRS} ${python_HDRS})
endif(ENABLE_PYTHON AND ALL_PYTHON_FOUND)

# Set up the include directory and symlink/copy all headers to it.
# Note: Windows lacks symlinks and so the files must be copied.
make_directory("${libavogadro_BINARY_DIR}/include/avogadro")
if(UNIX)
  set(header_cmd "create_symlink")
else(UNIX)
  set(header_cmd "copy_if_different")
endif(UNIX)
foreach(headerFile ${libavogadro_HDRS})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${headerFile}")
  set(to   "${libavogadro_BINARY_DIR}/include/avogadro/${headerFile}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach(headerFile ${libavogadro_HDRS})

add_library(avogadro SHARED ${libavogadro_SRCS} ${libavogadro_QM} ${pythontool_RC_SRCS})
set_target_properties(avogadro
  PROPERTIES VERSION ${Avogadro_VERSION_FULL} SOVERSION 0 )
target_link_libraries(avogadro ${AVO_LINK_LIBRARIES})

install(TARGETS avogadro DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${libavogadro_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${libavogadro_QM} DESTINATION ${I18N_INSTALL_DIR})

# Add the engiens, tools and extensions directories
add_subdirectory(engines)
add_subdirectory(tools)
add_subdirectory(extensions)
add_subdirectory(colors)

# Add the boost python module if the boost::python was found
if(ENABLE_PYTHON AND ALL_PYTHON_FOUND)
  add_subdirectory(python)
endif(ENABLE_PYTHON AND ALL_PYTHON_FOUND)
