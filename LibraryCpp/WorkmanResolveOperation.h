#pragma once

class Workman;
enum WorkmanType;

class WorkmanResolveOperation
{
public:
	static Workman* Resolve(WorkmanType w);
};