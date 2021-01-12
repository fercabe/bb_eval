#include "loadutffile.hpp"
#include "listmanager.hpp"
#include "filemanager.hpp"
#include "easyconfig.hpp"


int main(int argc, char **argv)
{
    // Check parammeters
    std::string alumn;
    bool single = false;
    if(argc == 2)
    {
        single = true;
        alumn = std::string(argv[1]);
    }

    // Load configuration file
    EasyConfig conf;
    std::string confFileContent = loadUTFFile("./bb_eval.conf");
    if(confFileContent.empty())
    {
        std::cout << "Can't find bb_eval.conf file!" << std::endl;
        return -1;
    }
    conf.parseBuffer(confFileContent);

    // Load student list
    std::string listFileName;
    if(!conf.find<std::string>("list:path", listFileName))
    {
        std::cout << "Can't find list:path entry into configuration file!" << std::endl;
        return -1;
    }
    std::string listFileContent = loadUTFFile(listFileName);
    if(listFileContent.empty())
    {
        std::cout << "Can't find " << listFileName << " file!" << std::endl;
        return -1;
    }

    // Parse student list
    ListManager list(listFileContent);
    int idCol, scoreCol;
    if(!conf.find<int>("list:id_col", idCol))
    {
        std::cout << "Can't find list:id_col entry into configuration file!" << std::endl;
        return -1;
    }
    list.setIdCol(idCol);
    if(!conf.find<int>("list:score_col", scoreCol))
    {
        std::cout << "Can't find list:score_col entry into configuration file!" << std::endl;
        return -1;
    }
    list.setScoreCol(scoreCol);

    // Load files to be evaluated
    std::string exercisePath;
    if(!conf.find<std::string>("exercise_path", exercisePath))
    {
        std::cout << "Can't find exercise_path entry into configuration file!" << std::endl;
        return -1;
    }
    FileManager fm(exercisePath);

    // Get grade parameters
    double maxScore;
    if(!conf.find<double>("max_score", maxScore))
    {
        std::cout << "Can't find max_score entry into configuration file!" << std::endl;
        return -1;
    }

    // Get default editor if any
    std::string editor;
    conf.find<std::string>("editor", editor);
    
    // Process each exercise
    int i = 1;
    std::string fileKey;
    while(conf.find<std::string>(std::string("exercise")+std::to_string(i)+std::string(":file_key"), fileKey))
    {
        std::cout << "Processing exercise " << fileKey << std::endl;

        // Get compilation and execution commands
        std::string compCmd, exeCmd;
        std::string exercise = std::string("exercise")+std::to_string(i);
        if(!conf.find<std::string>(exercise+":compilation", compCmd))
        {
            std::cout << "Can't find " << exercise << ":compilation entry into configuration file!" << std::endl;
            return -1;
        }
        if(!conf.find<std::string>(exercise+":execution", exeCmd))
        {
            std::cout << "Can't find " << exercise << ":execution entry into configuration file!" << std::endl;
            return -1;
        }

        // Get exercise score
        double exeScore;
        if(!conf.find<double>(exercise+":score", exeScore))
        {
            std::cout << "Can't find " << exercise << ":score entry into configuration file!" << std::endl;
            return -1;
        }

        // Process the evaluator for each student
        for(int j=0; j<list.size(); j++)
        {
            std::vector<std::string> fileNames;
            fileNames = fm.filter(list.getId(j)+std::string(" ")+fileKey);
            if(fileNames.size() == 1)
            {
                // Only process the requested student
                if(single && list.getId(j).compare(alumn) != 0)
                    continue;

                // Get file full path
                std::string file(exercisePath);
                if(file[file.size()-1] != '/')
                    file = file+"/";
                file = file+fileNames[0]; 
                std::cout << "\tProcessing file " << file << std::endl; 

                // Show file into editor (in single mode)
                std::string cmd;
                if(single && !editor.empty())
                {
                    cmd = editor+" "+file;
                    system(cmd.c_str());
                }

                // Compile the excersice
                std::cout << "\t\tCompiling..." << std::endl;
                cmd = compCmd+std::string(" ")+file;
                int res = system(cmd.c_str());

                // Execute the exercise
                int score;
                if(res == 0)
                {
                    std::cout << "\t\tExecuting..." << std::endl;
                    score = system(exeCmd.c_str()) >> 8;  
                    if(score > 10 || score < 0)
                        score = 0;
                }
                else
                    score = 0;
                std::cout << "\t\tScore: " << score << std::endl; 
                
                // Update the student grade in the list
                if(list._data[j].grade < 0)
                    list._data[j].grade = 0.0;
                list._data[j].grade += maxScore*exeScore*(double)score/10.0;
            }
        }

        i++;
    }

    // Write resulting evaluation (if we are not evaluating a single student)
    if(!single)
    {
        std::string evalId;
        if(!conf.find<std::string>("eval_id", evalId))
        {
            std::cout << "Can't find eval_id entry into configuration file!" << std::endl;
            return -1;
        }
        list.write(evalId+".xls");
    }

    return 0;
}


