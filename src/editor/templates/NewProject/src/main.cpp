/**
 * Coconuts2D entrypoint
 * Jan 2026
 */

#include <coconuts2D/Application.h>
#include <coconuts2D/ResourceManager.h>
#include <string>

 
int main(int argc, char* argv[])
{
    coconuts2D::Application app( static_cast<std::string>(argv[0]));

    //TODO read gameDesc from runtime
    std::string gameDesc = "/Users/temprilho/dev/Projects/Coconuts2DGame/res/desc.yaml";
    coconuts2D::ResourceManager rm(gameDesc);
    rm.LoadScene(1);

    app.Play();
    return 0;
}

