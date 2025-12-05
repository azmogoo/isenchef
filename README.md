# bitkitchen

Outil en ligne de commande pour manipuler des fichiers : conversion de formats, chiffrement et transformations. Inspiré de CyberChef.

## Compilation

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

L'exécutable sera dans `build/` .

## Utilisation

### Menu PowerShell (le plus simple)

```powershell
.\run-menu.ps1
```

Le menu guide pas à pas. Exemple :

```
PS> .\run-menu.ps1

bitkitchen quick menu

 [1] hex conversion
 [2] base64 conversion
 [3] caesar cipher
 [4] xor
 [5] rc4
 [6] uppercase
 [7] lowercase
 [8] round-trip hex

choose an option (number): 1
input file path: test_files/hex_input.txt
output file path: test_files/hex_output.txt
choose input format (1 bytes, 2 hex, 3 base64): 1
choose output format (1 bytes, 2 hex, 3 base64): 2
```

### Ligne de commande

```bash
./bitkitchen --in <fichier> --input-format <format> [--action <action>] --out <fichier> --output-format <format>
```

**Formats** : `bytes`, `hex`, `base64`

**Actions** :
- `CAESAR` (nécessite `--caesar-shift=<n>`)
- `RC4` (nécessite `--rc4-key=<cle>`)
- `XOR` (nécessite `--xorkey=<cle>`)
- `UPPERCASE`, `LOWERCASE`

**Exemples** :
```bash
#conversion hex
./cmake-build-debug/bitkitchen.exe --in test_files/hex_input.txt --input-format bytes --out test_files/hex_output.txt --output-format hex

#chiffrement césar
./cmake-build-debug/bitkitchen.exe --in test_files/caesar_input.txt --input-format bytes --action CAESAR --caesar-shift=3 --out test_files/caesar_output.txt --output-format bytes

#xor
./cmake-build-debug/bitkitchen.exe --in test_files/xor_input.txt --input-format bytes --action XOR --xorkey=123 --out test_files/xor_output.txt --output-format bytes
```

## Aide

```bash
./cmake-build-debug/bitkitchen.exe --help
```
