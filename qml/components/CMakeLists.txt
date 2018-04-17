file(GLOB COMPONENTS_QML_JS_FILES *.qml *.js)

# Make the files visible in the qtcreator tree
add_custom_target(machines-vs-machines_components_QMlFiles ALL SOURCES ${COMPONENTS_QML_JS_FILES})

install(FILES ${COMPONENTS_QML_JS_FILES} DESTINATION ${MACHINES-VS-MACHINES_DIR}/components)

