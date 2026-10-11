project(Rock3dEngine)

if(NOT WIN32 AND RRR3D_ENABLE_RENDERER AND
   NOT RRR3D_RENDERER_BACKEND STREQUAL "bgfx")
    message(FATAL_ERROR
        "The non-Windows renderer requires RRR3D_RENDERER_BACKEND=bgfx")
endif()

if(WIN32)
    list(APPEND PROJECT_DIRS
        "header"
        "header/graph"
        "header/graph/Driver"
        "header/gui"
        "header/include"
        "header/include/graph"
        "header/px"
        "header/resource"
        "header/res"
        "source"
        "source/graph"
        "source/graph/Driver"
        "source/gui"
        "source/px"
        "source/resource"
        "source/res"
    )
    add_dir("${PROJECT_DIRS}" "${PROJECT_NAME}")

    list(REMOVE_ITEM ${PROJECT_NAME}_HEADER
        "${CMAKE_CURRENT_SOURCE_DIR}/header/graph/GaussianBlur.h"
    )
    list(REMOVE_ITEM ${PROJECT_NAME}_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/source/graph/GaussianBlur.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/res/R3DFile.cpp"
    )
else()
    # Portable core that does not manufacture fake D3D or PhysX objects.
    # Renderer/physics entry points remain explicitly unavailable until their
    # own milestones provide real implementations.
    set(${PROJECT_NAME}_SOURCE
        source/ProgressTimer.cpp
        source/resource/R3DMeshAsset.cpp
        source/resource/ResourceFileSystem.cpp
        source/stub/PortableEngine.cpp
    )
    set(${PROJECT_NAME}_HEADER
        header/ProgressTimer.h
        header/PortableEngine.h
        header/audio/AudioBackend.h
        header/resource/R3DMeshAsset.h
        header/resource/ResourceFileSystem.h
        header/stdafx.h
    )

    if(RRR3D_ENABLE_RENDERER AND RRR3D_RENDERER_BACKEND STREQUAL "bgfx")
        list(APPEND ${PROJECT_NAME}_SOURCE
            source/renderer/bgfx/BgfxGraphicsDevice.cpp
        )
        list(APPEND ${PROJECT_NAME}_HEADER
            header/renderer/Renderer.h
            header/renderer/BgfxGraphicsDevice.h
        )
    endif()

    if(RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
        list(APPEND ${PROJECT_NAME}_SOURCE
            source/physics/JoltVehiclePhysics.cpp
        )
        list(APPEND ${PROJECT_NAME}_HEADER
            header/physics/OriginalVehiclePhysics.h
        )
    elseif(RRR3D_ENABLE_PHYSICS)
        list(APPEND ${PROJECT_NAME}_SOURCE
            source/physics/MinimalVehiclePhysics.cpp
        )
        list(APPEND ${PROJECT_NAME}_HEADER
            header/physics/PhysicsBackend.h
        )
    endif()
endif()

add_library(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SOURCE}
    ${${PROJECT_NAME}_INLINE}
    ${${PROJECT_NAME}_HEADER}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    header
)

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/extern/physx/include/Cooking
        ${CMAKE_SOURCE_DIR}/extern/physx/include/Foundation
        ${CMAKE_SOURCE_DIR}/extern/physx/include/Physics
        ${CMAKE_SOURCE_DIR}/extern/physx/include/PhysXLoader
        ${CMAKE_SOURCE_DIR}/extern/directx/include
    )
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        RRR3D_PORTABLE_ENGINE_STUBS=1
    )

    if(RRR3D_ENABLE_RENDERER AND RRR3D_RENDERER_BACKEND STREQUAL "bgfx")
        target_compile_definitions(${PROJECT_NAME} PUBLIC
            RRR3D_RENDERER_BGFX=1
        )
        target_link_libraries(${PROJECT_NAME} PUBLIC rrr3d_bgfx)
    endif()

    if(RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
        target_compile_definitions(${PROJECT_NAME} PUBLIC
            RRR3D_PHYSICS_JOLT=1
        )
        target_link_libraries(${PROJECT_NAME} PUBLIC ${RRR3D_JOLT_TARGET})
    elseif(RRR3D_ENABLE_PHYSICS)
        target_compile_definitions(${PROJECT_NAME} PUBLIC
            RRR3D_PHYSICS_MINIMAL=1
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    LexStd
    MathLib
    XPlatform
)

rrr3d_set_common_target_options(${PROJECT_NAME})

# Headless diagnostic uses the very same Jolt vehicle adapter as the game.
if(NOT WIN32 AND RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
    add_executable(rrr3d_physics_calibration ../../tools/calibration/mac.cpp)
    target_link_libraries(rrr3d_physics_calibration PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_physics_calibration)
endif()
if(NOT WIN32 AND RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
    add_executable(rrr3d_calibration_tuning_test ../../tools/calibration/test_tuning.cpp)
    target_link_libraries(rrr3d_calibration_tuning_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_calibration_tuning_test)
    add_test(NAME calibration_tuning COMMAND rrr3d_calibration_tuning_test
        "${CMAKE_CURRENT_BINARY_DIR}/calibration-test.cfg")
endif()

if(NOT WIN32 AND RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
    add_executable(rrr3d_source_inertia_test ../../tools/calibration/test_inertia.cpp)
    target_link_libraries(rrr3d_source_inertia_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_source_inertia_test)
    add_test(NAME source_inertia COMMAND rrr3d_source_inertia_test
        "${CMAKE_SOURCE_DIR}/tools/calibration/reference-inertia.txt")
endif()

if(NOT WIN32 AND RRR3D_ENABLE_PHYSICS AND RRR3D_BUILD_ORIGINAL_MENU)
    add_executable(rrr3d_contact_impulse_test ../../tools/calibration/test_contact_impulse.cpp)
    target_link_libraries(rrr3d_contact_impulse_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_contact_impulse_test)
    add_test(NAME solved_contact_impulse COMMAND rrr3d_contact_impulse_test)
    add_executable(rrr3d_suspension_stop_test ../../tools/calibration/test_suspension_stop.cpp)
    target_link_libraries(rrr3d_suspension_stop_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_suspension_stop_test)
    add_test(NAME source_suspension_stop COMMAND rrr3d_suspension_stop_test)
    add_executable(rrr3d_wall_timing_test ../../tools/calibration/test_wall_timing.cpp)
    target_link_libraries(rrr3d_wall_timing_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_wall_timing_test)
    add_test(NAME source_wall_contact_timing COMMAND rrr3d_wall_timing_test)
    add_executable(rrr3d_contact_skin_targets_test ../../tools/calibration/test_contact_skin_targets.cpp)
    target_link_libraries(rrr3d_contact_skin_targets_test PRIVATE Rock3dEngine)
    add_test(NAME rrr3d_contact_skin_targets_test COMMAND rrr3d_contact_skin_targets_test)
    add_executable(rrr3d_frozen_pose_test ../../tools/calibration/test_frozen_pose.cpp)
    target_link_libraries(rrr3d_frozen_pose_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_frozen_pose_test)
    add_test(NAME source_frozen_pose COMMAND rrr3d_frozen_pose_test
        "${CMAKE_CURRENT_BINARY_DIR}/frozen-pose-test.cfg")
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME source_box_contact_reference COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/tools/calibration/test_box_manifolds.py"
            $<TARGET_FILE:rrr3d_physics_calibration>
            "${CMAKE_SOURCE_DIR}/tools/calibration/probes/reference-box-contact-v14.json")
    endif()
    # Independent face-generation probe. The experimental solver is not
    # included in the production adapter until matrix regressions are fixed.
    add_executable(rrr3d_mesh_geometry_probe ../../tools/calibration/experimental/mesh_probe.cpp)
    rrr3d_set_common_target_options(rrr3d_mesh_geometry_probe)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(rrr3d_mesh_geometry_probe PRIVATE -ffp-contract=off)
    endif()
    if(Python3_Interpreter_FOUND)
        add_test(NAME source_mesh_first_geometry COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/tools/calibration/experimental/test_mesh_geometry.py"
            $<TARGET_FILE:rrr3d_mesh_geometry_probe>
            "${CMAKE_SOURCE_DIR}/tools/calibration/experimental/reference-mesh-first-v15.json")
    endif()
    # Short isolated probes share the production adapter. The bias patch is
    # experimental and must be applied separately before enabling its env switch.
    add_executable(rrr3d_physics_spring_probe ../../tools/calibration/mac.cpp)
    target_compile_definitions(rrr3d_physics_spring_probe PRIVATE MOTORROCK_EXPERIMENT_SPRING_PROBES=1)
    target_link_libraries(rrr3d_physics_spring_probe PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_physics_spring_probe)
    if(Python3_Interpreter_FOUND)
        add_test(NAME source_spring_reference COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/tools/calibration/experimental/test_spring_reference.py"
            "${CMAKE_SOURCE_DIR}/tools/calibration/experimental/reference-spring-first-v16.json")
    endif()
    option(RRR3D_BUILD_EXPERIMENTAL_SPRING_ABI_TEST "Test manually applied experimental Jolt bias patch" OFF)
    if(RRR3D_BUILD_EXPERIMENTAL_SPRING_ABI_TEST)
        add_executable(rrr3d_spring_serialization_test ../../tools/calibration/experimental/test_spring_serialization.cpp)
        target_link_libraries(rrr3d_spring_serialization_test PRIVATE Jolt)
        rrr3d_set_common_target_options(rrr3d_spring_serialization_test)
        add_test(NAME experimental_spring_serialization COMMAND rrr3d_spring_serialization_test)
    endif()
    add_executable(rrr3d_body_friction_test ../../tools/calibration/test_body_friction.cpp)
    target_link_libraries(rrr3d_body_friction_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_body_friction_test)
    add_test(NAME source_body_friction COMMAND rrr3d_body_friction_test)
    add_executable(rrr3d_fixture_selection_test ../../tools/calibration/test_fixture_selection.cpp)
    target_link_libraries(rrr3d_fixture_selection_test PRIVATE Rock3dEngine)
    rrr3d_set_common_target_options(rrr3d_fixture_selection_test)
    add_test(NAME calibration_fixture_selection COMMAND rrr3d_fixture_selection_test "${CMAKE_CURRENT_BINARY_DIR}/selection-test.jsonl")
endif()
