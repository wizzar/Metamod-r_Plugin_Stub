Project options:
	Metamod requires Options > Advanced > Character Set > Use Multi-Byte Character Set
	Additional includes in Options > C/C++ > General:
		$(ProjectDir)include\metamod;
		$(ProjectDir)\include\cssdk\common;
		$(ProjectDir)\include\cssdk\dlls;
		$(ProjectDir)\include\cssdk\engine;
		$(ProjectDir)\include\cssdk\game_shared;
		$(ProjectDir)\include\cssdk\pm_shared;
		$(ProjectDir)\include\cssdk\public;

Powershell scripts:
	Adjust $hldsPath and $hldsExecutable in PostBuild.ps1

build.yml:
	Adjust PLUGIN_NAME to the name of the compiled dll/so, without extension.