set(ATOM_SOURCES "src/main.cpp" "src/Model.cpp" "src/Importer.cpp" "src/Exporter.cpp")
set(ATOM_HEADERS "inc/Importer.h" "inc/Model.h" "inc/Types.h" "inc/Exporter.h")


add_executable(${PROJECT_NAME} ${ATOM_SOURCES} ${ATOM_HEADERS})
