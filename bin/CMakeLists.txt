# find_package(cpr CONFIG REQUIRED)

add_executable(weatherFrts main.cpp)

target_link_libraries(weatherFrts PRIVATE cpr::cpr)
target_link_libraries(weatherFrts PRIVATE nlohmann_json::nlohmann_json)