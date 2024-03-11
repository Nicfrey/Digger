#pragma once
#include <memory>
#include <vector>

#include "Singleton.h"

class Command;

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	private:
		std::vector<std::shared_ptr<Command>> m_Commands;
	};

}
