/*!
 * @file SimpleCrypto.h
 * @copyright	(C) Copyright Fluid Imaging 2014, All rights reserved.
 * @author		J. Peterson
 * @version		1.0
 * @date		01/02/2014
 *
 * @brief This header declares the CSimpleCrypto class.
 *
 *
 * Revision History
 * ----------------
 *  Version | Author        | Date          | Description
 *  :--:    | :-----        | :--:          | :----------
 *    1     | J. Peterson   | 01/02/2014    | initial version
 *
*/
#ifndef SIMPLE_CRYPTO
#define SIMPLE_CRYPTO

#include <string>

/*!
 * @brief Simple "cyclic group" encryption class
 *
 * The CSimpleCrypto class proides methods for encryption of a string with the optional
 * use of a key.  This class is not intended to be used where strong security is
 * required.  It is intended to be used to obfuscate strings that may be visible
 * to the end user in an initialization or configuration file, or even in a feature
 * license string.
 *
 * The simple for of encrypt(std::string &str) and decrypt(std::string &str) maps each printable character in the
 * string into another printable character with the following properties:
 *     1. No character maps to itself
 *     2. The mapping is string position dependent so that the same character in different
 *        positions map to different characters.
 *
 * The following is an example of the encryption with the default alphabet:
 *    "111-222-333-444-555-666-777-888-999" ---> "Gi^Wq\,C871_[7\WwjBmZO3(^xH;@w:H)i\q%cr-lbk.o3"
 *
 * The form of encrypt(std::string &str, std::string &alphabet) and decrypt(std::string &str, std::string &alphabet)
 * that also take an alphabet string map only characters found in the alphabet string.  Other
 * characters are left unchanged.  For example, using an alphabet string that excludes the dash character ('-') the
 * following encryption takes place:
 *     "111-222-333-444-555" ---> "QK8-jBz-SsE-Rh9-EYK"
 *
 * In the third form an encryption key is specified.  This allows the encryption to the tied to particular
 * data, or to a particular installation.
 *
 */
class CSimpleCrypto
{
public:
    //
    // encryption with default alphabet and no key
    //
    static void encrypt(std::string &str);
    static void decrypt(std::string &str);

    //
    // encryption with specified alphabet and no key
    //
    static void encrypt(std::string &str, const std::string &alphabet);
    static void decrypt(std::string &str, const std::string &alphabet);

    //
    // encryption with specified alphabet and key
    //
    static void encrypt(std::string &str, const std::string &alphabet, const std::string &key);
    static void decrypt(std::string &str, const std::string &alphabet, const std::string &key);
};

#endif // SIMPLE_CRYPTO
