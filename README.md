# EldenCipher — Keyword-Based File Encryptor/Decryptor (C++)

EldenCipher is a small, reliable C++ command-line tool that encrypts or decrypts text files using a keyword-generated monoalphabetic substitution cipher. It’s built to be easy to verify: encrypt a file to produce ciphertext, then decrypt it back to the original.

## Features
- Encrypts/decrypts letters only (A–Z / a–z)
- Preserves punctuation, whitespace (including line breaks), digits, and letter case
- File-based I/O: reads an input file and writes an output file
- Keyword is required (error if missing)
- Optional console “done” message (quality-of-life confirmation)

## How the cipher alphabet is built
Given a keyword:
1. Convert keyword to uppercase and keep letters only (A–Z)
2. Remove duplicate letters (keep first occurrence order)
3. Append the remaining letters of the alphabet in reverse order (Z → A), skipping letters already used

Example keyword: FEATHER  
Unique letters: FEATHR  
Cipher alphabet becomes:
FEATHRZYXWVUSQPONMLKJIGDCB

Mapping is position-based:
Plain:  ABCDEFGHIJKLMNOPQRSTUVWXYZ
Cipher: FEATHRZYXWVUSQPONMLKJIGDCB

Encryption maps plain → cipher. Decryption maps cipher → plain.

## Usage

### Encrypt
    crypt -kKEYWORD input.txt encrypt.txt

### Decrypt
    crypt -d -kKEYWORD encrypt.txt output.txt

### Example
    crypt -kFEATHER input.txt encrypt.txt
    crypt -d -kFEATHER encrypt.txt output.txt

If everything is correct, output.txt should match input.txt exactly.

## Quick verification (instant confidence)
Put this line in your input file:
    ABCXYZ abcxyz

With keyword FEATHER, the expected encrypted result for that line is:
    FEADCB feadcb

## Visual Studio (Windows) — Build & Run
1. Open the project in Visual Studio
2. Build: Build → Build Solution
3. Set the working folder so the program finds your .txt files:
   - Right-click Project → Properties
   - Configuration Properties → Debugging
   - Working Directory: $(ProjectDir)
4. Set Command Arguments for encryption:
   - -kFEATHER input.txt encrypt.txt
5. Run (Local Windows Debugger)
6. Switch Command Arguments for decryption:
   - -d -kFEATHER encrypt.txt output.txt
7. Run again and confirm output.txt matches the original input.txt

## Notes
- The primary output is written to the output file (that’s the point of a file cipher tool).
- Console output is intentionally minimal (errors + optional “done” message).
- Non-letter characters are written unchanged.

## License
MIT (or replace with your preferred license)

## Portfolio context
This project demonstrates:
- Stream-based file processing (ifstream / ofstream)
- Command-line argument parsing
- Deterministic cipher alphabet construction from a keyword
- Clean function decomposition and testable behavior (round-trip encryption/decryption)
