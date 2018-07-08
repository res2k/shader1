set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS QuickWidgets Widgets)

if(Qt5_FOUND)
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
      models/orderstat_tree/intrusive.h
      )
  add_library(qml_shader_models STATIC EXCLUDE_FROM_ALL
              ${qml_shader_models_sources})
  target_link_libraries(qml_shader_models
                        Qt5::Widgets)
  target_compile_features(qml_shader_models PUBLIC ${S1_REQUIRED_CXX_FEATURES})

  qt5_add_resources(MATERIAL_ICONS_QRC material_icons/MaterialIcons.qrc OPTIONS --compress 9 --threshold 0)
  set_property(SOURCE ${MATERIAL_ICONS_QRC} PROPERTY SKIP_AUTOGEN ON)
  set(qml_shader_material_icons_sources
      material_icons/MaterialIcons.cpp
      material_icons/MaterialIcons.h
      material_icons/MaterialIcons.qrc ${MATERIAL_ICONS_QRC}
      )
  add_library(qml_shader_material_icons STATIC EXCLUDE_FROM_ALL
              ${qml_shader_material_icons_sources})
  target_link_libraries(qml_shader_material_icons
                        Qt5::Gui)

  qt5_add_resources(QML_SHADER_QRC qml_shader.qrc OPTIONS --compress 9 --threshold 0)
  set_property(SOURCE ${QML_SHADER_QRC} PROPERTY SKIP_AUTOGEN ON)
  set(qml_shader_sources
      main.cpp
      AssetBrowsingModel.cpp
      AssetBrowsingModel.h
      AssetBrowsingWidget.cpp
      AssetBrowsingWidget.h
      AssetBrowsingWidget.ui
      CustomItemDataRole.h
      ImageChooserWidget.cpp
      ImageChooserWidget.h
      ImageLocation.cpp
      ImageLocation.h
      MainWindow.cpp
      MainWindow.h
      ShaderChooserWidget.cpp
      ShaderChooserWidget.h
      ShaderSource.cpp
      ShaderSource.h
      qml_shader.qrc ${QML_SHADER_QRC}
      ImageView.qml)
  add_executable(qml_shader ${qml_shader_sources})
  set_target_properties(qml_shader PROPERTIES AUTOUIC ON)
  target_link_libraries(qml_shader
                        qml_shader_models
                        qml_shader_material_icons
                        Qt5::QuickWidgets Qt5::Widgets
                        s1)
  if(WIN32)
    target_link_libraries(qml_shader uxtheme.lib)
  endif()
  setup_example(qml_shader)
  target_compile_features(qml_shader PRIVATE ${S1_REQUIRED_CXX_FEATURES})
elseif(Qt5_NOT_FOUND_MESSAGE)
  message(STATUS "Qt5 not found: ${Qt5_NOT_FOUND_MESSAGE}")
endif()

if(CXXTEST_FOUND)
  set(orderstat_tree_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/models/orderstat_tree/t/orderstat_tree.h)
  S1_CXXTEST_ADD_TEST(test_orderstat_tree test_orderstat_tree.cpp
                      ${orderstat_tree_test_sources})
  set_property(SOURCE test_orderstat_tree.cpp PROPERTY SKIP_AUTOGEN ON)
  target_include_directories(test_orderstat_tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/models)
  set_property(TARGET test_orderstat_tree PROPERTY CXX_STANDARD 11)
  set_property(TARGET test_orderstat_tree PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_orderstat_tree)
endif()
