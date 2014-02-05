/*!
 *
 * @file        SimpleCrypto.cpp
 * @author      J. Peterson
 * @version     1.0
 * @date        01/02/2014
 * @copyright	(C) Copyright Fluid Imaging 2014, All rights reserved.
 *
 * @brief This file contains the implementation of the CSimpleCrypto class.
 *
 *
 *
 * Revision History
 * ----------------
 *  Version | Author        | Date          | Description
 *  :--:    | :-----        | :--:          | :----------
 *    1     | J. Peterson   | 01/02/2014    | initial version
 *
*/
#include <stdint.h>
#include <string.h>
#include "SimpleCrypto.h"

static const int g_primeCount = 128;
static const int g_primeList[g_primeCount] =
{
    1229, 1409, 1867, 1619,  947, 1213, 1459, 1097, 1663, 1327,  967, 1091, 1847, 1811,  997, 1367,
    1399, 1481,  971, 1483, 1559, 1453, 1051, 1223, 1087, 1873,  977, 1621, 1531, 1301, 1153, 1823,
    1697, 1381, 1187, 1103, 1787, 1279, 1019, 1699, 1667, 1657, 1297, 1889, 1289, 1039, 1879, 1013,
    1427, 1009, 1423, 1259, 1871, 1723, 1307, 1163, 1721, 1447, 1061, 1217, 1741, 1069, 1609, 1523,
    1753, 1553, 1777, 1321, 1049, 1789, 1579, 1303, 1181, 1021, 1861, 1601, 1031, 1747, 1439, 1831,
    1093, 1319, 1801, 1193, 1063, 1783, 1361, 1109, 1571, 1171, 1877, 1429, 1237, 1549, 1231, 1499,
    1117, 1669, 1567, 1433, 1277, 1291, 1249, 1733, 1123, 1033, 1759, 1607, 1471, 1129, 1709, 1283,
    1613, 1201, 1451, 1583, 1637, 1151, 1627, 1487,  983, 1693, 1373, 1543,  953, 1489, 1493, 1597
};

static const char g_alphabet[] = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static const int  g_alphabetLength = strlen(g_alphabet);


/*!
 * @brief Encrypts the specified string in place using the default alphabet
 *
 * The specified string is encrypted in place.  All printing characters are replaced with
 * a new printing character.
 *
 * @param str - std::string to be encrypted
 * @return void
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::encrypt(std::string &str)
{
    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(g_alphabet, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t x = (ptr - g_alphabet);

        x = (x + p) % g_alphabetLength;
        str[i] = g_alphabet[x];
    }
}


/*!
 * @brief Decrypts the specified string in place using the default alphabet
 *
 * @param str - std::string to be encrypted
 * @return void
 *
 * The specified string is decrypted in place.  All printing characters are replaced with
 * the original character before encryption.
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::decrypt(std::string &str)
{
    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(g_alphabet, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t q = g_alphabetLength - (p % g_alphabetLength);
        int32_t x = (ptr - g_alphabet);
        x = (x + q) % g_alphabetLength;
        str[i] = g_alphabet[x];
    }
}

/*!
 * @brief Encrypts the specified string in place using the specified alphabet
 *
 * @param str - std::string to be encrypted
 * @param[in] alphabet - std::string that contains the characters to encrypt
 * @return void
 *
 * The specified string is encrypted in place.  The specified alphabet lists all
 * characters in the string to be replaced.  Characters in the string that are also
 * in the alphabet string are replaced with another character from the alphabet string.
 * The alphabet string must contain no repeating characters.
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::encrypt(std::string &str, const std::string &alphabet)
{
    const char *alphaStr = alphabet.c_str();
    int alphaLen = alphabet.length();

    if (alphaLen < 2)
        return;

    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(alphaStr, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t x = (ptr - alphaStr);

        x = (x + p) % alphaLen;
        str[i] = alphaStr[x];
    }
}


/*!
 * @brief Decrypts the specified string in place using the specified alphabet
 *
 * @param str - std::string to be encrypted
 * @param[in] alphabet - std::string that contains the characters to encrypt
 * @return void
 *
 * The specified string is decrypted in place using the specified alphabet.
 * All characters in the alphabet are replaced with the original characters
 * before encryption.  The same alphabet string as was used for encription
 * must be specified.
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::decrypt(std::string &str, const std::string &alphabet)
{
    const char *alphaStr = alphabet.c_str();
    int alphaLen = alphabet.length();

    if (alphaLen < 2)
        return;

    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(alphaStr, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t q = alphaLen - (p % alphaLen);
        int32_t x = (ptr - alphaStr);
        x = (x + q) % alphaLen;
        str[i] = alphaStr[x];
    }
}

/*!
 * @brief Encrypts the specified string in place using the specified alphabet and key
 *
 * @param str - std::string to be encrypted
 * @param[in] alphabet - std::string that contains the characters to encrypt
 * @param[in] key - std::string that contains a private key
 * @return void
 *
 * The specified string is encrypted in place using the key.  The key allows the encryption to
 * be tied to a specific installation, for example.  The specified alphabet lists all
 * characters in the string to be replaced.  Characters in the string that are also in the
 * the alphabet string are replaced with another character from the alphabet string.
 *
 * Note: The alphabet string must contain at least 2 characters with none repeating.
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::encrypt(std::string &str, const std::string &alphabet, const std::string &key)
{
    const char *alphaStr = alphabet.c_str();
    int alphaLen = alphabet.length();

    if (alphaLen < 2)
        return;

    if (key.length() == 0)
    {
        encrypt(str, alphabet);
        return;
    }

    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(alphaStr, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t x = (ptr - alphaStr);

        int keyOffset = key[i%alphaLen];
        x = (x + keyOffset) % alphaLen;

        x = (x + p) % alphaLen;
        str[i] = alphaStr[x];
    }
}


/*!
 * @brief Decrypts the specified string in place using the specified alphabet and key
 *
 * @param str - std::string to be encrypted
 * @param[in] alphabet - std::string that contains the characters to encrypt
 * @param[in] key - std::string that contains a private key
 * @return void
 *
 * The specified string is decrypted in place using the specified alphabet and key.
 * All characters in the alphabet are replaced with the original characters
 * before encryption.  The same alphabet and key string as were used for encription
 * must be specified.
 *
 * @author J. Peterson
 * @date 01/02/2013
*/
void CSimpleCrypto::decrypt(std::string &str, const std::string &alphabet, const std::string &key)
{
    const char *alphaStr = alphabet.c_str();
    int alphaLen = alphabet.length();

    if (alphaLen < 2)
        return;

    if (key.length() == 0)
    {
        decrypt(str, alphabet);
        return;
    }


    for (int i=0; i<(int)str.length(); i++)
    {
        const char *ptr = strchr(alphaStr, str[i]);
        if (ptr == NULL)
            continue;

        int32_t p = g_primeList[i%g_primeCount];
        int32_t q = alphaLen - (p % alphaLen);
        int32_t x = (ptr - alphaStr);
        x = (x + q) % alphaLen;

        int keyOffset = key[i%alphaLen];
        keyOffset = alphaLen - (keyOffset % alphaLen);
        x = (x + keyOffset) % alphaLen;

        str[i] = alphaStr[x];
    }
}

