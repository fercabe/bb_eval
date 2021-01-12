#ifndef __EASYCONFIG_HPP__
#define __EASYCONFIG_HPP__

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

struct ConfParam
{
    std::string _key;
    std::string _value;
    std::vector<ConfParam> _block;

    void convert(int &out)
    {
        out = std::stoi(_value);
    }

    void convert(double &out)
    {
        out = std::stod(_value);
    }

    void convert(float &out)
    {
        out = std::stof(_value);
    }

    void convert(std::string &out)
    {
        out = _value;
    }

    void convert(bool &out)
    {
        // To lower case
        std::string k = _key;
        std::transform(k.begin(), k.end(), k.begin(), [](unsigned char c){ return std::tolower(c); });

        if(k.find("true"))
            out = true;
        else
            out = false;
    }
};

struct EasyConfig
{
public:

    bool parseBuffer(std::string &fileBuffer)
    {
        // Clean up the file content leaving only keys, values and special tokens
        _buf = cleanString(fileBuffer);
        
        // Initialize buf iterartor used all around the parsing
        _it = _buf.begin();

        // Parse params until we reach the end of the buffer or error ocurred
        ConfParam p;
        while(parseParam(p))
        {
            _params.push_back(p);  
            p._key.clear();
            p._value.clear();
            p._block.clear();
        }
        
        return true;
    }

    void printParams(void)
    {
        for(int i=0; i<_params.size(); i++)
            printParam(_params[i]);
    }

    template<class T>
    bool find(const std::string key, T &value)
    {
        std::string parsed;
        std::stringstream ss(key);
        std::vector<ConfParam> *p = NULL;
        bool res = false;

        p = &_params;
        while(getline(ss, parsed, ':') && !res)
        {
            for(int i=0; i<(*p).size(); i++)
            {
                if((*p)[i]._key.compare(parsed) == 0)
                {
                    if(! (*p)[i]._value.empty())
                    {
                        res = true;
                        (*p)[i].convert(value);
                    }
                    else
                    {
                        p = &((*p)[i]._block);
                    }
                }
            }

        }

        return res;
    }

private:

    std::string _buf;
    std::string::iterator _it;
    std::vector<ConfParam> _params;

    std::string cleanString(std::string &s)
    {
        std::string r;
        bool comment = false;
        bool inValue = false;
        for(std::string::iterator it=s.begin(); it!=s.end(); ++it)
        {
            // Check if we are in a comment
            if(comment && *it == '\n')
            {
                comment = false;
                continue;
            }
            if(comment)
                continue;
            if(*it == '#')
            {
                comment = true;
                continue;
            }

            // Are we into a value?
            if(*it == '"')
                inValue = !inValue;
                
            // Discard all possible separators unless we are into value string
            if(!inValue && (*it == ' ' || *it == '\t' || *it == '\n' || *it == '\r'))
                continue;
            
            r.push_back(*it);
        }
        return r;
    }

    bool parseParam(ConfParam &p)
    {
        if(parseKey(p))
        {
            if(_it != _buf.end() && *_it == '"')
            {
                if(!parseValue(p))
                    return false;
            }
            else if(_it != _buf.end() && *_it == '{')
            {
                if(!parseBlock(p))
                    return false;
            }
            else
            {
                std::cout << "Unexpected '" << *_it << "' after assignememnt token ':' " << std::endl;
                return false;
            }
        }
        else  
            return false;
        
        return true;
    }
    
    bool parseKey(ConfParam &p)
    {
        while(_it != _buf.end() && *_it != ':')
        {
            if(*_it == '"' || *_it == '}' || *_it == '{')
            {
                std::cout << "Unexpected '" << *_it << "' before assignememnt token ':' " << std::endl;
                return false;
            }
            p._key.push_back(*_it);
            ++_it;
        }
        
        // Remove = token
        if(_it != _buf.end() && *_it == ':')
            ++_it;
        else
            return false;

        return true;
    }

    bool parseValue(ConfParam &p)
    {
        // Remove " token at the beginning
        if(_it != _buf.end() && *_it == '"')
            ++_it;
        else
            return false;

        while(_it != _buf.end() && *_it != '"')
        {
            if(*_it == ':' || *_it == '}' || *_it == '{')
            {
                std::cout << "Unexpected '" << *_it << "' into value." << std::endl;
                return false;
            }
            p._value.push_back(*_it);
            ++_it;
        }

        // Remove " token at the end
        if(_it != _buf.end() && *_it == '"')
            ++_it;
        else
            return false;

        return true;
    }

    bool parseBlock(ConfParam &p)
    {
        // Remove { at the beginning
        if(_it != _buf.end() && *_it == '{')
            ++_it;
        else
            return false;
        
        while(_it != _buf.end() && *_it != '}')
        {
            ConfParam s;
            if(parseParam(s))
                p._block.push_back(s);
            else  
                return false;
        }
        
        // Remove } at the end
        if(_it != _buf.end() && *_it == '}')
            ++_it;
        else
            return false;

        return true;
    }

    void printParam(const ConfParam &p, int indent = 0)
    {
        for(int i=0; i<indent; i++)
            std::cout << "\t";
        std::cout << p._key << ": ";
        if(!p._value.empty())
            std::cout << p._value << std::endl;
        else
        {
            std::cout << "\n";
            for(int i=0; i<indent; i++)
                std::cout << "\t";
            std::cout << "{ " << std::endl;
            for(int i=0; i<p._block.size(); i++)
                printParam(p._block[i], indent+1);
            for(int i=0; i<indent; i++)
                std::cout << "\t";
            std::cout << "} " << std::endl;
        }
    }
};


#endif

