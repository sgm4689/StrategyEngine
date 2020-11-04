#pragma once
#include <unordered_map>

template <class T>
class ResourceManager
{
public:
	ResourceManager* GetInstance();
	float GetResource(T);
	void AddResource(T, float);
	void SubtractResource(T, float);
private:
	ResourceManager();
	std::unordered_map<T, float> resources;//Map to store resources in.  Resources can be stored as whatever key value works best for the users game
};

