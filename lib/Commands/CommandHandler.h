#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H

#define COMMAND_DELIMITER '\n'
#define ARG_DELIMITER " "
#define BUFFER_SIZE 200
#define MAX_ARGS 5
#define ARG_SIZE 50

#include <map>
#include <string>
#include <vector>
#include <HardwareSerial.h>

#include <functional>
class CommandHandler;
typedef char argv_t[MAX_ARGS][ARG_SIZE];
typedef std::function<void(CommandHandler *)> Command;

class CommandHandler
{
  Command defaultCommand = [](CommandHandler *handler) { handler->serial->printf("Unknown command '%s'\n", handler->argv[0]); };
  char buffer[BUFFER_SIZE];
  std::map<std::string, Command> commands;

public:
  HardwareSerial *serial;
  size_t argc;
  argv_t argv;

  CommandHandler(HardwareSerial *serial) : serial(serial)
  {
  }

  void _default(Command &command)
  {
    defaultCommand = command;
  }

  void command(char *name, const Command &command)
  {
    commands.emplace(name, command);
  }

  void update()
  {
    if (serial->available())
    {
      int read = serial->readBytesUntil(COMMAND_DELIMITER, buffer, BUFFER_SIZE - 1);
      if (read == 0)
        return;
      buffer[read] = '\0';

      int argc = parseArgs();
      if (argc == 0)
        return;

      if (commands.count(argv[0]) == 0)
        defaultCommand(this);
      else
        commands[argv[0]](this);
    }
  }

private:
  int parseArgs()
  {
    int idx = 0;
    for (char *arg = strtok(buffer, ARG_DELIMITER); arg && idx < MAX_ARGS; arg = strtok(NULL, ARG_DELIMITER))
    {
      strncpy(argv[idx++], arg, ARG_SIZE);
    }
    return idx;
  }
};

#endif
