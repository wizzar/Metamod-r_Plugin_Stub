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

# Debug DLL
$parentPath = Split-Path $PSScriptRoot -Parent
$debugDLL = Join-Path -Path $parentPath -ChildPath "build\debug\bin\$pluginName.dll"

# Test if was build
if (!(Test-Path($debugDLL)))
{
    Write-Host("Failed do run HLDS: $($debugDLL) was not found or not built.") -ForegroundColor yellow;
    Exit 0
}

# Get Ethernet address as IPV4
$hldsIP = (Get-NetIPAddress -AddressFamily IPV4 -InterfaceAlias Ethernet).IPAddress;

# Port
$hldsPort = 27015

# Executable parameters
$hldsParam = "-dev -console -game cstrike -port $hldsPort -pingboost 3 -steam -master -secure -bots -timeout 3 +ip $hldsIP +map de_nuke +maxplayers 32 +sys_ticrate 1000 +log on";

# DLL Path
$dllPath = "$hldsPath\cstrike\addons\$pluginName";

# Resolve path of debug dll
$debugDLL = (Resolve-Path($debugDLL)).Path;

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
Copy-Item  -Path $debugDLL -Destination $dllPath -Recurse -Force

# Start HLDS
Start-Process -FilePath $hldsExecutable -ArgumentList $hldsParam -WorkingDirectory $hldsPath -WindowStyle Minimized

# Success
Write-Host("Running HLDS: IP is $($hldsIP)") -ForegroundColor green;