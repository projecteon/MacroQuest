### https://stackoverflow.com/questions/8834278/powershell-if-a-process-is-not-running-start-it

$macroQuest2 = "MacroQuest2"
$isRunning = Get-Process macroQuest2 -ErrorAction SilentlyContinue
if (!$isRunning)
{
    Start-Process -FilePath "MacroQuest2.exe" -WorkingDirectory "C:\Games\mq2ez"
}

$eqbcs = "EQBCServer"
$isRunning = (Get-Process | Where-Object { $_.Name -eq $eqbcs }).Count -gt 0
if (!$isRunning)
{
    Start-Process -FilePath "EQBCServer.exe" -WorkingDirectory "C:\Games\mq2ez"
}

$eqgamePath='C:\Games\Everquest'
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro2"
Start-Sleep -Seconds 2
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro3"
Start-Sleep -Seconds 2
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro4"
Start-Sleep -Seconds 2
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro5"
Start-Sleep -Seconds 2
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro6"
Start-Sleep -Seconds 2
Start-Process -FilePath "eqgame.exe" -WorkingDirectory $eqgamePath -ArgumentList "patchme -h /login:eqretro"