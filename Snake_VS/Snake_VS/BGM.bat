@echo off
set "soundfile=BGM.wav"

echo Set Sound = CreateObject("WMPlayer.OCX.7") >> %temp%\sound.vbs
echo Sound.URL = "%soundfile%" >> %temp%\sound.vbs
echo Sound.settings.volume = 100 >> %temp%\sound.vbs
echo Sound.settings.setMode "loop", True >> %temp%\sound.vbs
echo Sound.settings.volume = 1 >> %temp%\sound.vbs
echo Sound.Controls.play >> %temp%\sound.vbs
echo Do While Sound.playState <> 1 >> %temp%\sound.vbs
echo     WScript.Sleep 100 >> %temp%\sound.vbs
echo Loop >> %temp%\sound.vbs
echo Sound.settings.volume = 100 >> %temp%\sound.vbs
echo While True >> %temp%\sound.vbs
echo     WScript.Sleep 1000 >> %temp%\sound.vbs
echo Wend >> %temp%\sound.vbs

start /min "" wscript.exe //B //nologo %temp%\sound.vbs