{ pkgs ? import <nixpkgs> {} }:
let
  # Створюємо змінну-знімок старого репозиторію
  stariy_zayob = import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-20.09.tar.gz") {};
in
pkgs.mkShell {
  name = "paddleocr-dev-env";

  # nativeBuildInputs - інструменти для збірки (Nix автоматично додасть сюди правильний GCC через stdenv)
  nativeBuildInputs = with pkgs; [ 
  ];

  # buildInputs - зовнішні бібліотеки
  buildInputs = with pkgs; [ 
    gcc
    gdb
    wayland
    wayland-protocols
    wayland-scanner
    xxd
    libxkbcommon
    pkg-config
  ];

  shellHook = ''
    export LD_LIBRARY_PATH="${pkgs.stdenv.cc.cc.lib}/lib:$LD_LIBRARY_PATH"
    echo "=== C++ Environment Ready ==="
    echo "Compiler: $(c++ --version | head -n 1)"
  '';
}

