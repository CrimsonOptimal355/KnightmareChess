$ErrorActionPreference = 'Stop'

# ── Paths ──────────────────────────────────────────────────────
$scriptDir = $PSScriptRoot
$projectDir = if (Test-Path (Join-Path $scriptDir 'src')) { $scriptDir } else { Split-Path -Path $scriptDir -Parent }
$sfmlDir     = Join-Path $projectDir 'New\Practice\External\SFML-MinGW\SFML-3.0.0'
$outputDir   = Join-Path $projectDir 'bin'
$outputExe   = Join-Path $outputDir 'Chess.exe'

# ── Ensure output dir exists ───────────────────────────────────
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
}

# ── Copy DLLs ──────────────────────────────────────────────────
Write-Host "[1/2] Copying SFML DLLs..."
$sfmlDlls = Join-Path $sfmlDir 'bin\*.dll'
if (Test-Path $sfmlDlls) {
    Copy-Item -Force $sfmlDlls $outputDir
    Write-Host "DLLs successfully copied to $outputDir"
} else {
    Write-Warning "SFML DLLs not found in expected path: $sfmlDir\bin\"
}

# ── Copy Assets ────────────────────────────────────────────────
Write-Host "[1.5/2] Copying project assets..."
$assetsSrc = Join-Path $projectDir 'assets'
if (Test-Path $assetsSrc) {
    # Ensure destination assets folder exists
    $assetsDest = Join-Path $outputDir 'assets'
    if (-not (Test-Path $assetsDest)) {
        New-Item -ItemType Directory -Force -Path $assetsDest | Out-Null
    }
    Copy-Item -Force -Recurse "$assetsSrc\*" $assetsDest
    Write-Host "Assets successfully copied to $assetsDest"
}

# ── Gather Source Files ─────────────────────────────────────────
$srcPath = Join-Path $projectDir 'src'
if (Test-Path $srcPath) {
    $sourceFiles = Get-ChildItem -Path $srcPath -Filter *.cpp |
        Where-Object { $_.Name -ne 'main2.cpp' } |
        ForEach-Object { $_.FullName }
} else {
    $sourceFiles = @()
}

if ($sourceFiles.Count -eq 0) {
    Write-Error "No C++ source files (*.cpp) found in $srcPath"
    exit 1
}

$sourceFilesStr = $sourceFiles | ForEach-Object { "`"$_`"" }

# ── Compile & Link ─────────────────────────────────────────────
Write-Host "[2/2] Compiling Chess files..."
$compileArgs = @(
    '-std=c++17',
    '-g',                          # debug symbols for GDB
    "-I`"$projectDir\include`"",
    "-I`"$sfmlDir\include`"",
    "-L`"$sfmlDir\lib`""
)

# Append source files
$compileArgs += $sourceFilesStr

# Append link libraries
$compileArgs += @(
    '-lsfml-graphics',
    '-lsfml-window',
    '-lsfml-system',
    '-lsfml-audio',
    '-lopengl32',
    "-o `"$outputExe`""
)

$cmd = "g++ $($compileArgs -join ' ')"
Write-Host $cmd
Invoke-Expression $cmd

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build FAILED (exit code $LASTEXITCODE)"
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build succeeded -> $outputExe"
