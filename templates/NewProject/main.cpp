#include <coconuts2D/Application.h>
#include <string>

int main(int argc, char* argv[])
{
    coconuts2D::Application app( static_cast<std::string>(argv[0]) );
    app.Play();

    return 0;
}
