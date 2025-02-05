#include "HelloTriangleApplication.h"

int main()
{
    try
    {
		glfwInit();

        {
            zvk::HelloTriangleApplication app{};
            app.Run();
        }

		glfwTerminate();
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
