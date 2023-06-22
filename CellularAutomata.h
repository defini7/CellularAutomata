#pragma once

//#include <cstdint>
//#include <cstring>
#include <cassert>
#include <unordered_map>
#include <string>

#ifndef CA_ASSERT
#define CA_ASSERT assert
#endif

using RuleContainer = std::unordered_map<std::string, bool>;

inline RuleContainer RULE_NONE;
inline RuleContainer RULE_110 = { { "111", 0 }, { "110", 1 }, { "101", 1 }, { "100", 0 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
inline RuleContainer RULE_30 =  { { "111", 0 }, { "110", 0 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
inline RuleContainer RULE_184 = { { "111", 1 }, { "110", 0 }, { "101", 1 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 0 }, { "000", 0 } };
inline RuleContainer RULE_90 =  { { "111", 0 }, { "110", 1 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 1 }, { "000", 0 } };

class CellularAutomata
{
public:
	enum class Mode
	{
		NONE,
		RULES,
		CONWAYSGOL,
		BRIANSBRAIN,
		LANGTONSANT
	};

	CellularAutomata() = default;
	CellularAutomata(uint32_t w, uint32_t h, Mode m);
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

		State get(int x, int y);
		void set(int x, int y, State s);
	};

	enum class Dir { Up, Left, Down, Right };

	Field* output;
	Field* state;

	uint32_t width;
	uint32_t height;

	Mode mode = Mode::NONE;

	int antPosX = 0;
	int antPosY = 0;

	int antDir = 0;

private:
	std::vector<std::vector<int>> m_ConwaysStateLookup =
	{
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0 }, // dead
		{ 0, 0, 1, 1, 0, 0, 0, 0, 0 }  // alive
	};

public:
	bool UpdateState(RuleContainer& rule = RULE_NONE);
	void UpdateBoth(int x, int y, Field::State s);

};

