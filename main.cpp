/*!
\file
\brief Файл с обработкой команд
Данный файл содержит в себе прикладной код
обработки команд фиксированного и произвольного размера
*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
  /// число команд в блоке, по умолчанию 1
  size_t N = 1;
  /// получение числа команд в блоке из командной строки
  /// если нет параметра берем 1 по умолчанию
  if (argc == 1)
    cout << "No paramers, default N=1" << endl;
  else {
    N = to_size_t(argv[1]);
  }
  /// хранитель команд инициализируем размером блока
  CommandSaver com_sav(N);
  /// объект для вывода блоков команд в консоль
  CoutPrint cprinter;
  /// добавляем наблюдателем хранителю
  com_sav.add(cprinter);
  /// объект для вывода блоков команд в файл
  FilePrint fprinter;
  /// добавляем наблюдателем хранителю
  com_sav.add(fprinter);
  /// цикл получения команд
  string temp;
  while (getline(cin, temp)) {
    com_sav.newCmd(temp);
  }

  return 0;
}
