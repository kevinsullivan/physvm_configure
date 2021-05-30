/***********************************************
Lean-specific consistency-checking functionality
************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "Checker.h"
#include "Domain.h"

//#include <g3log/g3log.hpp>
/*
struct aFile {
    FILE* file;
    const char* name;
};
*/
aFile* openFile();
aFile* openFile(std::string);
void generateMath(aFile* f, interp::Interpretation* i);
bool checkMath(aFile*);
void cleanup(aFile*);

// return true if domain is consistent
bool Checker::Check() {
    aFile* f = openFile();
    generateMath(f, interp_); 
    bool status = checkMath(f);
    cleanup(f);
    return status;
}

std::string myfile = "/peirce/PeirceOutput.lean";

bool Checker::CheckPoll(){
    
    aFile* f = openFile(myfile);
    generateMath(f, interp_); 
    bool status = true;//checkMath(f);
    cleanup(f);
    return status;
}

bool Checker::Setup(){
    return true;
    //this->to_check_ = openFile();
}

/************************
 * Implementation Details
 * **********************/

void writeDomain(FILE*, domain::Domain& d);

aFile* openFile() {
    aFile* f = new aFile;
    std::string name = std::string(tmpnam(NULL)) + ".lean";
    char * name_cstr = new char [name.length()+1];
    strcpy (name_cstr, name.c_str());
    f->name = name_cstr;
    std::cout<<"Generating file ... " << name_cstr << "\n";
    f->file = fopen(f->name,"w");
    return f;
}
aFile* openFile(std::string fname) {
    aFile* f = new aFile;
    std::string name = fname;
    char * name_cstr = new char [name.length()+1];
    strcpy (name_cstr, name.c_str());
    f->name = name_cstr;
    std::cout<<"Generating file ... " << name_cstr << "\n";
    f->file = fopen(f->name,"w");
    return f;
}

void generateMath(aFile* f, interp::Interpretation* interp) {
    std::string math = interp->toString_AST();
    //math += "import .lang.imperative_DSL.physlang\n\n";
    //math += "noncomputable theory\n\n";
    //math += interp->toString_Spaces();
    //math += interp->toString_PROGRAMs();
   // math += interp->toString_COMPOUND_STMTs();
    //math += interp->toString_ScalarDefs();
    //math += interp->toString_Defs();
    //math += interp->toString_TransformDefs();
    //math += interp->toString_Assigns();
    //math += interp->toString_ScalarAssigns();
    //math += interp->toString_TransformAssigns();
    //LOG(DEBUG) << "Checker::generateMath generated this: \n"
    //           << math << "\n";
    fputs(math.c_str(), f->file);
    fclose(f->file);
}

void cleanup(aFile* f) {
    delete f->name;
    delete f;
}

/*
launch a domain output file type checking process
get the exit code to determine whether there was an error
return true if there was no error otherwise return false
*/
bool checkMath(aFile* f) {
    int status = system((std::string("lean ") + std::string(f->name)).c_str());
    return (status == 0); 
}
