/**
 * Coconuts2D entrypoint
 * Jan 2026
 */

#include <coconuts2D/Application.h>
#include <coconuts2D/ResourceManager.h>
#include <string>

 
int main(int argc, char* argv[])
{
    // Set LUA_PATH to include Coconuts2D Lua API scripts
    std::string lua_api_dir("/Users/temprilho/dev/Coconuts2D/scripting");
    std::string env_lua_path = "$LUA_PATH;" + lua_api_dir + "/?.lua";
    (void) setenv("LUA_PATH", env_lua_path.c_str(), 1);

    coconuts2D::Application app( static_cast<std::string>(argv[0]));
    coconuts2D::ResourceManager rm;
    rm.LoadVirtualFS();

    app.Play();
    return 0;
}

