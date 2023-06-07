# Set the source files for the project
set(SOURCES
    Main.cpp
    MainComponent.h
    MainComponent.cpp
)

# Add an executable target for the project
juce_add_gui_app(MinecraftToolJUCEApp
    PRODUCT_NAME "Minecraft Tool JUCE App"
    VERSION ${PROJECT_VERSION}
    COMPANY_NAME "KoenSoft"
    DOCUMENT_EXTENSIONS "myext"
    DOCUMENT_NAMES "My Document"
    DOCUMENT_DESCRIPTIONS "My Document Description"
)

juce_generate_juce_header(MinecraftToolJUCEApp)

target_sources(MinecraftToolJUCEApp PRIVATE ${SOURCES} "TableModel.h" "TableModel.cpp" "NamedVector3.h" "NamedVector3.cpp")

# Link against the JUCE module
target_link_libraries(MinecraftToolJUCEApp
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_include_directories(MinecraftToolJUCEApp PUBLIC "${PROJECT_BINARY_DIR}" "${CommonCodeIncludeDir}")