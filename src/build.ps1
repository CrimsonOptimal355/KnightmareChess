$ErrorActionPreference = 'Stop'

# Wrapper kept in src so VS Code task can target src/build.ps1.
# Actual build logic remains centralized in tools/build.ps1.
$projectDir = Split-Path -Path $PSScriptRoot -Parent
$realBuildScript = Join-Path $projectDir 'tools\build.ps1'

if (-not (Test-Path $realBuildScript)) {
    Write-Error "Build script not found: $realBuildScript"
    exit 1
}

& $realBuildScript
exit $LASTEXITCODE
