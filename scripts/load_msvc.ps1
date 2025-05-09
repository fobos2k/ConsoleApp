# load-msvc.ps1 — sets up the MSVC Build Tools environment in the current PowerShell session

param (
    [string]$Arch = "x64"  # You can change to: x86, x86_amd64, etc.
)

$vcvarsPath = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"

if (-Not (Test-Path $vcvarsPath)) {
    Write-Error "vcvarsall.bat not found at: $vcvarsPath"
    return
}

Write-Host "Initializing MSVC environment for architecture: $Arch" -ForegroundColor Cyan

# Run vcvarsall.bat and capture environment variables via `set`
$envDump = cmd /c "`"$vcvarsPath`" $Arch && set"

# Apply each environment variable to the current PowerShell session
foreach ($line in $envDump) {
    if ($line -match "^([^=]+)=(.*)$") {
        Set-Item -Path "env:$($matches[1])" -Value $matches[2]
    }
}

Write-Host "MSVC environment loaded. Example: cl.exe /Bv" -ForegroundColor Green
