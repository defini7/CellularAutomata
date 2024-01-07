#ifndef CELLULAR_AUTOMATA_HPP
#define CELLULAR_AUTOMATA_HPP

//#include <cstdint>
//#include <cstring>
#include <unordered_map>
#include <string>

#ifndef CA_ASSERT
#include <cassert>
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
		Field(uint32_t w, uint32_t h);
		~Field();

		State* cells = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;

		State get(uint32_t x, uint32_t y) const;
		void set(uint32_t x, uint32_t y, State s);

		uint32_t count_neighbours(uint32_t x, uint32_t y) const;
	};

	Field* output;
	Field* state;

	uint32_t width;
	uint32_t height;

public:
	void UpdateState(CA_Mode* mode);
	void UpdateBoth(int x, int y, Field::State s);

	void SimulateEpochs(CA_Mode* mode, int epochs);

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
	static inline RuleContainer R110 = { { "111", 0 }, { "110", 1 }, { "101", 1 }, { "100", 0 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
	static inline RuleContainer R30 = { { "111", 0 }, { "110", 0 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 1 }, { "001", 1 }, { "000", 0 } };
	static inline RuleContainer R184 = { { "111", 1 }, { "110", 0 }, { "101", 1 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 0 }, { "000", 0 } };
	static inline RuleContainer R90 = { { "111", 0 }, { "110", 1 }, { "101", 0 }, { "100", 1 }, { "011", 1 }, { "010", 0 }, { "001", 1 }, { "000", 0 } };

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

class CA_Mode_LangtonsAnt : public CA_Mode
{
public:
	enum class Dir { UP, LEFT, DOWN, RIGHT };

	CA_Mode_LangtonsAnt(int32_t* antPosX, int32_t* antPosY, Dir* antDir);

	virtual void UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output) override;

private:
	void Turn(Dir dir);

private:
	int* m_AntPosX = nullptr;
	int* m_AntPosY = nullptr;

	Dir* m_AntDir = nullptr;

};

#endif
