#pragma once
#include "Scene.h"
class Form1 : public Scene {
public:
	Form1() : Scene() { InitializeComponents();  };
protected:
	void InitializeComponents() override;
};
