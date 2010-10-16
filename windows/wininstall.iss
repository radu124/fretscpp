[Setup]
AppName=Frets-on-Fire C++
AppVersion=0.9
DefaultDirName={pf}\FretsCPP
DefaultGroupName=Games
UninstallDisplayIcon={app}\fretscpp.exe
OutputDir=c:\fretscpp-inst

[Files]
Source: "fretscpp.exe"; DestDir: "{app}"
Source: "data\*"; DestDir: "{app}\data"; Flags: recursesubdirs
Source: "*.dll"; DestDir: "{app}";
Source: "dlsongs\*"; DestDir: "{app}\dlsongs";
Source: "README"; DestDir: "{app}"; DestName: "README.TXT"; Flags: isreadme

[Icons]
Name: "{group}\Games"; Filename: "{app}\fretscpp.exe"

[Run]
Filename: "{app}\dlsongs\sh.exe"; Description: "Download Songs"; WorkingDir: "{app}\dlsongs"; Parameters: "download.sh"
Filename: "{app}\README.TXT"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent
Filename: "{app}\fretscpp.exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

[UninstallDelete]
Type: filesandordirs; Name: "{app}\data\songs"
