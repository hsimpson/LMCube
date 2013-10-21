@echo off

set ProjectDir=%1%
set OutDir=%2%
set Configuration=%3%

echo "ProjectDir=%ProjectDir%"
echo "OutDir=%OutDir%"
echo "Configuration=%Configuration%"

if "%Configuration%" == "Debug" (
  xcopy /D /Y %ProjectDir%..\..\LeapDeveloperKit\LeapSDK\lib\x86\Leapd.dll %OutDir%
) ELSE (
  xcopy /D /Y %ProjectDir%..\..\LeapDeveloperKit\LeapSDK\lib\x86\Leap.dll %OutDir%
)
