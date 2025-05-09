
# Define URLs
$vs2022_url = "https://aka.ms/vs/17/release/vs_BuildTools.exe"
$vs2017_url = "https://aka.ms/vs/15/release/vs_BuildTools.exe"

# Define download paths
$download_dir = "$env:TEMP\vs_buildtools"
New-Item -ItemType Directory -Path $download_dir -Force | Out-Null
$vs2022_installer = Join-Path $download_dir "vs2022_buildtools.exe"
$vs2017_installer = Join-Path $download_dir "vs2017_buildtools.exe"

# Define log files
$vs2022_log = "$env:TEMP\vs2022_install.log"
$vs2017_log = "$env:TEMP\vs2017_install.log"

# Download installers
Invoke-WebRequest -Uri $vs2022_url -OutFile $vs2022_installer -MaximumRedirection 5
Invoke-WebRequest -Uri $vs2017_url -OutFile $vs2017_installer -MaximumRedirection 5

# Install VS 2022 Build Tools
Start-Process -Wait -FilePath $vs2022_installer -ArgumentList @(
    "--passive", "--norestart", "--nocache",
    "--add", "Microsoft.VisualStudio.Workload.VCTools",
    "--add", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
    "--add", "Microsoft.VisualStudio.Component.VC.Redist.14.Latest",
    "--add", "Microsoft.VisualStudio.Component.Windows10SDK.19041",
    "--add", "Microsoft.VisualStudio.Component.Windows10SDK.20348",
    "--add", "Microsoft.VisualStudio.Component.VC.v141.x86.x64",
    "--add", "Microsoft.VisualStudio.Component.CMake.Project",
    "--add", "Microsoft.VisualStudio.Component.VC.Llvm.Clang",
    "--add", "Microsoft.VisualStudio.Component.VC.Llvm.ClangToolset",
    "--add", "Microsoft.VisualStudio.ComponentGroup.NativeDesktop.Llvm.Clang",
    "--includeRecommended",
    "--log", $vs2022_log
)

# Install VS 2019 Build Tools (only legacy components)
Start-Process -Wait -FilePath $vs2017_installer -ArgumentList @(
    "--passive", "--norestart", "--nocache",
    "--add", "Microsoft.VisualStudio.Component.VC.140",                   # VS2015 toolset
    "--add", "Microsoft.VisualStudio.Component.Windows81SDK",            # Windows 8.1 SDK
    "--includeRecommended",
    "--log", $vs2017_log
)

Write-Host "`n✅ Build Tools installation completed."
Write-Host "📄 Logs:"
Write-Host "  VS2022 → $vs2022_log"
Write-Host "  VS2017 → $vs2017_log"
