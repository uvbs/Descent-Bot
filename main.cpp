#include "main.h"

int main(int argc, char **argv)
{
  int retval = 0;
  
  HogManager::Init();
  
   global_Log.add_Logger(Debug, new CLogDriverRaw());

	vector<string> names = HogManager::get_Hog("chaos.hog").get_Filenames();
	for(vector<string>::iterator i = names.begin(); i != names.end(); i++) {
		printf("%s\n", (*i).c_str());
	}
   
   CRdl rdl("chaos.hog", "chaos1.rdl");
   
	return retval;
}
