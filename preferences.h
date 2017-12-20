#ifndef PREFERENCES_H
#define PREFERENCES_H

#include<iostream>
#include<fstream>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sstream>

using namespace std;

inline void writePreference(string category, string value){
    ofstream fileOut;
    char* homedir;
    if((homedir = getenv("HOME")) == NULL){
        homedir = getpwuid(getuid())->pw_dir;
    }
    ostringstream os;
    os<<homedir<<"/.ksms/"<<category<<".txt";
    string prefdir = os.str();
    fileOut.open(prefdir.c_str(), ios_base::app);
    fileOut<<value<<"\n";
    fileOut.close();
}

inline ifstream readPreferences(string category){
    ifstream fileIn;
    char* homedir;
    if((homedir = getenv("HOME")) == NULL){
        homedir = getpwuid(getuid())->pw_dir;
    }
    ostringstream os;
    os<<homedir<<"/.ksms/"<<category<<".txt";
    string prefdir = os.str();
    fileIn.open(prefdir.c_str(), ios::out);
    return fileIn;
}

#endif // PREFERENCES_H
