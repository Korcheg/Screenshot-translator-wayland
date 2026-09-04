# shell.nix
{ pkgs ? import <nixpkgs> {} }:
#let
  
# We pin to a specific nixpkgs commit for reproducibility.
  
# Last updated: 2024-04-29. Check for new commits at https://status.nixos.org.
  
#pkgs = import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/cf8cc1201be8bc71b7cbbbdaf349b22f4f99c7ae.tar.gz") {};
pkgs.mkShell {
  
packages = [
    
(pkgs.python313.withPackages (python-pkgs: with python-pkgs; [
      
# select Python packages here
      paddleocr
      paddlex[ocr]
      shapely
      scikit-image
      pyclipper
      lmdb
      tqdm
      rapidfuzz
      opencv4
]))
  
];
}
