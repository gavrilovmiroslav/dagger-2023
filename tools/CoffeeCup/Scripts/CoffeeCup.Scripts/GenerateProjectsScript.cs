using System.Runtime.InteropServices;
using CoffeeCup.Scripting;

namespace CoffeeCup.Scripts
{
    public class GenerateProjectsScript : IScript
    {
        public bool Run(ScriptArgs args)
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                return ScriptUtil.Run("tools/Premake/premake-5.0.0-beta2-windows/premake5.exe vs2022") == 0;
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            {
                return ScriptUtil.Run("tools/Premake/premake-5.0.0-beta2-macos/premake5 xcode4") == 0;
            }
            else
            {
                return ScriptUtil.Run("tools/Premake/premake-5.0.0-beta2-linux/premake5 gmake2") == 0;
            }
        }
    }
}
