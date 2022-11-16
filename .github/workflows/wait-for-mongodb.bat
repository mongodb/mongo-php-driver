@ECHO OFF
:repeat
powershell -nop -c "& {sleep 1}"
mongo --quiet --eval 'db.runCommand({ping:1})' || goto :repeat
