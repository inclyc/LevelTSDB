{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }@inputs:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          legacyPackages = pkgs;
          devShell = pkgs.mkShell
            {
              nativeBuildInputs = with pkgs; [
                git
                gnumake
                meson
                ninja
              ];
            };
        }
      );
}
