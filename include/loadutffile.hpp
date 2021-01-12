#ifndef __LOADUTFFILE_HPP__
#define __LOADUTFFILE_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iostream>

#define ENCODING_ASCII      0
#define ENCODING_UTF8       1
#define ENCODING_UTF16LE    2
#define ENCODING_UTF16BE    3

void swab(char *src, char *dest, int n)
{
    for(int i=0; i<n; i+=2)
    {
        dest[i] = src[i+1];
        dest[i+1] = src[i];
    }
}

std::string loadUTFFile(std::string path)
{
	std::string result;
	std::ifstream ifs(path.c_str(), std::ios::binary);
	std::stringstream ss;
	int encoding = ENCODING_ASCII;
    size_t size = 0;

	if (!ifs.is_open()) {
		// Unable to read file
		result.clear();
		return result;
	}
	else if (ifs.eof()) {
		result.clear();
	}
	else {

        // Get file size
        ifs.seekg(0, std::ios::end);
        size = (size_t)ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        // Identify file according to BOM
        // Also correct file size
		int ch1 = ifs.get();
		int ch2 = ifs.get();
		if (ch1 == 0xff && ch2 == 0xfe) {
			// The file contains UTF-16LE BOM
			encoding = ENCODING_UTF16LE;
            size -= 2;
            std::cout << "Detected encoding UTF-16LE in file " << path << std::endl;
		}
		else if (ch1 == 0xfe && ch2 == 0xff) {
			// The file contains UTF-16BE BOM
			encoding = ENCODING_UTF16BE;
            size -= 2;
            std::cout << "Detected encoding UTF-16BE in file " << path << std::endl;
		}
		else {
			int ch3 = ifs.get();
			if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf) {
				// The file contains UTF-8 BOM
				encoding = ENCODING_UTF8;
                size -= 3;
                std::cout << "Detected encoding UTF-8 in file " << path << std::endl;
			}
			else {
				// The file does not have BOM
				encoding = ENCODING_ASCII;
				ifs.seekg(0);
                std::cout << "Unknown encoding, assuming ASCII encoding in file " << path << std::endl;
			}
		}
	}

    // Convert file according to enconding    
	if (encoding == ENCODING_UTF16LE) {
        // Read file and stores in u16string
        std::u16string u16((size / 2) + 1, '\0');
        ifs.read((char*)&u16[0], size);

        // Convert to utf8
        std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t> utfconv;
        result = utfconv.to_bytes(u16);
	}
	else if (encoding == ENCODING_UTF16BE) {
        // Read file and stores in u16string
        std::u16string u16((size / 2) + 1, '\0'), swap((size / 2) + 1, '\0');
        ifs.read((char*)&u16[0], size);
        swab((char*)&u16[0], (char*)&swap[0], size);

        // Convert to utf8
        std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t> utfconv;
        result = utfconv.to_bytes(swap);
	}
	else if (encoding == ENCODING_UTF8) {
		std::string s(size + 1, '\0');
        ifs.read((char*)&s[0], size);
        result = s;
	}
	else {
		std::string s(size + 1, '\0');
        ifs.read((char*)&s[0], size);
        result = s;
	}
	return result;
}


#endif
