@echo off

rem Caminho para o executável
set executavel=%cd%\execGrupo19.exe

rem Lista de instâncias
set Instancias=A-n34-k5 A-n48-k7 B-n34-k5 B-n50-k7 B-n50-k8 Golden_1 Golden_17 M-n101-k10 M-n151-k12 X-n101-k25 X-n280-k17

@REM "%executavel%" A-n34-k5 A-n34-k5.txt 2 0.05 30


rem Loop para iterar sobre as instâncias
for %%i in (%Instancias%) do (
    call :ExecutarComando %%i %%a
)

goto :EOF

:ExecutarComando
    set instancia=%1
    "%executavel%" %instancia% %instancia%.txt 1
goto :EOF