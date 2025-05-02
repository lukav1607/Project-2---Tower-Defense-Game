cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable(
    "TowerDefenseGame"
    "src/main.cpp"
    "src/core/Game.cpp"
    "src/core/Tile.cpp"
    "src/core/Grid.cpp"
    "src/core/Utility.cpp"
    "src/entities/Tower.cpp"
    "src/entities/Enemy.cpp"
    "src/entities/Entity.cpp"
    "src/ui/UIManager.cpp"
    "src/ui/HUD.cpp"
    "src/ui/TowerBuildMenu.cpp"
    "src/ui/TowerInfoPanel.cpp"
    "src/ui/Button.cpp")
target_compile_features("TowerDefenseGame" PRIVATE cxx_std_17)
target_link_libraries("TowerDefenseGame" PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)
target_compile_definitions("TowerDefenseGame" PRIVATE SFML_STATIC)