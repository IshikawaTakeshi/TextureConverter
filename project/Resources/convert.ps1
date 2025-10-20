$files = Get-Item *.png

foreach($f in $files){
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f -Wait
}
pause