Write-Host ""
Write-Host "**** INSTALL TOOLS ****"
Write-Host ""

if (-Not (Get-Command scoop 2> $null)) {
    Write-Host "Scoop not found, installing it..."
    Set-ExecutionPolicy RemoteSigned -Scope CurrentUser # Optional: Needed to run a remote script the first time
    Invoke-RestMethod get.scoop.sh | Invoke-Expression
}

$script:SCOOP_UPDATED = $false

function install($pkgman, $pkg, $cmd = $null) {
    if ($cmd -eq $null) {
        $cmd = $pkg
    }
    $pkgcmd = $pkgman
    switch($pkgman){
        "npm"{
            $pkgcmd = "npm install -g $pkg"
        }
        "pip"{
            $pkgcmd = "pip install --upgrade $pkg"
        }
        "scoop"{
            $pkgcmd = "scoop which $cmd && scoop update $pkg || scoop install $pkg"
        }
        default{
            Write-Host "**** Package manager $pkgman is unknown! ****"
            Exit 1
        }
    }
    if (-Not (Get-Command $cmd 2> $null)) {
        if (-Not ${script:SCOOP_UPDATED} -And $pkgman -eq "scoop") {
            scoop update
            $script:SCOOP_UPDATED = $true
        }
        Write-Host "**** Install $pkg for command $cmd with $pkgman ($pkgcmd) ****"
        Invoke-Expression "$pkgcmd"
    }
}


install "scoop" "ninja"
install "scoop" "cmake"
install "scoop" "gcc"
install "scoop" "python"
install "scoop" "nodejs-lts" "node"
install "scoop" "pkg-config"
scoop cache rm * 2> $null


install "npm" "pathman"
Write-Host "**** Add python scripts to path ****"
pathman add "~/scoop/apps/python/current/Scripts"


install "pip" "meson"
install "pip" "conan"
pip cache remove * 2> $null

Write-Host "**** Detecting or just showing conan profiles ****"
conan profile show 2> $null || conan profile detect


Write-Host ""
Write-Host "**** INSTALL DEPS WITH CONAN ****"
Write-Host ""
conan install . --output-folder=builddir --build=missing

Write-Host ""
Write-Host "**** SETUP MESON BUILD DIR ****"
Write-Host ""
meson setup --reconfigure --native-file builddir/conan_meson_native.ini ./builddir . || meson setup --native-file builddir/conan_meson_native.ini ./builddir .

Write-Host ""
Write-Host "**** COMPILE WITH MESON ****"
Write-Host ""
meson compile -C builddir

Write-Host ""
Write-Host "**** RUN TEST WITH MESON ****"
Write-Host ""
meson test -C builddir