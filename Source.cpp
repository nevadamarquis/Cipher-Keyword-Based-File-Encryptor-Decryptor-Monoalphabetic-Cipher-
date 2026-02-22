/**
   @file MarquisNevadaAssignment5_P8_7.cpp
   @author Nevada Marquis
   @date 2026-02-22
   @version 1.0

   Big C++ P8.7: Random monoalphabet cipher.
   Encrypts or decrypts a file using a keyword-based substitution alphabet.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// Author: Nevada Marquis
string normalize_keyword(const string& key)
{
    string cleaned = "";

    for (int i = 0; i < static_cast<int>(key.size()); i++)
    {
        char ch = key[i];

        if (isalpha(static_cast<unsigned char>(ch)))
        {
            ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));

            // Append only if not already present (removes duplicates)
            if (cleaned.find(ch) == string::npos)
            {
                cleaned += ch;
            }
        }
    }

    return cleaned;
}

// Author: Nevada Marquis
string build_cipher_alphabet(const string& key_unique)
{
    string cipher = key_unique;

    // Append remaining letters in reverse order (Z down to A), skipping duplicates
    for (char ch = 'Z'; ch >= 'A'; ch--)
    {
        if (cipher.find(ch) == string::npos)
        {
            cipher += ch;
        }
    }

    return cipher;
}

// Author: Nevada Marquis
bool parse_arguments(int argc, char* argv[],
    bool& decrypt_mode, string& keyword, string& input_name, string& output_name)
{
    decrypt_mode = false;
    keyword = "";
    input_name = "";
    output_name = "";

    // Need at least: crypt -kKEY input output  (argc >= 4)
    if (argc < 4)
    {
        return false;
    }

    // Best practice: treat last two args as filenames (options can appear before them)
    input_name = argv[argc - 2];
    output_name = argv[argc - 1];

    // Scan options in argv[1] .. argv[argc-3]
    for (int i = 1; i <= argc - 3; i++)
    {
        string arg = argv[i];

        if (arg == "-d")
        {
            decrypt_mode = true;
        }
        else if (arg.rfind("-k", 0) == 0)
        {
            // "-kFEATHER" OR "-k FEATHER"
            if (arg.size() > 2)
            {
                keyword = arg.substr(2);
            }
            else
            {
                // "-k" then next token is the keyword (must exist before filenames)
                if (i + 1 > argc - 3)
                {
                    return false;
                }
                keyword = argv[i + 1];
                i++; // consume keyword token
            }
        }
        else
        {
            // Unknown option
            return false;
        }
    }

    // Keyword is required by the assignment
    if (keyword.size() == 0)
    {
        return false;
    }

    return true;
}

// Author: Nevada Marquis
char transform_char(char ch, const string& plain, const string& cipher, bool decrypt_mode)
{
    if (!isalpha(static_cast<unsigned char>(ch)))
    {
        return ch;
    }

    bool was_lower = (islower(static_cast<unsigned char>(ch)) != 0);
    char up = static_cast<char>(toupper(static_cast<unsigned char>(ch)));

    int pos = -1;
    char result = up;

    if (!decrypt_mode)
    {
        // Encrypt: plain -> cipher
        pos = static_cast<int>(plain.find(up));
        if (pos >= 0)
        {
            result = cipher[pos];
        }
    }
    else
    {
        // Decrypt: cipher -> plain
        pos = static_cast<int>(cipher.find(up));
        if (pos >= 0)
        {
            result = plain[pos];
        }
    }

    if (was_lower)
    {
        result = static_cast<char>(tolower(static_cast<unsigned char>(result)));
    }

    return result;
}

// Author: Nevada Marquis
bool process_file(const string& input_name, const string& output_name,
    const string& cipher_alphabet, bool decrypt_mode)
{
    const string PLAIN = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Best practice for encryption-style programs: binary mode prevents newline translation on Windows
    ifstream in(input_name.c_str(), ios::binary);
    if (!in)
    {
        cout << "Error: Cannot open input file: " << input_name << endl;
        return false;
    }

    ofstream out(output_name.c_str(), ios::binary);
    if (!out)
    {
        cout << "Error: Cannot open output file: " << output_name << endl;
        return false;
    }

    char ch;
    while (in.get(ch))
    {
        char converted = transform_char(ch, PLAIN, cipher_alphabet, decrypt_mode);
        out.put(converted);
    }

    return true;
}

int main(int argc, char* argv[])
{
    bool decrypt_mode;
    string keyword;
    string input_name;
    string output_name;

    bool ok = parse_arguments(argc, argv, decrypt_mode, keyword, input_name, output_name);

    if (!ok)
    {
        cout << "Error: Keyword is required." << endl;
        cout << "Usage: crypt [-d] -kKEYWORD input.txt output.txt" << endl;
        cout << "Example: crypt -d -kFEATHER encrypt.txt output.txt" << endl;
        return 0;
    }

    string key_unique = normalize_keyword(keyword);

    if (key_unique.size() == 0)
    {
        cout << "Error: Keyword must contain at least one letter A-Z." << endl;
        return 0;
    }

    string cipher_alphabet = build_cipher_alphabet(key_unique);

    // Sanity check: cipher must be 26 letters
    if (cipher_alphabet.size() != 26)
    {
        cout << "Error: Cipher alphabet generation failed." << endl;
        return 0;
    }

    bool worked = process_file(input_name, output_name, cipher_alphabet, decrypt_mode);

    if (!worked)
    {
        return 0;
    }

    cout << "Grace restored, Tarnished. Task complete." << endl;

    return 0;
}