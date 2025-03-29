#include "HelloTriangleApplication.h"

int main(int argc, char *argv[])
{
    std::filesystem::path shaderPath{};
    std::filesystem::path texturePath{};

    if (argc > 0 && argv[0])
    {
        std::filesystem::path executablePath{argv[0]};
        executablePath.remove_filename();
        shaderPath = executablePath / "shaders";
        texturePath = executablePath / "textures";
    }

    try
    {
        zvk::HelloTriangleApplication app{shaderPath, texturePath};
        app.Run();
    }

    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
#ifdef _WIN32
        OutputDebugStringA(e.what());
        OutputDebugString(L"\n");
#endif

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
