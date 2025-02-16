# HLDS Path
$hldsPath = "D:\steamcmd\steamapps\common\Half-Life";

# Test HLDS Path
if (!(Test-Path($hldsPath)))
{
    Write-Host("$($hldsPath) was not found.") -ForegroundColor yellow;
    Exit 0
}

# Executable path
$hldsExecutable = "D:\steamcmd\steamapps\common\Half-Life\hlds.exe";

# Plugin Name
$pluginName = "StubPlugin";

# Release DLL
$parentPath = Split-Path $PSScriptRoot -Parent
$releaseDLL = Join-Path -Path $parentPath -ChildPath "build\release\bin\$pluginName.dll"

# Test if was build
if (!(Test-Path($releaseDLL)))
{
    Write-Host("Failed do run HLDS: $($releaseDLL) was not found or not built.") -ForegroundColor yellow;
    Exit 0
}

# Get Ethernet address as IPV4
$hldsIP = (Get-NetIPAddress -AddressFamily IPV4 -InterfaceAlias Ethernet).IPAddress;

# Port
$hldsPort = 27015

# Executable parameters
$hldsParam = "-console -game cstrike -port $hldsPort -pingboost 3 -steam -master -secure -bots -timeout 3 +ip $hldsIP +map de_nuke +maxplayers 32 +sys_ticrate 1000";

# DLL Path
$dllPath = "$hldsPath\cstrike\addons\$pluginName";

# Resolve path of release dll
$releaseDLL = (Resolve-Path($releaseDLL)).Path;

# Find HLDS Process
$hlds = Get-Process -Name hlds -ErrorAction SilentlyContinue;

# If found
if ($hlds)
{
    $hlds | Stop-Process -Force -ErrorAction SilentlyContinue
}

# Wait 1 second
Start-Sleep(1);

# Copy DLL to path
Copy-Item  -Path $releaseDLL -Destination $dllPath -Recurse -Force

# Start HLDS
Start-Process -FilePath $hldsExecutable -ArgumentList $hldsParam -WorkingDirectory $hldsPath -WindowStyle Minimized

# Success
Write-Host("Running HLDS: IP is $($hldsIP)") -ForegroundColor green;