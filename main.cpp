#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//#include "1.h"
//#include "2.h"
#include "include/PubSub.h"
using namespace std;

/**
 * @brief  Convert const char* to size_t
 * @note   When there is an error it returns the maximum of size_t
 * @param  *number: const char*
 * @retval size_t
 * https://stackoverflow.com/questions/34043894/convert-string-to-size-t
 */
size_t to_size_t(const char *number) {
  size_t sizeT;
  std::istringstream iss(number);
  iss >> sizeT;
  if (iss.fail()) {
    return 1;
  } else {
    return sizeT;
  }
}

int main(int argc, char *argv[]) {
  size_t N = 1;
  if (argc == 1)
    cout << "No paramers, default N=1" << endl;
  else {
    N = to_size_t(argv[1]);
  }
  // cout << N << endl;
  CommandSaver com_sav(N);
  CoutPrint cprinter;
  com_sav.add(cprinter);
  FilePrint fprinter;
  com_sav.add(fprinter);
  string temp;
  while (getline(cin, temp)) {
    com_sav.newCmd(temp);
  }

  return 0;
}
