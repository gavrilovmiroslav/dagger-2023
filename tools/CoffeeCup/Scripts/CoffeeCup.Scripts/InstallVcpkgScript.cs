using CoffeeCup.Scripting;
using System.Diagnostics;
using System.Management.Automation;

namespace CoffeeCup.Scripts
{
    public class InstallVcpkgScript : IScript
    {
        public bool Run(ScriptArgs args)
        {
            var dir = Directory.GetCurrentDirectory().Split("\\").ToList();
            dir.RemoveRange(dir.Count - 6, 6);
            var path = String.Join("\\", dir);
            Console.WriteLine(path);

            PowerShell ps = PowerShell.Create();
            ps.AddScript($"{path}\\Scripts\\install.ps1").Invoke();
            return false;
        }
    }
}
