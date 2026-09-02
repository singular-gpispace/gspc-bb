{
  description = "gspc-bb: Singular/GPI-Space Buchberger workflow";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        lib = pkgs.lib;

        requiredGpiSpaceVersion = "26.3";
        requiredSingularVersion = "4.3.0";
        externalScriptsRoot = "/m/scratch/hive/wittmann/Singular_GPI_Space_buchberger";

        # Transitional setup: GPISpace and Singular 4.3.0 are taken from external paths.
        # Provide paths when building:
        #   GPI_SPACE_ROOT=/p/hpc/soft/gspc/26.3 SINGULAR_ROOT=<path> nix build --impure .
        gpiSpaceRoot = builtins.getEnv "GPI_SPACE_ROOT";
        singularRoot = builtins.getEnv "SINGULAR_ROOT";

        runInit = pkgs.writeShellApplication {
          name = "gspc-bb-init";
          text = ''
            set -euo pipefail
            script="${externalScriptsRoot}/init.sh"

            if [[ ! -f "$script" ]]; then
              echo "ERROR: init script not found: $script"
              exit 1
            fi

            # shellcheck source=/dev/null
            . "$script"

            if [[ ''${GPISpace_install_dir:-} != *"/${requiredGpiSpaceVersion}"* ]]; then
              echo "ERROR: expected GPISpace ${requiredGpiSpaceVersion}, got: ''${GPISpace_install_dir:-<unset>}"
              exit 1
            fi

            singular_ver="$(Singular --version 2>&1 | head -n1 || true)"
            if [[ "$singular_ver" != *"${requiredSingularVersion}"* && "$singular_ver" != *"4-3-0"* ]]; then
              echo "ERROR: expected Singular ${requiredSingularVersion}, got: $singular_ver"
              exit 1
            fi

            echo "Environment initialized with GPISpace ${requiredGpiSpaceVersion} and Singular ${requiredSingularVersion}."
            echo "Note: run this as 'source ${externalScriptsRoot}/init.sh' in your interactive shell to persist exports."
          '';
        };

        runFreshCompileAndRun = pkgs.writeShellApplication {
          name = "gspc-bb-fresh-compile-and-run";
          text = ''
            set -euo pipefail
            init_script="${externalScriptsRoot}/init.sh"
            run_script="${externalScriptsRoot}/fresh_compile_and_run.sh"

            if [[ ! -f "$init_script" ]]; then
              echo "ERROR: init script not found: $init_script"
              exit 1
            fi
            if [[ ! -f "$run_script" ]]; then
              echo "ERROR: run script not found: $run_script"
              exit 1
            fi

            # shellcheck source=/dev/null
            . "$init_script"

            if [[ ''${GPISpace_install_dir:-} != *"/${requiredGpiSpaceVersion}"* ]]; then
              echo "ERROR: expected GPISpace ${requiredGpiSpaceVersion}, got: ''${GPISpace_install_dir:-<unset>}"
              exit 1
            fi

            singular_ver="$(Singular --version 2>&1 | head -n1 || true)"
            if [[ "$singular_ver" != *"${requiredSingularVersion}"* && "$singular_ver" != *"4-3-0"* ]]; then
              echo "ERROR: expected Singular ${requiredSingularVersion}, got: $singular_ver"
              exit 1
            fi

            exec bash "$run_script"
          '';
        };
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            ninja
            pkg-config
            boost
            git
          ];

          shellHook = ''
            export NIX_CONFIG="experimental-features = nix-command flakes"
            echo "devShell ready: cmake, ninja, boost"
            if [ -z "${gpiSpaceRoot}" ]; then
              echo "NOTE: Set GPI_SPACE_ROOT for configure/build, e.g.:"
              echo "  export GPI_SPACE_ROOT=/p/hpc/soft/gspc/${requiredGpiSpaceVersion}"
            fi
            if [ -z "${singularRoot}" ]; then
              echo "NOTE: Set SINGULAR_ROOT (Singular ${requiredSingularVersion} install prefix) for configure/build."
            fi
          '';
        };

        packages.default = pkgs.stdenv.mkDerivation {
          pname = "gspc-bb";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
          ];

          buildInputs = with pkgs; [
            boost
          ];

          cmakeFlags = [
            "-DBoost_USE_DEBUG_RUNTIME=OFF"
            "-DGSPC_WITH_MONITOR_APP=ON"
            "-DGSPC_WITH_EXPRESSION_SHELL=ON"
            "-DCMAKE_INSTALL_PREFIX=${placeholder "out"}"
          ] ++ lib.optionals (gpiSpaceRoot != "") [
            "-DGPISpace_ROOT=${gpiSpaceRoot}"
          ] ++ lib.optionals (singularRoot != "") [
            "-DSINGULAR_HOME=${singularRoot}"
          ];

          preConfigure = ''
            if [ -z "${gpiSpaceRoot}" ]; then
              echo "ERROR: GPI_SPACE_ROOT is not set."
              echo "Example:"
              echo "  GPI_SPACE_ROOT=/p/hpc/soft/gspc/${requiredGpiSpaceVersion} SINGULAR_ROOT=<path> nix build --impure ."
              exit 1
            fi
            if [[ "${gpiSpaceRoot}" != *"/${requiredGpiSpaceVersion}"* ]]; then
              echo "ERROR: expected GPISpace path containing /${requiredGpiSpaceVersion}, got: ${gpiSpaceRoot}"
              exit 1
            fi
            if [ -z "${singularRoot}" ]; then
              echo "ERROR: SINGULAR_ROOT is not set (need Singular ${requiredSingularVersion})."
              echo "Example:"
              echo "  GPI_SPACE_ROOT=/p/hpc/soft/gspc/${requiredGpiSpaceVersion} SINGULAR_ROOT=<path> nix build --impure ."
              exit 1
            fi
          '';
        };

        apps.init = {
          type = "app";
          program = "${runInit}/bin/gspc-bb-init";
        };

        apps.fresh-compile-and-run = {
          type = "app";
          program = "${runFreshCompileAndRun}/bin/gspc-bb-fresh-compile-and-run";
        };
      });
}
