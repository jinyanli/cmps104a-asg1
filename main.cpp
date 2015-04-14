// $Id: main.cpp,v 1.3 2015-03-25 19:03:01-07 - - $
//jinyan Li
//jli134@ucsc.edu

#include <string>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include "auxlib.h"
#include "stringset.h"
#include <libgen.h>
using namespace std;
const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;
string cppD="";
int yy_flex_debug = 0;
int yydebug = 0;



// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}


// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         intern_stringset (token);

      }
      ++linenr;
   }
}



int main (int argc, char** argv) {  
 set_execname (argv[0]);
 FILE *out;
 int max=0;
 int opt;
int numberOfOpt=0;
   while ((opt = getopt(argc, argv, "@:D:ly")) != -1) {
        
        switch (opt) {
        case '@':
            set_debugflags (optarg);
           numberOfOpt=numberOfOpt+2;
            break;
        case 'D':
              cppD=cppD+"-D" +optarg;  
                numberOfOpt=numberOfOpt+2;      
            break;
	case 'l':
            yy_flex_debug = 1;
               numberOfOpt++;

            break;
	case 'y':
             yydebug = 1;
           numberOfOpt++;

            break;
        default: 
            //errprintf("Error:wrong option\n");
            exit(EXIT_FAILURE); 
            break;
        }
      if(optind>max)
       max=optind;
      
    }

if(max>(argc-1)){
 errprintf ("Usage: %s [-ly] [filename]\n", get_execname());
    //return 1;
       exit(EXIT_FAILURE);
}




if(argc==1) {
    errprintf("Error:no file is accepted\n");
    //return 1;
       exit(EXIT_FAILURE);
  }

 if((argc-numberOfOpt-1)>1) {
    errprintf("Error:Program only accepts one file\n");
    //return 1;
     exit(EXIT_FAILURE);
  }

string findfile;
size_t foundOc;
string checkOc;

   findfile=argv[argc-1];
   //printf("%s\n",findfile.c_str());
   foundOc=findfile.find_last_of(".");
    //printf("%d\n",foundOc);
   checkOc=findfile.substr(foundOc+1,string::npos);//
    //printf("%s\n", checkOc.c_str());

 if(checkOc!="oc") {
    errprintf("Error:Not .oc file\n");
    //return 1;
       exit(EXIT_FAILURE);
  }

  



//strcat(output,".str");
 

      char* filename = argv[argc - 1];
      string command = CPP + " "+cppD+" "+ filename;
      //printf ("command=\"%s\"\n", command.c_str());
      FILE* pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
      }else {
         cpplines (pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
      }
   
   string outputfile= argv[argc-1];
   size_t foundDot= outputfile.find_last_of(".");
   outputfile=  outputfile.substr(0,foundDot);
   outputfile= outputfile.append (".str");
   out = fopen(outputfile.c_str(), "w");
   
   dump_stringset (out);
   fclose(out);
   return get_exitstatus();
}


