#include "MainComponent.h"
#include "Block.h"
#include <fstream>

//==============================================================================
MainComponent::MainComponent()
{
    m_NrSelectedFiles = 0;
    m_FileNumber = 0;

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setFramesPerSecond (60); // This sets the frequency of the update calls.

    addAndMakeVisible(m_DataTable);
    m_pTableModel = new TableModel();
    m_DataTable.setModel(m_pTableModel);
    m_DataTable.getHeader().addColumn("Name", 1, 100, 50, 200);
    m_DataTable.getHeader().addColumn("x", 2, 50, 20, 60);
    m_DataTable.getHeader().addColumn("y", 3, 50, 20, 60);
    m_DataTable.getHeader().addColumn("z", 4, 50, 20, 60);

    m_InputButton.setButtonText("Select input file");
    addAndMakeVisible(m_InputButton);
    m_InputButton.onClick = [this]()
    {
        SelectFile("*.json",m_InputPath);
    };

    m_OutputButton.setButtonText("Select output path");
    addAndMakeVisible(m_OutputButton);
    m_OutputButton.onClick = [this]()
    {
        SelectFile(String(), m_OutputPath);
    };
    m_OutputButton.setEnabled(false);

    m_CancelButton.setButtonText("Cancel");
    addAndMakeVisible(m_CancelButton);
    m_CancelButton.onClick = [this]()
    {
        ClearFiles();
    };
   
    m_OkButton.setButtonText("Convert");
    addAndMakeVisible(m_OkButton);
    m_OkButton.setEnabled(false);
    m_OkButton.onClick = [this]()
    {
        std::wstring outputPath = L"\\output" + std::to_wstring(m_FileNumber) + L".obj";
        m_OutputPath.append(outputPath);
        Block::JsonToOBJ(m_InputPath, m_OutputPath);
        PrintObj(m_OutputPath);
        ++m_FileNumber;
    };
}

MainComponent::~MainComponent()
{
    delete m_pTableModel;
}

//==============================================================================
void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    m_DataTable.setBounds(0, 0, getWidth(), getHeight());

    constexpr int height = 30;
    const int width = getWidth() - 20;
    constexpr int x = 10;
    int compHeight = getHeight();

    m_InputButton.setBounds(x, compHeight - (height + 10), width, height);
    m_OutputButton.setBounds(x, compHeight - 2 * (height + 10), width, height);
    m_CancelButton.setBounds(x + width / 2, compHeight - 3 * (height + 10), width / 2, height);
    m_OkButton.setBounds(x, compHeight - 3 * (height + 10), width / 2, height );
}

void MainComponent::SelectFile(const String& filePattern , std::wstring& pathToSave)
{
    using namespace juce;
    m_pInputChooser = std::make_unique<FileChooser>(
        "Please select the obj file you want to load...",
        File::getSpecialLocation(File::userHomeDirectory),
        filePattern);
    auto folderChooserFlags =
        FileBrowserComponent::openMode |
        FileBrowserComponent::canSelectFiles |
        FileBrowserComponent::canSelectDirectories;
    m_pInputChooser->launchAsync(folderChooserFlags,
        [&](const FileChooser& chooser)
        {
            juce::Logger::getCurrentLogger()->writeToLog("File selected");
            File file = chooser.getResult();
            if (file.exists())
            {
                juce::String filePath= file.getFullPathName();
                
                if (filePath.isNotEmpty())
                {
                    pathToSave = filePath.toWideCharPointer();
                    ++m_NrSelectedFiles;

                    if (m_NrSelectedFiles == 1)
                    {
                        //Enable output selector
                        m_OutputButton.setEnabled(true);
                    }
                    else if (m_NrSelectedFiles == 2)
                    {
                        //enable output button
                        m_OkButton.setEnabled(true);
                    }
                }
            }
        }
    );
}

void MainComponent::ClearFiles()
{
    m_InputPath.clear();
    m_OutputPath.clear();
    m_NrSelectedFiles = 0;

    m_OutputButton.setEnabled(false);
    m_OkButton.setEnabled(false);

    m_pTableModel->Empty();
}

void MainComponent::PrintObj(const std::wstring& path)
{
    m_pTableModel->Empty();

    if (std::ifstream is{ path })
    {
        std::string line{};
        NamedVector3 vec{};
        while (!is.eof())
        {
           
            is >> line;
            if (line == "v")
            {
                is >> vec.x >> vec.y >> vec.z;
                vec.name = "Vertex";
                m_pTableModel->AddRow(std::move(vec));
            }
            else if (line == "vn")
            {
                is >> vec.x >> vec.y >> vec.z;
                vec.name = "Normal";
                m_pTableModel->AddRow(std::move(vec));
            }

        }
    }

    m_DataTable.updateContent();
   
}
