@echo off

dir /b /s /a-d "accounts" | findstr .>nul && (
    dir /b /a-d "accounts" > log\accountDATA.txt
    echo Folder is NOT empty, file names written to log\accountDATA.txt
) || (
    >log\accountDATA.txt (
        echo. 
    )
    echo Folder is empty, accountDATA.txt cleared
)