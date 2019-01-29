/***********************************************
Lean-specific consistency-checking functionality
************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "Checker.h"

struct aFile {
    FILE* file;
    const char* name;
};

aFile* openFile();
void generateMath(aFile* f, Bridge& dom);
bool checkMath(aFile*);
void cleanup(aFile*);

// return true if domain is consistent
bool Checker::Check() {
    aFile* f = openFile();
    generateMath(f, dom_);
    bool status = checkMath(f);
    cleanup(f);
    return status;
}

/************************
 * Implementation Details
 * **********************/

void writeTheory(FILE*);
void writeDomain(FILE*, Bridge& d);

aFile* openFile() {
    aFile* f = new aFile;
    string name = std::string(tmpnam(NULL)) + ".lean";
    char * name_cstr = new char [name.length()+1];
    strcpy (name_cstr, name.c_str());
    f->name = name_cstr;
    f->file = fopen(f->name,"w");
    return f;
}

void generateMath(aFile* f, Bridge& dom) {
    writeTheory(f->file);
    writeDomain(f->file, dom);
    fclose(f->file);
}

void cleanup(aFile* f) {
    delete f->name;
    delete f;
}

// output Euclidean space header definitions to f
void writeTheory(FILE* f) 
{
    // STUB: output nothing
}

/*
 iterate over vectors and output Lean "def" constructs
      def v1_aFilename_lino := (mkVector <space>)
 iterate over expressions outputting Lean "def" constructs
      def expr123 : Vector <space1> := v1 + v2 
*/
void writeDomain(FILE* f, Bridge& d) {

    // STUB -- write one0line Lean with type error
    fputs ("def s : string := 1\n", f);
}

/*
launch a domain output file type checking process
get the exit code to determine whether there was an error
return true if there was no error otherwise return false
*/
bool checkMath(aFile* f) {
    int status = system((string("lean ") + string(f->name)).c_str());
    return (status == 0); 
}

/*
bool Bridge::Reuse(Expression& expr) {
    if(expr.getVecParam1().getVecSpace().getName() == expr.getVecParam2().getVecSpace().getName()){
        cout<<"This expression is consistent!"<<endl;
    }
    else{
        cout<<"This expression is inconsistent!"<<endl;
    }
    return false;
}
*/