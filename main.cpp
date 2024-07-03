#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <ctime>
#include <vector>

#include<bits/stdc++.h>
#include <process.h>
using namespace std;


#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"


string gen_random(const int len)
{
    srand((unsigned)time(NULL) * getpid());
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

string get_time()
{
    time_t t = std::time(0); // get time now
    tm *now = std::localtime(&t);
    string dateTime = to_string(now->tm_year + 1900) + "/" +
                      to_string(now->tm_mon + 1) + "/" +
                      to_string(now->tm_mday) + " " +
                      to_string(now->tm_hour) + ":" +
                      to_string(now->tm_min) + "\n";

    return dateTime;
}
// =========================================================

class commitNode
{
private:
    string commitID;
    string commitMsg;
    string nextCommitID;
    commitNode *next;

    void createCommitNode()
    {
        // create commit dir, create commitInfo.txt, copy files
        filesystem::create_directory(filesystem::current_path() / ".git" / "commits" / commitID);
        auto path = filesystem::current_path() / ".git" / "commits" / commitID / "commitInfo.txt";
        ofstream write(path.string());
        write << "1." + commitID + "\n" +
                     "2." + commitMsg + "\n" +
                     "3." + get_time() + "\n";

        auto STAGING_AREA_PATH = filesystem::path(filesystem::current_path() / ".git" / "staging_area");
        const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;
        filesystem::copy(STAGING_AREA_PATH, filesystem::current_path() / ".git" / "commits" / commitID / "Data", copyOptions);
    }

public:
    commitNode()
    {
        this->next = NULL;
    }
    commitNode(string commitID, string commitMsg)
    {
        this->commitID = commitID;
        this->commitMsg = commitMsg;
        this->next = NULL;
        createCommitNode();
    }
    commitNode(string commitId)
    {
        // msg in not populated.
        checkNextCommitId();
        this->commitID = commitId;
        next = NULL;
    }

    void revertCommitNode(string fromHash) {
    // Define paths
 
    
   
    auto copyFrom = filesystem::path(filesystem::current_path() / ".git" / "commits" / fromHash / "Data");
    
    cout<<copyFrom<<endl;
  
    // Remove existing content in the target directory if it exists
    
    // Create necessary directories
   filesystem::create_directory(filesystem::current_path() / ".git" / "commits" /getCommitID() );

  

   auto nextCommitPath = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
    
    // Write commit information to commitInfo.txt
    ofstream write(nextCommitPath.string());
    write << "1." + commitID + "\n" +
                  "2." + commitMsg + "\n" +
                  "3." + get_time() + "\n";
    
    // Perform the copy operation
   const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;
    try {
        filesystem::copy(copyFrom, filesystem::current_path() / ".git" / "commits" / getCommitID() / "Data", copyOptions);
          
       
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    }
}

    void readNodeInfo()
    {
        string info;
        auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "commitInfo.txt";
        ifstream file(path.string());
        while (getline(file, info))
        {
            if (info[0] == '1')
            {
                this->setCommitID(info.substr(2));
            }
            if (info[0] == '2')
            {
                this->setCommitMsg(info.substr(2));
            }
        }
    }

    string getCommitMsg()
    {
        return this->commitMsg;
    }
    void setCommitMsg(string commitMsg)
    {
        this->commitMsg = commitMsg;
    }

    void setCommitID(string commitID)
    {
        this->commitID = commitID;
    }
    string getCommitID()
    {
        return this->commitID;
    }

    void setNext(commitNode *node)
    {
        this->next = node;
    }
    commitNode *getNext()
    {
        return next;
    }

    void setNextCommitID(string nextCommitId)
    {
        this->nextCommitID = nextCommitId;
    }
    void writeNextCommitID(string nextCommitID)
    {
        setNextCommitID(nextCommitID);
        auto path = filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt";
        ofstream write(path.string());
        write << nextCommitID;
    }
    string checkNextCommitId()
    {
        filesystem::path tempPath(filesystem::current_path() / ".git" / "commits" / getCommitID() / "nextCommitInfo.txt");
        bool exists = filesystem::exists(tempPath);
        if (exists)
        {
            string info;
            ifstream file(tempPath);
            while (getline(file, info))
            {
                this->nextCommitID = info;
                break;
            }
            file.close();
            return nextCommitID;
        }
        return "";
    }
    string getNextCommitId()
    {
        return this->nextCommitID;
    }
};

class commitNodeList
{
private:
    commitNode *HEAD;
    commitNode *TAIL;

    bool checkHead()
    {
        // check if HEAD commit exists
        auto tempDir = filesystem::current_path() / ".git" / "commits" / "0x1111";
        return filesystem::exists(tempDir);
    }

public:
    commitNodeList()
    {
        setHead(NULL);
        setTail(NULL);
        if (checkHead())
        {
            setHead(new commitNode("0x1111"));
        }
    }

    commitNode *getHead()
    {
        return this->HEAD;
    }
    void setHead(commitNode *HEAD)
    {
        this->HEAD = HEAD;
    }

    commitNode *getTail()
    {
        return this->TAIL;
    }
    void setTail(commitNode *TAIL)
    {
        this->TAIL = TAIL;
    }

    void addOnTail(string msg)
    {
        if (!checkHead())
        {
            commitNode *newNode = new commitNode("0x1111", msg);
            setHead(newNode);
        }
        else
        {
            string commitID = gen_random(8);
            commitNode *currNode = getHead();
            while (currNode != NULL)
            {
                string nextCommitId = currNode->checkNextCommitId();

                if (nextCommitId.length() < 8)
                {
                    commitNode *newNode = new commitNode(commitID, msg);
                    currNode->writeNextCommitID(commitID);
                    currNode = NULL;
                }
                else
                {
                    commitNode *newNode = new commitNode();
                    newNode->setCommitID(nextCommitId);
                    currNode = newNode;
                }
            }
        }
    }

    
    void revertCommit(string commitHash)
    {
        commitNode *commitNodeToRevert;
        if (!checkHead())
        {
            cout << "Nothing to Revert to " << endl;
        }
        else
        {
            bool error = true;
            string commitID = gen_random(8);
            commitNode *currNode = getHead();
            while (currNode != NULL)
            {
                string nextCommitId = currNode->checkNextCommitId();
                if (commitHash == currNode->getCommitID())
                {
                    commitNodeToRevert = currNode;
                    error = false;
                }

                if (nextCommitId.length() < 8)
                {
                    if (!error)
                    {
                        commitNodeToRevert->readNodeInfo();
                        commitNode *newNode = new commitNode();
                        newNode->setCommitID(commitID);
                        newNode->setCommitMsg(commitNodeToRevert->getCommitMsg());
                        newNode->revertCommitNode(commitNodeToRevert->getCommitID());

                        currNode->writeNextCommitID(commitID);
                    }
                    currNode = NULL;
                }
                else
                {
                    commitNode *newNode = new commitNode();
                    newNode->setCommitID(nextCommitId);
                    currNode = newNode;
                }
            }

            if (error == true)
            {
                cout << "does not match any Commit Hash" << endl;
            }
        }
    }

    void printCommitList()
    {
        commitNode *currNode = getHead();
        while (currNode != NULL)
        {
            string nextCommitId = currNode->checkNextCommitId();
            filesystem::path commitPath(filesystem::current_path() / ".git" / "commits" / currNode->getCommitID() / "commitInfo.txt");
            string info;
            ifstream file(commitPath.string());
            while (getline(file, info))
            {
                if (info[0] == '1')
                {
                    cout << "Commit ID:    " << info.substr(2) << endl;
                }
                if (info[0] == '2')
                {
                    cout << "Commit Msg:   " << info.substr(2) << endl;
                }
                if (info[0] == '3')
                {
                    cout << "Date & Time:  " << info.substr(2) << endl;
                }
            }
            file.close();
            cout << "============================\n\n";

            if (nextCommitId.length() < 8)
            {
                currNode = NULL;
            }
            else
            {
                commitNode *newNode = new commitNode();
                newNode->setCommitID(nextCommitId);
                currNode = newNode;
            }
        }
    }

   
};

class gitClass
{
public:

    commitNodeList list;
    void gitCommit(string msg);
  
    void gitInit();
    void gitAdd();
    void gitAdd(string files[], int arrSize);
      void gitRevert(string commitHash);
  void gitLog();
    // void gitStatus();
};

void gitClass::gitLog()
{
    list.printCommitList();
}

void gitClass::gitRevert(string commitHash)
{
    list.revertCommit(commitHash);
}

void gitClass::gitCommit(string msg)
{
    list.addOnTail(msg);
}

void gitClass::gitInit()
{
    filesystem::create_directory(".git");
    filesystem::create_directory(".git/staging_area");
    filesystem::create_directory(".git/commits");
}

bool areFilesIdentical(const std::filesystem::path& filePath1, const std::filesystem::path& filePath2) {
    try {
        // Check if files exist
        if (!std::filesystem::exists(filePath1) || !std::filesystem::exists(filePath2)) {
            std::cerr << "Error: One or both files do not exist." << std::endl;
            return false; // Early exit if files are missing
        }

        // Check if file sizes are the same (faster check)
        if (std::filesystem::file_size(filePath1) != std::filesystem::file_size(filePath2)) {
         //   std::cout << "Files have different sizes and are likely not identical." << std::endl;
            return false;
        }
        return true;

        // Use `equivalent` for efficient byte-by-byte comparison
        return std::filesystem::equivalent(filePath1, filePath2);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error comparing files: " << e.what() << std::endl;
        return false;
    }
}

void gitClass::gitAdd()
{
    const auto copyOptions = filesystem::copy_options::overwrite_existing | filesystem::copy_options::recursive;

    // Helper function to copy files recursively
    auto copyRecursively = [&](const filesystem::path &sourcePath, const filesystem::path &targetPath) {
        for (const auto &dirEntry : filesystem::recursive_directory_iterator(sourcePath))
        {
            auto relativePath = filesystem::relative(dirEntry.path(), sourcePath);
            auto destPath = targetPath / relativePath;

            try
            {
                if (filesystem::is_regular_file(dirEntry.path()))
                {
                    if (!filesystem::exists(destPath))
                    {
                       
                        filesystem::copy(dirEntry.path(), destPath, copyOptions);
                        cout << GRN "Copied " << dirEntry.path().filename() << " successfully!" << END << endl;
                    }
                    else
                    {
                        // Check if the files are different before copying
                        if (!areFilesIdentical(dirEntry.path(), destPath))
                        {
                            cout << YEL "File " << dirEntry.path().filename() << " already exists in the staging area but is different. Updating..." << END << endl;
                            filesystem::remove(destPath);
                            filesystem::copy(dirEntry.path(), destPath, copyOptions);
                            cout << GRN "Updated " << dirEntry.path().filename() << " successfully!" << END << endl;
                        }
                        else
                        {
                            cout << YEL "File " << dirEntry.path().filename() << " already exists in the staging area and is identical. Skipping..." << END << endl;
                        }
                    }
                }
                else if (filesystem::is_directory(dirEntry.path()))
                {
                    if (!filesystem::exists(destPath))
                    {
                
                        filesystem::create_directories(destPath);
                    }
                }
            }
            catch (const filesystem::filesystem_error &e)
            {
                cout << RED "Error copying " << dirEntry.path().filename() << ": " << e.what() << END << endl;
            }
        }
    };

    auto currentPath = filesystem::current_path();
    auto stagingAreaPath = currentPath / ".git" / "staging_area";
   

    for (const auto &dirEntry : filesystem::directory_iterator(currentPath))
    {
        auto fileName = dirEntry.path().filename();
        if (fileName != "git" && fileName != ".git")
        {
            auto sourcePath = currentPath / fileName;
            auto targetPath = stagingAreaPath / fileName;

            if (filesystem::is_directory(sourcePath))
            {
                filesystem::create_directories(targetPath);
                
                copyRecursively(sourcePath, targetPath);
            }
            else if (filesystem::is_regular_file(sourcePath))
            {
                try
                {
                    if (!filesystem::exists(targetPath))
                    {
                        // cout << "Attempting to copy " << sourcePath << " to " << targetPath << endl;
                        filesystem::copy(sourcePath, targetPath, copyOptions);
                        cout << GRN "Copied " << fileName << " successfully!" << END << endl;
                    }
                    else
                    {
                        // Check if the files are different before copying
                        if (!areFilesIdentical(sourcePath, targetPath))
                        {
                            cout << YEL "File " << fileName << " already exists in the staging area but is different. Updating..." << END << endl;
                            filesystem::remove(targetPath);
                            filesystem::copy(sourcePath, targetPath, copyOptions);
                            cout << GRN "Updated " << fileName << " successfully!" << END << endl;
                        }
                        else
                        {
                            cout << YEL "File " << fileName << " already exists in the staging area and is identical. Skipping..." << END << endl;
                        }
                    }
                }
                catch (const filesystem::filesystem_error &e)
                {
                    cout << RED "Error copying " << fileName << ": " << e.what() << END << endl;
                }
            }
        }
    }

     // Remove any files that exist in staging area but not in the working directory
     for (const auto &dirEntry : filesystem::recursive_directory_iterator(stagingAreaPath)) {
        auto relativePath = filesystem::relative(dirEntry.path(), stagingAreaPath);
        auto sourcePath = currentPath / relativePath;

        if (!filesystem::exists(sourcePath)) {
            try {
                if (filesystem::is_regular_file(dirEntry.path())) {
                    filesystem::remove(dirEntry.path());
                    cout << RED "Removed " << relativePath << " from staging area as it does not exist in the current path." << END << endl;
                } else if (filesystem::is_directory(dirEntry.path()) && filesystem::is_empty(dirEntry.path())) {
                    filesystem::remove(dirEntry.path());
                    cout << RED "Removed empty directory " << relativePath << " from staging area as it does not exist in the current path." << END << endl;
                }
            } catch (const filesystem::filesystem_error &e) {
                cout << RED "Error removing " << relativePath << " from staging area: " << e.what() << END << endl;
            }
        }
    }
   
}


void gitClass::gitAdd(string files[], int arrSize)
{
    const auto copyOptions = filesystem::copy_options::update_existing | filesystem::copy_options::recursive;

    for (int i = 0; i < arrSize; i++)
    {
        if (filesystem::exists(files[i]))
        {
            auto sourcePath = filesystem::current_path() / files[i];
            auto targetPath = filesystem::current_path() / ".git" / "staging_area" / files[i];
            
            // Check if the target file already exists
            if (!filesystem::exists(targetPath))
            {
                int last_index = files[i].find_last_of('/');
                if (last_index != string::npos)
                {
                    auto newDir = filesystem::current_path() / ".git" / "staging_area" / files[i].substr(0, last_index);
                    filesystem::create_directories(newDir);
                }
                
                try
                {
                    filesystem::copy(sourcePath, targetPath, copyOptions);
                    cout << GRN "Copied " << files[i] << " successfully!" << END << endl;
                }
                catch (const filesystem::filesystem_error &e)
                {
                    cout << RED "Error copying " << files[i] << ": " << e.what() << END << endl;
                }
            }
            else
            {
                if (!areFilesIdentical(sourcePath, targetPath))
                {
                    cout << YEL "File " << files[i] << " already exists in the staging area but is different. Updating..." << END << endl;
                    try
                    {
                        filesystem::remove(targetPath);
                        filesystem::copy(sourcePath, targetPath, copyOptions);
                        cout << GRN "Updated " << files[i] << " successfully!" << END << endl;
                    }
                    catch (const filesystem::filesystem_error &e)
                    {
                        cout << RED "Error updating " << files[i] << ": " << e.what() << END << endl;
                    }
                }
                else
                {
                    cout << YEL "File " << files[i] << " already exists in the staging area and is identical. Skipping..." << END << endl;
                }
            }
        }
        else
        {
            cout << files[i] << RED " does not exist!" END << endl;
        }
    }
}


void status() {
    auto cwd = filesystem::current_path();
    auto staging_area = cwd / ".git" / "staging_area";

    vector<string> added;
    vector<string> modified;
    vector<string> deleted;

    for (const auto& entry : filesystem::recursive_directory_iterator(cwd)) {
        auto path = entry.path();
        if (path.string().find(".git") != string::npos) {
            continue; // Skip .git directory
        }

        if (filesystem::is_regular_file(path)) {
            auto relative_path = filesystem::relative(path, cwd);
            auto staged_path = staging_area / relative_path;

            if (filesystem::exists(staged_path)) {
                if (  !areFilesIdentical(path, staged_path)) {
                    modified.push_back(relative_path.string());
                }
            } else {
                added.push_back(relative_path.string());
            }
        }
    }

    for (const auto& entry : filesystem::recursive_directory_iterator(staging_area)) {
        auto staged_path = entry.path();
        if (filesystem::is_regular_file(staged_path)) {
            auto relative_path = filesystem::relative(staged_path, staging_area);
            auto path = cwd / relative_path;

            if (!filesystem::exists(path)) {
                deleted.push_back(relative_path.string());
            }
        }
    }

    cout << "Changes to be committed:" << endl;
    for (const auto& file : added) {
        cout << GRN << "\tnew file: " << file << END << endl;
    }
    for (const auto& file : modified) {
        cout << YEL << "\tmodified: " << file << END << endl;
    }
    for (const auto& file : deleted) {
        cout << RED << "\tdeleted: " << file << END << endl;
    }
}




int main(int argc, char *argv[])
{
    gitClass gitClassObj;
    if(argc >= 2)
    {
        string argument = string(argv[1]);
        //git init
        if (argument == "init")
        {
            gitClassObj.gitInit();
            cout <<"git repository initialized successfully!"<< endl;
            
        }
    else if (argument == "add")                                               
        {
            if(argc == 2){     
                cout << RED "missing arguments!" <<endl;
                cout << "Provide a third argument e.g." << endl;
                cout << "git add <'.' | 'file_name'>" END << endl;
            }
            if(argc >= 3){
                if(argc == 3){
                    string argumentC = string(argv[2]);
                    if (argumentC == ".") {
                        gitClassObj.gitAdd();
                    }
                    else
                    {
                        string files[1] = {string(argv[2])};
                        gitClassObj.gitAdd(files, 1);
                    }

                } else {
                    string files[argc-2];
                    for (int i = 0; i < argc-2; i++)
                    {
                        files[i] = string(argv[i]); 
                    }
                    gitClassObj.gitAdd(files, argc-2);
                }
            }
            
        }
         //git commit
        else if (argument == "commit")
        {
            if(argc == 4)    //[ git, commit, -m, "msg" ]
            {
                string argumentC = string(argv[2]);    
                string argumentD = string(argv[3]);      
                if(argumentC == "-m")
                {
                    gitClassObj.gitCommit(argumentD);
                    cout << "files commited successfully" << endl;
                }
            }
            else 
            {
                cout << RED "missing arguments!" <<endl;
                cout << "Provide with a message field e.g." << endl;
                cout << "git commit -m 'my commit message'" END << endl;
            }
        }
         else if(argument == "revert")
        {
            if(argc == 3)
            {
                string argumentC = string(argv[2]);
                if(argumentC == "HEAD")
                {
                    gitClassObj.gitRevert(argumentC);
                    cout << "The project is now at HEAD" << endl;
                }
                else
                {
                    gitClassObj.gitRevert(argumentC);
                    cout << "Reverted to <commit_id> commit" << endl;
                }
            }
            else 
            {
                cout << RED "invalid arguments, should be like: " << endl;
                cout << "git revert <'HEAD'|'commit_hash'>" END<< endl;
            }
        }
          else if(argument == "log")
        {
            gitClassObj.gitLog();
        }
        else if(argument == "status")
        {
            status();
            
        }
    }

 

}