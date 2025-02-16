param (
    [string]$BuildType,
    [string]$PluginDllName
)

# Check and setup BuildType argument
if (-not $BuildType) {
    Write-Host "Error: you must specify a build type (Debug or Release)." -ForegroundColor Red
    Exit 1
}
$BuildType = $BuildType.ToLower()

$hldsParam

if ($BuildType -eq "debug") {
    
    # Executable parameters
    $hldsParam = "-dev -console -game cstrike -port $hldsPort -pingboost 3 -steam -master -secure -bots -timeout 3 +ip $hldsIP +map de_nuke +maxplayers 32 +sys_ticrate 1000 +log on"

} elseif ($BuildType -eq "release") {
    
    # Executable parameters
    $hldsParam = "-console -game cstrike -port $hldsPort -pingboost 3 -steam -master -secure -bots -timeout 3 +ip $hldsIP +map de_nuke +maxplayers 32 +sys_ticrate 1000"

} else {
    Write-Host "Invalid build type: $BuildType"
    Exit 1
}

# Check and setup $PluginDllName argument
if (-not $PluginDllName) {
    Write-Host "Error: you must specify the plugin dll name without extension." -ForegroundColor Red
    Exit 1
}
if ($PluginDllName.EndsWith(".dll")) {
    $PluginDllName = $PluginDllName.Substring(0, $PluginDllName.Length - 4)
}

Write-Host "Build Type: $BuildType" -ForegroundColor Green

# HLDS Path
$hldsPath = "D:\steamcmd\steamapps\common\Half-Life"

# Test HLDS Path
if (!(Test-Path($hldsPath))) {
    Write-Host "$($hldsPath) was not found." -ForegroundColor Yellow
    Exit 0
}

# Executable path
$hldsExecutable = "D:\steamcmd\steamapps\common\Half-Life\hlds.exe"

# Compiled plugin DLL path
$parentPath = Split-Path $PSScriptRoot -Parent
$pluginDllPath = Join-Path -Path $parentPath -ChildPath "build\$BuildType\bin\$PluginDllName.dll"

# Check whether the plugin dll exists in the path
if (!(Test-Path($pluginDllPath))) {
    Write-Host "$($pluginDllPath) was not found (maybe building failed?)." -ForegroundColor Yellow
    Exit 0
}

# DLL Path
$dllDestinationPath = "$hldsPath\cstrike\addons\$PluginDllName\"

# Check if directory exists
if (!(Test-Path -Path $dllDestinationPath -PathType Container)) {
    # Create directory if it does not exist
    New-Item -ItemType Directory -Path $dllDestinationPath -Force | Out-Null
    Write-Host "Directory created: $dllDestinationPath"
}

# Find HLDS process to kill it before copying the new DLL
$hlds = Get-Process -Name hlds -ErrorAction SilentlyContinue

# If found
if ($hlds) {
    $hlds | Stop-Process -Force -ErrorAction SilentlyContinue
}

# Wait 1 second
Start-Sleep(1)

# Copy DLL to path
Copy-Item -Path (Resolve-Path($pluginDllPath)).Path -Destination $dllDestinationPath -Recurse -Force
Write-Host "$pluginDllPath copied to $dllDestinationPath" -ForegroundColor Green

# Copy config file if any
if (Test-Path "config\*.cfg") {
    Copy-Item -Path "config\*.cfg" -Destination $dllDestinationPath -Recurse -Force
    Write-Host "config\*.cfg copied to $dllDestinationPath" -ForegroundColor Green
} else {
    Write-Host "No *.cfg file(s) found in '/src/config' directory so configuration file was copied." -ForegroundColor Yellow
}

# Add plugin to metamod plugins.ini, at least if installed in default location...
$metamodPluginsIniFilePath = "$hldsPath\cstrike\addons\metamod\plugins.ini"
if ((Test-Path($metamodPluginsIniFilePath))) {

    $content = Get-Content $metamodPluginsIniFilePath
    $lineToAdd = "win32 addons\$PluginDllName\$PluginDllName.dll"

    # Check if the plugin is already added
    if ($content -notcontains $lineToAdd) {
        Add-Content -Path $metamodPluginsIniFilePath -Value $lineToAdd
        Write-Output "Plugin added to metamod's plugins.ini."
    }
}
else {
    Write-Host "Metamod's plugins.ini not found in '$metamodPluginsIniFilePath': add the plugin config manually." -ForegroundColor Yellow
}

# Get Ethernet address as IPV4
$hldsIP = (Get-NetIPAddress -AddressFamily IPV4 -InterfaceAlias Ethernet).IPAddress

# Port
$hldsPort = 27015

# Start HLDS
Start-Process -FilePath $hldsExecutable -ArgumentList $hldsParam -WorkingDirectory $hldsPath -WindowStyle Minimized

# Success
Write-Host("Running HLDS: IP is $($hldsIP)") -ForegroundColor Green