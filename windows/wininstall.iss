[Setup]
AppName=Frets-on-Fire C++
AppVersion=0.9
DefaultDirName={pf}\FretsCPP
DefaultGroupName=Games
UninstallDisplayIcon={app}\fretscpp.exe
OutputDir=c:\fretscpp-inst

[Components]
Name: "main"; Description: "Main Files"; Types: full compact custom; Flags: fixed
Name: "down"; Description: "Downloadable songs"; Types: full
Name: "down\foforig"; Description: "Original Frets-on-Fire Songs";  Types: full; ExtraDiskSpaceRequired: 24747653
Name: "down\muldjord"; Description: "Muldjord (Ubuntu Repository)"; Types: full; ExtraDiskSpaceRequired: 29735916
Name: "down\sectoid"; Description: "Sectoid (Ubuntu repository)";   Types: full; ExtraDiskSpaceRequired: 30339643
Name: "down\100ftsnow"; Description: "100ft Snowman";               Types: full; ExtraDiskSpaceRequired: 18999042
Name: "down\hoshi"; Description: "hOSHI";                           Types: full; ExtraDiskSpaceRequired: 14686877
Name: "down\mhalo"; Description: "M-Halo";                          Types: full; ExtraDiskSpaceRequired: 10976409
Name: "down\rustyjazz"; Description: "Rusty Jazz MotorFuck";        Types: full; ExtraDiskSpaceRequired:  2658424
Name: "down\scenery"; Description: "Scenery Channel";               Types: full; ExtraDiskSpaceRequired: 11358151

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: desktopicon\common; Description: "For all users"; GroupDescription: "Additional icons:"; Flags: exclusive
Name: desktopicon\user; Description: "For the current user only"; GroupDescription: "Additional icons:"; Flags: exclusive unchecked
Name: startmicon; Description: "Create a &Start Menu icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Files]
Source: "fretscpp.exe"; DestDir: "{app}"
Source: "data\*"; DestDir: "{app}\data"; Flags: recursesubdirs
Source: "*.dll"; DestDir: "{app}";
Source: "dlsongs\*"; DestDir: "{app}\dlsongs";
Source: "README"; DestDir: "{app}"; DestName: "README.TXT"; Flags: isreadme

[Icons]
Name: "{group}\Games";              Filename: "{app}\fretscpp.exe"; Tasks: startmicon
Name: "{commondesktop}\FretsC++";   Filename: "{app}\fretscpp.exe"; Tasks: desktopicon\common
Name: "{userdesktop}\FretsC++";     Filename: "{app}\fretscpp.exe"; Tasks: desktopicon\user

[Run]
Filename: "{app}\dlsongs\down-foforig.bat";   WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... Original Frets-on-Fire";  Components: down\foforig
Filename: "{app}\dlsongs\down-muldjord.bat";  WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... Muldjord";                Components: down\muldjord
Filename: "{app}\dlsongs\down-sectoid.bat";   WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... Sectoid";                 Components: down\sectoid
Filename: "{app}\dlsongs\down-100ftsnow.bat"; WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... 100ft Snowman";           Components: down\100ftsnow
Filename: "{app}\dlsongs\down-hoshi.bat";     WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... hOSHI";                   Components: down\hoshi
Filename: "{app}\dlsongs\down-mhalo.bat";     WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... M-Halo";                  Components: down\mhalo
Filename: "{app}\dlsongs\down-rustyjazz.bat"; WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... Rusty Jazz MotorFuck";    Components: down\rustyjazz
Filename: "{app}\dlsongs\down-scenery.bat";   WorkingDir: "{app}\dlsongs"; StatusMsg: "Downloading Songs ... Scenery Channel";         Components: down\scenery

Filename: "{app}\fretscpp.exe"; Description: "Launch game"; Flags: postinstall nowait skipifsilent unchecked

[UninstallDelete]
Type: filesandordirs; Name: "{app}\data\songs"
