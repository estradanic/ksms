#ifndef PREFERENCES_H
#define PREFERENCES_H

#include<iostream>
#include<fstream>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sstream>
#include <stdio.h>

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

inline void editPreference(string category, string oldValue, string newValue){
    ofstream fileOut;
    char* homedir;
    if((homedir = getenv("HOME")) == NULL){
        homedir = getpwuid(getuid())->pw_dir;
    }
    ostringstream os;
    os<<homedir<<"/.ksms/"<<category<<".txt.new";
    string prefdir = os.str();
    fileOut.open(prefdir.c_str(), ios_base::app);
    ifstream fileIn = readPreferences(category);
    string line;
    while(std::getline(fileIn, line)){
        if(line.compare(oldValue) != 0){
            fileOut<<line<<"\n";
        }
    }
    fileOut<<newValue<<"\n";
    ostringstream os2;
    os2<<homedir<<"/.ksms/"<<category<<".txt";
    string oldfile = os2.str();
    remove(oldfile.c_str());
    rename(prefdir.c_str(), oldfile.c_str());
    fileIn.close();
    fileOut.close();

}

inline void removePreference(string category, string value){
    ofstream fileOut;
    char* homedir;
    if((homedir = getenv("HOME")) == NULL){
        homedir = getpwuid(getuid())->pw_dir;
    }
    ostringstream os;
    os<<homedir<<"/.ksms/"<<category<<".txt.new";
    string prefdir = os.str();
    fileOut.open(prefdir.c_str(), ios_base::app);
    ifstream fileIn = readPreferences(category);
    string line;
    while(std::getline(fileIn, line)){
        if(line.compare(value) != 0){
            fileOut<<line<<"\n";
        }
    }
    ostringstream os2;
    os2<<homedir<<"/.ksms/"<<category<<".txt";
    string oldfile = os2.str();
    remove(oldfile.c_str());
    rename(prefdir.c_str(), oldfile.c_str());
    fileIn.close();
    fileOut.close();
}

#endif // PREFERENCES_H
