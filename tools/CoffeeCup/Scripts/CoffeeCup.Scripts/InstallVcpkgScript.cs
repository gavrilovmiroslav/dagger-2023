using System.Diagnostics;
using System.Runtime.InteropServices;
using CoffeeCup.Scripting;

namespace CoffeeCup.Scripts
{
    public class InstallVcpkgScript : IScript
    {
        private static void Bootstrap()
        {
            var bootstrapProcessArgs = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ?
                "cmd.exe bootstrap-vcpkg.bat" :
                "bash bootstrap-vcpkg.sh";
            var bootstrapProcess = ScriptUtil.CreateProcess(bootstrapProcessArgs);
            bootstrapProcess.StartInfo.WorkingDirectory = "tools/vcpkg";
            bootstrapProcess.Start();
            bootstrapProcess.WaitForExit();

            if (bootstrapProcess.ExitCode != 0)
                throw new Exception($"Failed to bootstrap vcpkg :(");
        }

        private static void InstallPackage(string package)
        {
            if (ScriptUtil.Run($"tools/vcpkg/vcpkg install {package}") != 0)
                throw new Exception($"Failed to install package: {package}");
        }

        public bool Run(ScriptArgs args)
        {
            Bootstrap();
            InstallPackage("entt");
            InstallPackage("glm");
            InstallPackage("sdl2");
            InstallPackage("yaml-cpp");
            InstallPackage("spdlog");
            InstallPackage("imgui[core,sdl2-binding,sdl2-renderer-binding,docking-experimental]");
            InstallPackage("implot");
            InstallPackage("entt");

            return ScriptUtil.Run("tools/vcpkg/vcpkg integrate install") == 0;
        }
    }
}
