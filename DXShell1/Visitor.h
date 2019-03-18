#pragma once

/*
Class Name: Visitor
Purpose: This class implements the Visitor Design pattern.
	In this program, I'm using it to allow Physics and InputComponenets to recover lost type information
*/
class Visitor
{
public:
	Visitor() {}
	virtual ~Visitor() {}

	virtual void visit(class GameObject* gameobject) = 0;
	virtual void visit(class Actor* actor) = 0;
};

class VisitorComponent
{

public:
	VisitorComponent() {}
	virtual ~VisitorComponent() {}

	virtual void accept(Visitor& visitor) = 0;
};

