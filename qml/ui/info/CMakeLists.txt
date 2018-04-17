file(GLOB UI_INFO_QML_JS_FILES *.qml *.js)

# Make the files visible in the qtcreator tree
add_custom_target(machines-vs-machines_ui_info_QMlFiles ALL SOURCES ${UI_INFO_QML_JS_FILES})

install(FILES ${UI_INFO_QML_JS_FILES} DESTINATION ${MACHINES-VS-MACHINES_DIR}/ui/info)

