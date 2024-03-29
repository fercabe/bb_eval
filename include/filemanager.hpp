#ifndef __FILEMANAGER_HPP__
#define __FILEMANAGER_HPP__

#include <string>
#include <dirent.h>

class FileManager
{
public:

    std::vector<std::string> _files;

    FileManager(std::string path)
    {
        struct dirent *ent;
        DIR *dir = opendir(path.c_str());
        if(dir != NULL) 
        {
            while ((ent = readdir (dir)) != NULL) 
                _files.push_back(ent->d_name);
            closedir(dir);
            
            // Eliminate deal with white spaces and parenthesis
            for(size_t i=0; i<_files.size(); i++){
                replaceString(_files[i], " ", "\\ ");
                replaceString(_files[i], "(", "\\(");
                replaceString(_files[i], ")", "\\)");
            }
        }
    }

    FileManager(void) : FileManager("./")
    {
    }

    void print(void)
    {
        std::cout << "List of files:" << std::endl;
        for(int i=0; i<_files.size(); i++)
            std::cout << "\t" << _files[i] << std::endl;
    } 

    std::vector<std::string> filter(const std::string & filter_keys)
    {
        std::vector<std::string> keys = splitString(filter_keys, ' ');
        std::vector<std::string> in, out;
        
        in = _files;
        for(int i=0; i<keys.size(); i++)
        {
            out.clear();
            for(int j=0; j<in.size(); j++)
                if(in[j].find(keys[i]) !=  std::string::npos)
                    out.push_back(in[j]);
            in = out;
        }

        return out;
    }

private:

    void replaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
    {
        std::string::size_type pos = 0u;
        while((pos = str.find(oldStr, pos)) != std::string::npos){
            str.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
    }

    std::vector<std::string> splitString(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
            tokens.push_back(token);
        return tokens;
    }

};


#endif
