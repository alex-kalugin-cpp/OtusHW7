/*!
\file
\brief Файл с классами для команд
Данный файл содержит в себе классы
получения, обработки, хранения и вывода команд фиксированного и произвольного
размера
*/
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class CommandSaver;
/*!
    \brief Родительский класс для команд вывода
  Родительский класс для команд вывода
*/
class IObserverCmd {
 public:
  /*!
  Выводит содержимое из объекта CommandSaver
 \param[in] const CommandSaver& ссылка на объект CommandSaver
 чисто виртуальная
  */
  virtual void handleEvent(const CommandSaver&) = 0;
};
/*!
    \brief Класс для получения, обработки команд и хранения блока команд
  Класс для получения, предварительной обработки команд и хранения не последнего
  выведенного блока команд производьного или фиксированного размера
*/
class CommandSaver  // Observable class
{
 private:
  // data
  //последняя команда
  // string _lastCommand;
  /*!
      \brief Данные
      */
  ///не обработанные команды
  vector<string> _cmd;
  ///размер блока команд фиксированного размера
  size_t _BulkSize;
  ///глубина вложенности {}
  size_t _depthOfBulk = 0;
  /// время получения последней команды
  int64_t _timestamp;
  // for observers
  /*!
      \brief Для наблюдателей
      */
  /// список обрабчиков
  list<IObserverCmd*> _observers;
  /// уведомление обработчиков
  void _Notify() {
    for (auto& observer : _observers) {
      observer->handleEvent(*this);
    }
  }

 public:
  /*!
  Конструктор объекта CommandSaver
 \param[in] size_t BulkSize размер фиксированного блока

  */
  CommandSaver(size_t BulkSize) : _BulkSize(BulkSize) {}
  /*!
  Деструктор объекта CommandSaver
   */
  ~CommandSaver() {
    /*!
   \warning нужно для вывода блока фиксированного размера в случае завершения
   программы вероятно не лучшее решение
    */
    if (_depthOfBulk == 0) _Notify();
  }
  // for observers
  /*!
      \brief Для наблюдателей
      */
  /// добавление
  void add(IObserverCmd& ref) { _observers.push_back(&ref); }
  /// удаление
  void remove(IObserverCmd& ref) { _observers.remove(&ref); }
  // геттреры
  /*!
      \brief получение приватных переменных
      */
  const vector<string>& get() const { return _cmd; }
  size_t getSize() const { return _cmd.size(); }
  int64_t gettime() const { return _timestamp; }
  /*!
  Получение новой команды
 \param[in] string command новая команда

  */
  void newCmd(string command) {
    /// временная метка
    int64_t temp_timestamp = chrono::duration_cast<chrono::milliseconds>(
                                 chrono::system_clock::now().time_since_epoch())
                                 .count();
    /// если начало динамического блока
    if (command == "{") {
      //
      if (_depthOfBulk == 0) {
        _Notify();
        _cmd.clear();
      }
      _depthOfBulk++;

      return;
    }
    /// если конец динамического блока
    if (command == "}") {
      if (_depthOfBulk != 0) _depthOfBulk--;
      if (_depthOfBulk == 0) {
        _Notify();
        _cmd.clear();
      }
      return;
    }
    /// если нормальная команда и блок фиксированный
    if (_depthOfBulk == 0) {
      if (_cmd.empty()) _timestamp = temp_timestamp;
      _cmd.emplace_back(command);
      if (_cmd.size() == _BulkSize) {
        _Notify();
        _cmd.clear();
      }

    } else {
      /// если нормальная команда и блок динамический
      if (_cmd.empty()) _timestamp = temp_timestamp;
      _cmd.emplace_back(command);
    }
  }
};
/*!
    \brief Класс для вывода блоков команд в файл
 */
class FilePrint : public IObserverCmd {
 public:
  /*!
  Выводит содержимое из объекта CommandSaver в файл
 \param[in] const CommandSaver& ссылка на объект CommandSaver
 виртуальная
  */
  virtual void handleEvent(const CommandSaver& ref) {
    ofstream fout;
    /// имя файла
    string filename = "bulk" + to_string(ref.gettime()) + ".log";
    size_t tempsize = ref.getSize();
    if (tempsize != 0) {
      fout.open(filename);
      fout << "bulk: ";
      for (size_t i = 0; i < tempsize; ++i) {
        if (i != 0) fout << ", ";
        fout << ref.get().at(i);
      }
      fout << endl;
      fout.close();
    }
  }
};
/*!
    \brief Класс для вывода блоков команд в файл
 */

class CoutPrint : public IObserverCmd  // Prints the observed string into file
{
 public:
  /*!
  Выводит содержимое из объекта CommandSaver в консоль
  \param[in] const CommandSaver& ссылка на объект CommandSaver
  виртуальная
  */
  virtual void handleEvent(const CommandSaver& ref) {
    size_t tempsize = ref.getSize();
    if (tempsize != 0) {
      cout << "bulk: ";
      for (size_t i = 0; i < tempsize; ++i) {
        if (i != 0) cout << ", ";
        cout << ref.get().at(i);
      }
      cout << endl;
    }
  }
};
/*!
    \brief Класс для вывода блоков команд в вектор
    Класс для вывода блоков команд в вектор. Используется для тестов.
 */
class StringPrint : public IObserverCmd  // Prints the observed string into file
{
 private:
  /// вектор с блоками команд
  vector<string> _logCmdVector;

 public:
  const vector<string>& get() const { return _logCmdVector; }
  /*!
  Сохраняет содержимое из объекта CommandSaver в вектор
  \param[in] const CommandSaver& ссылка на объект CommandSaver
  виртуальная
  */
  virtual void handleEvent(const CommandSaver& ref) {
    size_t tempsize = ref.getSize();
    if (tempsize != 0) {
      stringstream strout;
      strout << "bulk: ";
      for (size_t i = 0; i < tempsize; ++i) {
        if (i != 0) strout << ", ";
        strout << ref.get().at(i);
      }
      _logCmdVector.emplace_back(strout.str());
    }
  }
};
