#pragma once
#include <memory>
class Command;

enum TriggerType
{
	KeyDown,
	KeyUp,
	KeyPressed,
	Thumbs
};

class Controller
{
public:
	virtual ~Controller() = default;
	virtual void ProcessInput() = 0;
	virtual void BindAction(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown) = 0;
	virtual void UnbindActionGameObject() = 0;
	virtual void UnbindActionGameObjectDestroyed() = 0;
};

class GamepadController : public Controller
{
public:
	GamepadController(int index);
	~GamepadController() override;
	void ProcessInput() override;
	void BindAction(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown) override;
	void UnbindActionGameObject() override;
	void UnbindActionGameObjectDestroyed() override;
private:
	class GamepadControllerImpl;
	GamepadControllerImpl* m_pImpl;
};

