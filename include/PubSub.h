#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class CommandSaver;
class IObserverCmd {
 public:
  virtual void handleEvent(const CommandSaver&) = 0;
};
class CommandSaver  // Observable class
{
 private:
  // data
  //последняя команда
  string _lastCommand;
  //не обработанные команды
  vector<string> _cmd;
  //размер блока команд фиксированного размера
  size_t _BulkSize;
  //глубина вложенности {}
  size_t _depthOfBulk = 0;
  // время получения последней команды
  int64_t _timestamp;
  // for observers
  list<IObserverCmd*> _observers;
  void _Notify() {
    for (auto& observer : _observers) {
      observer->handleEvent(*this);
    }
  }

 public:
  CommandSaver(size_t BulkSize) : _BulkSize(BulkSize) {}
  ~CommandSaver() {
    if (_depthOfBulk == 0) _Notify();
  }
  // for observers
  void add(IObserverCmd& ref) { _observers.push_back(&ref); }

  void remove(IObserverCmd& ref) { _observers.remove(&ref); }
  // геттреры
  const vector<string>& get() const { return _cmd; }
  size_t getSize() const { return _cmd.size(); }
  int64_t gettime() const { return _timestamp; }
  void newCmd(string command) {
    // _str = str;
    int64_t temp_timestamp = chrono::duration_cast<chrono::milliseconds>(
                                 chrono::system_clock::now().time_since_epoch())
                                 .count();
    // cout << _timestamp << endl;
    if (command == "{") {
      //
      if (_depthOfBulk == 0) {
        _Notify();
        _cmd.clear();
      }
      _depthOfBulk++;

      return;
    }
    if (command == "}") {
      if (_depthOfBulk != 0) _depthOfBulk--;
      if (_depthOfBulk == 0) {
        _Notify();
        _cmd.clear();
      }
      return;
    }
    if (_depthOfBulk == 0) {
      if (_cmd.empty()) _timestamp = temp_timestamp;
      _cmd.emplace_back(command);
      if (_cmd.size() == _BulkSize) {
        _Notify();
        _cmd.clear();
      }

    } else {
      if (_cmd.empty()) _timestamp = temp_timestamp;
      _cmd.emplace_back(command);
    }
  }
};
class FilePrint : public IObserverCmd  // Prints the observed string into cout
{
 public:
  virtual void handleEvent(const CommandSaver& ref) {
    ofstream fout;
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
class CoutPrint : public IObserverCmd  // Prints the observed string into file
{
 public:
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
class StringPrint : public IObserverCmd  // Prints the observed string into file
{
 private:
  vector<string> _logCmdVector;

 public:
  const vector<string>& get() const { return _logCmdVector; }
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
