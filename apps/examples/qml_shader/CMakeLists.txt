set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS QuickWidgets Widgets)

if(Qt5_FOUND)
  qt5_add_resources(QML_SHADER_QRC qml_shader.qrc OPTIONS --compress 9 --threshold 0)
  set(qml_shader_sources
      main.cpp
      ImageLocation.cpp
      ImageLocation.h
      MainWindow.cpp
      MainWindow.h
      ShaderSource.cpp
      ShaderSource.h
      qml_shader.qrc ${QML_SHADER_QRC}
      ImageView.qml)
  add_executable(qml_shader ${qml_shader_sources})
  target_link_libraries(qml_shader
                        Qt5::QuickWidgets Qt5::Widgets
                        s1)
  if(WIN32)
    target_link_libraries(qml_shader uxtheme.lib)
  endif()
  setup_example(qml_shader)
  # Require C++11 features if set globally
  if(S1_REQUIRED_CXX_FEATURES)
    target_compile_features(qml_shader PRIVATE ${S1_REQUIRED_CXX_FEATURES})
  endif()
elseif(Qt5_NOT_FOUND_MESSAGE)
  message(STATUS "Qt5 not found: ${Qt5_NOT_FOUND_MESSAGE}")
endif()
