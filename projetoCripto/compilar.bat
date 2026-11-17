@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x86

if errorlevel 1 (
    echo Erro ao configurar o compilador!
    exit /b 1
)

cl.exe /nologo /Zi /W4 /TC /D_CRT_SECURE_NO_WARNINGS /Fe:exchange.exe exchange.c funcoesLogin.c funcoesConsulta.c funcoesCarteira.c funcoesCotacao.c

if errorlevel 1 (
    echo.
    echo Erro na compilacao!
    exit /b 1
)

echo.
echo Projeto compilado com sucesso!