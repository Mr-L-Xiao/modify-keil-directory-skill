param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectRoot
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$skillRoot = Split-Path -Parent $scriptDir
$sourceDir = Join-Path $skillRoot "assets\\utilities"
$targetDir = Join-Path $ProjectRoot "Common\\utilities"

if (-not (Test-Path -LiteralPath $sourceDir)) {
    throw "Utilities source directory not found: $sourceDir"
}

if (-not (Test-Path -LiteralPath $ProjectRoot)) {
    throw "Project root not found: $ProjectRoot"
}

if (-not (Test-Path -LiteralPath $targetDir)) {
    New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
}

Get-ChildItem -LiteralPath $sourceDir -File | ForEach-Object {
    $destination = Join-Path $targetDir $_.Name
    Copy-Item -LiteralPath $_.FullName -Destination $destination -Force
}

Write-Output "Copied utilities from '$sourceDir' to '$targetDir'."
