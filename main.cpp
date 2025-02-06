#include "HelloTriangleApplication.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc > 0 && argv[0])
    {
        std::string executable{argv[0]};
    }
    
    try
    {
        zvk::HelloTriangleApplication app{};
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
