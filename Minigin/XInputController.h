#pragma once

namespace dae
{
	class ControllerComponent;

	enum class ControllerButtons
	{
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		DDown = 0x0002,
		DUp = 0x0001,
		DLeft = 0x0004,
		DRight = 0x0008,
	};

	class XInputController final
	{
		class XInputControllerImpl;

	public:
		XInputController(ControllerComponent* pControllerComp);
		~XInputController();

		void Update();

		bool WentDownThisFrame(ControllerButtons button) const;
		bool WentUpThisFrame(ControllerButtons button) const;
		bool IsPressed(ControllerButtons button) const;

	private:
		XInputControllerImpl* m_pImpl;
	};

}