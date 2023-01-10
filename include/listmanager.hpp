#ifndef __LISTMANAGER_HPP__
#define __LISTMANAGER_HPP__

#include <vector>
#include <sstream>
#include <iostream>
#include <clocale>

struct Entry
{
    std::vector<std::string> cols;
    float grade;
};

struct ListManager
{
    std::vector<Entry> _data;
    int _iCol, _sCol;

    ListManager(const std::string &list, const char delimiter = '\t')
    {
        std::istringstream ss(list);
        std::string line;    

        while (std::getline(ss, line)) 
        {
            int copy = 0;
            Entry ent;
            std::string elem;
            for(std::string::iterator it=line.begin(); it!=line.end(); ++it)
            {
                if(*it == delimiter)
                {
                    ent.cols.push_back(elem);
                    elem.clear();
                    copy = 0;
                    continue;
                }

                if(*it == '"' && copy == 0)
                    copy = 1;
                else if(*it == '"' && copy == 1)
                    copy = 0;
                else if(copy == 1)
                    elem.push_back(*it);             
            }
            ent.cols.push_back(elem);
            ent.grade = -1.0;
            _data.push_back(ent);
        }
    }

    bool write(std::string fileName)
    {
        // Open file
        std::ofstream ofs(fileName.c_str());
        if (!ofs.is_open()) 
            return false;
        
        for(int i=0; i<_data.size(); i++)
        {
            for(int j=0; j<_data[i].cols.size(); j++)
            {
                if(j != _sCol)
                    ofs << '"' << _data[i].cols[j] << '"' << "\t";
                else
                {
                    if(_data[i].grade >= 0)
                    {
                        ofs << '"' << (int)_data[i].grade << ',' << (int)(10.0*(_data[i].grade-(int)_data[i].grade)) << '"' << "\t";
                    }
                    else 
                        ofs << '"' << _data[i].cols[j] << '"' << "\t";
                }
            }
            ofs << std::endl;
        }

        // Close file
        ofs.close();
    }

    void print(void)
    {        
        for(int i=0; i<_data.size(); i++)
        {
            for(int j=0; j<_data[i].cols.size(); j++)
            {
                if(j != _sCol)
                    std::cout << '"' << _data[i].cols[j] << '"' << "\t";
                else
                {
                    if(_data[i].grade >= 0)
                    {
                        std::cout << '"' << (int)_data[i].grade << ',' << (int)(10.0*(_data[i].grade-(int)_data[i].grade)) << '"' << "\t";
                    }
                    else 
                        std::cout << '"' << _data[i].cols[j] << '"' << "\t";
                }
            }
            std::cout << std::endl;
        }
    }

    void setIdCol(int iCol)
    {
        _iCol = iCol;
    }

    void setScoreCol(int sCol)
    {
        _sCol = sCol;
    }

    int size(void)
    {
        return _data.size();
    }

    std::string &getId(int i)
    {
        return _data[i].cols[_iCol];
    }

    float getScore(int i)
    {
        return _data[i].grade;
    }

    void setScore(int i, float score)
    {
        _data[i].grade = score;
    }

};


#endif

