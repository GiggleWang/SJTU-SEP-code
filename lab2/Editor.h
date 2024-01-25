#include <string>
#include "Buffer.h"

class Editor {
private:
  Buffer *buffer;
  int currentLine=0;

  void dispatchCmd(const string &cmd);
  void cmdAppend();
  void cmdInsert();
  void cmdDelete(int start, int end);
  void cmdNumber(int start, int end);
  void cmdWrite(const string &filename);
  void cmdNull(int line);
  void cmdAll();
  int returnCurrentLine()
  {
      return currentLine;
  }
public:
  Editor();
  ~Editor();
  void run();
};
