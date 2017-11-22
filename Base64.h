/*
Base64 is a group of similar binary-to-text encoding schemes that represent binary data in an
ASCII string format by translating it into a radix-64 representation.

In base64 A is assigned as 0 and so until 51 for z. 52 to 61 for 0-9 and 62 for + and 63 for /.

For Example Take a string Man, the ascii codes for M,a,n are 77,97,110 respectively.
The binary representation are 01001101, 01100001, 01101110. Combining all these we get
010011010110000101101110. Taking it in group of six and writing it in base64 we get TWFu.

So Man encoded in Base64 is TWFu
*/


#ifndef BASE64_H
#define BASE64_H
#include <vector>
#include <string>
using std::string;
using std::vector;

namespace Base64
{
std::string base64_encode(const std::string &);                       //Function declarations
std::string base64_decode(const std::string &);
                                                                      //Salts are basically our encryption and Decryption Keys.
const std::string &SALT1 = "LM::TB::BB";                              //Random SALT1
const std::string &SALT2 = "__:/__77";                                //Random SALT2
const std::string &SALT3 = "line=wowC++";                             //Random SALT3

std::string EncryptB64 (std::string s)
    {
        s = SALT1 + s + SALT2 + SALT3;                                 //Taking a random combinations of SALT's and string to create a larger string
        s = base64_encode (s);                                         //Encoding s via Base_64
        s.insert (7, SALT3);                                           //Inserting SALT3 after 7th character. Random, just to make it complicated.
        s += SALT1;                                                    //Adding SALT1 again to the encoded string
        s = base64_encode (s);                                         //Again encoding the generated string
        s = SALT2 + SALT3 + s + SALT1;                                 //Random combination of SALT's and string s to make a more complicated string
        s = base64_encode (s);                                         //Triple encoding the string using Base64
        s.insert (1, "L");                                             //Inserting L after the 1st character.
        s.insert (7, "M");                                             //Inserting M after the 7th character.
        return s;                                                      //Returning the encoded string.
    }
std::string DecryptB64 (std::string s)
    {
        s = s.erase (7, 1);                                            //Removing 1st character from the 7th position
        s = s.erase (1, 1);                                            //Removing 1st character from the 1st position
        s = base64_decode (s);                                         //Decoding s for the 1st time
        s = s.substr (SALT2.length() + SALT3.length());                //Taking initial position to be started after the length of SALT2+SALT3
        s = s.substr (0, s.length() - SALT1.length());                 //Taking a substring of length s.length - SALT1.length
        s = base64_decode (s);                                         //Decoding for the 2nd time
        s = s.substr (0, s.length() - SALT1.length());                 //Taking s substring of length s-SALT1
        s = s.erase (7, SALT3.length());                               //Erase SALT3 length after 7th character
        s = base64_decode (s);                                         //Decoding for the 3rd time
        s = s.substr (SALT1.length());                                 //Taking initial position after the length of SALT1
        s = s.substr (0, s.length() - SALT2.length() - SALT3.length());//Taking a substring of length s-(SALT2+SALT3)
        return s;
    }

const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode (const std::string &s)
    {
        std::string ret;
        int val = 0;
        int bits = -6;                                           //6 bits encoding in Base64
        const unsigned int b63 = 0x3F;                           //3F is hexadecimal for 63
        for (const auto &c : s)                                  // for each c in string s iterate
            {
                val = (val << 8) + c;                            //Left shift val by 8 and add it to c. Similar to val=val*2^8
                bits += 8;                                       //Base64 works with octets while extracting information. Will add 8 to bits
                while (bits >= 0)
                    {
                        ret.push_back(BASE64_CODES[(val >> bits) & b63]);// bitwise and operation with decimal 63 value to get last 6 bits of string
                        bits -= 6;                                      // updating bits value
                    }
            }
        if (bits > -6)
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

        while (ret.size() % 4)                                    // if remaining string is not of exactly 24 bit we'll add '=' padding
            ret.push_back('=');

        return ret;                                               // returning encoded string
    }

std::string base64_decode(const std::string &s)                   // function definition to decode the string
    {
        std::string ret;                                          // output decoded string
        std::vector<int> vec(256, -1);
        for (int i = 0; i < 64; i++)
            vec [BASE64_CODES[i]] = i;                            //storing all keys of keyboard
        int val = 0, bits = -8;
        for (const auto &c : s)                                   // c refers to characters of string
            {
                if (vec[c] == -1) break;                          // if vector found empty
                val = (val << 6) + vec[c];                        // left shift val by 6 adding coming character to val to get updated val
                bits += 6;                                        // updating bits since input string is combined in group of 6 bits

                if (bits >= 0)
                    {

                        ret.push_back(char((val >> bits) & 0xFF)); //right shifting val by 2 bits at a time and bitwise 'and' with decimal 255
                        bits -= 8;                                // updating bits by 8 so that decoded string combined in group of 8 bits
                    }
            }

        return ret;                                              // returning decoded string
    }
}
#endif
