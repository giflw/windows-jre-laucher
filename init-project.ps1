if ( Get-Command scoop 2> $null ) {
    Write-Host "Scoop found!"
} else {
    Write-Host "Scoop not found, installing it..."
    Set-ExecutionPolicy RemoteSigned -Scope CurrentUser # Optional: Needed to run a remote script the first time
    Invoke-RestMethod get.scoop.sh | Invoke-Expression
}

Write-Host "*****************************"

scoop install meson ninja conan cmake gcc

Write-Host "*****************************"

meson setup --reconfigure .\build\ . || meson setup .\build\ .

Write-Host "*****************************"

meson compile -C build

Write-Host "*****************************"

.\build\launcher.exe