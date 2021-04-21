$macroQuest2 = "MacroQuest2"
#$isRunning = (Get-Process | Where-Object { $_.Name -eq $macroQuest2 }).Count -gt 0
$isRunning = Get-Process macroQuest2 -ErrorAction SilentlyContinue
if (!$isRunning)
{
    Write-Output "MacroQuest2 not running"
}
else
{
    Write-Output "MacroQuest2 running"
}

$eqbcs2 = "EQBCServer"
$isRunning = (Get-Process | Where-Object { $_.Name -eq $eqbcs2 }).Count -gt 0
# $isRunning = Get-Process eqbcs2 -ErrorAction SilentlyContinue
if (!$isRunning)
{
    Write-Output "EQBCServer not running"
}
else
{
    Write-Output "EQBCServer running"
}