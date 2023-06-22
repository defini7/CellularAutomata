#pragma once

//#include <cstdint>
//#include <cstring>
#include <cassert>
#include <unordered_map>
#include <string>

#ifndef CA_ASSERT
#define CA_ASSERT(expr, msg) assert(expr && msg)
#endif

class CA_Mode;

class CellularAutomata
{
public:
	CellularAutomata() = default;
	CellularAutomata(uint32_t w, uint32_t h);
	~CellularAutomata();

public:
	struct Field
	{
		enum class State { ON = 1, OFF = 0, DYING = 3 };

		Field() = default;
		Field(int w, int h);
		~Field();

		State* cells = nullptr;
		int width = 0;
		int height = 0;

		State get(int x, int y) const;
		void set(int x, int y, State s);

		int count_neigh(int x, int y) const;
	};

	Field* output;
	Field* state;

	uint32_t width;
	uint32_t height;

public:
	void UpdateState(CA_Mode* mode);
	void UpdateBoth(int x, int y, Field::State s);
	
};

class CA_Mode
{
public:
	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) = 0;
};

class CA_Mode_Rules : public CA_Mode
{
public:
	using RuleContainer = std::unordered_map<std::string, bool>;

	CA_Mode_Rules(RuleContainer& rule);

	static inline RuleContainer RNONE;
	static inline RuleContainer R110 =	{ { "111", 0 }, { "110", 1 }, { "101", 1 }, { "100", 0 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
	static inline RuleContainer R30 =	{ { "111", 0 }, { "110", 0 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
	static inline RuleContainer R184 =	{ { "111", 1 }, { "110", 0 }, { "101", 1 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 0 }, { "000", 0 } };
	static inline RuleContainer R90 =	{ { "111", 0 }, { "110", 1 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 1 }, { "000", 0 } };

public:
	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) override;

private:
	RuleContainer* m_Rule = nullptr;

};

class CA_Mode_ConwaysGoL : public CA_Mode
{
public:
	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) override;

private:
	std::vector<std::vector<int>> m_ConwaysStateLookup =
	{
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0 }, // dead
		{ 0, 0, 1, 1, 0, 0, 0, 0, 0 }  // alive
	};

};

class CA_Mode_BriansBrain : public CA_Mode
{
public:
	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) override;

};

class CA_Mode_LanctonsAnt : public CA_Mode
{
public:
	enum class Dir { UP, LEFT, DOWN, RIGHT };

	CA_Mode_LanctonsAnt(int* antPosX, int* antPosY, int* antDir);

	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) override;

private:
	void Turn(Dir dir);
	
private:
	int* m_AntPosX = nullptr;
	int* m_AntPosY = nullptr;

	int* m_AntDir = nullptr;

};
