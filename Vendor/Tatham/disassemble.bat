@echo off

for %%f in (*.bin) do (
  ndisasm -b 16 -p intel "%%f" > "%%~nf.n.txt"
)

