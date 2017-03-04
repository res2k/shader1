set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS QuickWidgets Widgets)

if(Qt5_FOUND)
  qt5_add_resources(QML_SHADER_QRC qml_shader.qrc OPTIONS --compress 9 --threshold 0)

  set(qml_shader_models_sources
      models/CombiningItemModel.cpp
      models/CombiningItemModel.h
      models/CustomDataModel.h
      models/FosterModel.cpp
      models/FosterModel.h
      models/ModelHierarchyMirror.h
      models/NodeHierarchy.cpp
      models/NodeHierarchy.h
      models/SubindexModel.cpp
      models/SubindexModel.h
      models/sum_tree.h
      )
  add_library(qml_shader_models STATIC EXCLUDE_FROM_ALL
              ${qml_shader_models_sources})
  target_link_libraries(qml_shader_models
                        Qt5::Widgets)

  set(qml_shader_sources
      main.cpp
      CustomItemDataRole.h
      ImageChooserWidget.cpp
      ImageChooserWidget.h
      ImageChooserWidget.ui
      ImageLocation.cpp
      ImageLocation.h
      MainWindow.cpp
      MainWindow.h
      ShaderSource.cpp
      ShaderSource.h
      qml_shader.qrc ${QML_SHADER_QRC}
      ImageView.qml)
  add_executable(qml_shader ${qml_shader_sources})
  set_target_properties(qml_shader PROPERTIES AUTOUIC ON)
  target_link_libraries(qml_shader
                        qml_shader_models
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
