@echo off
if not exist bin\pbl_optimizer.exe (
    echo [*] Executable not found. Running build.bat first...
    call build.bat
)

bin\pbl_optimizer.exe %*
