
#ifndef ABSTRACT_COMMAND_H_
#define ABSTRACT_COMMAND_H_
#include "Command.h"

namespace zathras::interface {
	class Abstract_command : public Command
	{
	public:
		Abstract_command() {}
		virtual ~Abstract_command() {};
		virtual void execute() = 0;

	};
}
#endif /* ABSTRACT_COMMAND_H_ */
