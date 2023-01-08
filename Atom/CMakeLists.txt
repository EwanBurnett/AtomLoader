set(ATOM_SOURCES "src/Model.cpp" "src/Importer.cpp" "src/Exporter.cpp")
set(ATOM_HEADERS "inc/Importer.h" "inc/Model.h" "inc/Types.h" "inc/Exporter.h")


add_executable(${PROJECT_NAME} "src/main.cpp" ${ATOM_SOURCES} ${ATOM_HEADERS})
#add_library(${LIB_NAME} STATIC ${ATOM_SOURCES} ${ATOM_HEADERS})
