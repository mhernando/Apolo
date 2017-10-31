; -- Components.iss --
; Demonstrates a components-based installation.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Apolo Robot Simulator
AppVersion=1.0
DefaultDirName={pf}\Apolo
DefaultGroupName=Apolo
UninstallDisplayIcon={app}\Apolo.exe
OutputDir=./Installer
WizardImageFile=./images/installerImage.bmp
WizardSmallImageFile=./images/logoCAR.bmp
PrivilegesRequired=admin

[Types]
Name: "full"; Description: "Full installation"
Name: "sim_only"; Description: "Simulator Only"
Name: "custom"; Description: "Custom installation"; Flags: iscustom


[Components]
Name: "program"; Description: "Program Files"; Types: full sim_only custom; Flags: fixed
Name: "data"; Description: "Environments and objects files"; Types: full
Name: "matlab"; Description: "MatLab Toolbox"; Types: full
Name: "docs"; Description: "Documentation"; Types: full
Name: "libs"; Description: "External Libs -manual installation if needed"; Types: full


[Files]
Source: "Win32/*.*"; DestDir: "{app}"; Components: program
Source: "Matlab32/*.*"; DestDir: "{app}/Matlab"; Components: matlab
Source: "data/*.*"; DestDir: "{app}/data"; Components: data
Source: "docs/*.*"; DestDir: "{app}/docs"; Components: docs
Source: "libs/*.*"; DestDir: "{app}/ExternalLibs"; Components: libs


[Icons]
Name: "{group}\Apolo"; Filename: "{app}\Apolo.exe"
Name: "{group}\Documentation"; Filename: "{app}\docs"
Name: "{group}\ApoloFolder"; Filename: "{app}"

[Run]

;Filename: "{app}\Apolo.exe";  WorkingDir: "{app}\data"
Filename: "{app}\README.TXT"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent
Filename: "{app}\Apolo.exe"; Description: "Launch application"; WorkingDir: "{app}\data"; Flags: postinstall nowait skipifsilent unchecked