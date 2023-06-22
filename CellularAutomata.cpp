#include "CellularAutomata.h"

CellularAutomata::CellularAutomata(uint32_t w, uint32_t h, Mode m)
{
	CA_ASSERT(w > 0 && h > 0 && w < UINT32_MAX && h < UINT32_MAX);

	width = w;
	height = h;

	output = new Field(w, h);
	state = new Field(w, h);

	mode = m;
}

CellularAutomata::~CellularAutomata()
{
	delete output;
	delete state;
}

bool CellularAutomata::UpdateState(RuleContainer& rule)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			output->set(x, y, state->get(x, y));

	switch (mode)
	{
	case Mode::RULES:
	{
		if (rule == RULE_NONE) return false;

		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				int c = (int)output->get(x, y);
				int l = (int)output->get(x - 1, y);
				int r = (int)output->get(x + 1, y);

				state->set(x, y + 1, (Field::State)rule[{ char(l + 48), char(c + 48), char(r + 48) }]);
			}
	}
	break;

	case Mode::CONWAYSGOL:
	{
		CA_ASSERT(rule == RULE_NONE && "Mode was set to CONWAYSGOL, but the rule was specified.");
		
		auto neighs = [&](int x, int y)
		{
			int ñount = 0;

			ñount += int(output->get(x - 1, y - 1) == Field::State::ON);
			ñount += int(output->get(x + 0, y - 1) == Field::State::ON);
			ñount += int(output->get(x - 1, y + 0) == Field::State::ON);
			ñount += int(output->get(x - 1, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y + 0) == Field::State::ON);
			ñount += int(output->get(x + 0, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y - 1) == Field::State::ON);

			return ñount;
		};

		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				int count = neighs(x, y);
				state->set(x, y, Field::State(m_ConwaysStateLookup[(int)output->get(x, y)][count]));
			}
	}
	break;

	case Mode::BRIANSBRAIN:
	{
		CA_ASSERT(rule == RULE_NONE && "Mode was set to BRIANSBRAIN, but the rule was specified.");

		auto neighs = [&](int x, int y)
		{
			int ñount = 0;

			ñount += int(output->get(x - 1, y - 1) == Field::State::ON);
			ñount += int(output->get(x + 0, y - 1) == Field::State::ON);
			ñount += int(output->get(x - 1, y + 0) == Field::State::ON);
			ñount += int(output->get(x - 1, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y + 0) == Field::State::ON);
			ñount += int(output->get(x + 0, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y + 1) == Field::State::ON);
			ñount += int(output->get(x + 1, y - 1) == Field::State::ON);

			return ñount;
		};

		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				switch (output->get(x, y))
				{
				case Field::State::DYING:	state->set(x, y, Field::State::OFF);	break;
				case Field::State::ON:		state->set(x, y, Field::State::DYING);	break;
				case Field::State::OFF:
				{
					if (neighs(x, y) == 2)
						state->set(x, y, Field::State::ON);
				}
				break;

				}
			}
	}
	break;

	case Mode::LANGTONSANT:
	{
		state->set(antPosX, antPosY, Field::State(!(bool)output->get(antPosX, antPosY)));

		auto turn = [&](Dir dir)
		{
			switch (dir)
			{
			case Dir::LEFT:	 antDir++; break;
			case Dir::RIGHT: antDir--; break;
			}

			if (antDir < 0)					antDir = (int)Dir::RIGHT;
			if (antDir > (int)Dir::RIGHT)	antDir = 0;
		};

		switch (output->get(antPosX, antPosY))
		{
		case Field::State::ON:	turn(Dir::LEFT);  break;
		case Field::State::OFF: turn(Dir::RIGHT); break;
		}

		switch ((Dir)antDir)
		{
		case Dir::UP:	 antPosY--; break;
		case Dir::DOWN:  antPosY++; break;
		case Dir::LEFT:	 antPosX--; break;
		case Dir::RIGHT: antPosX++; break;
		}
	}
	break;

	};

	return true;
}

void CellularAutomata::UpdateBoth(int x, int y, CellularAutomata::Field::State s)
{
	state->set(x, y, s);
	output->set(x, y, s);
}

CellularAutomata::Field::Field(int w, int h)
{
	CA_ASSERT(w > 0 && h > 0 && w < INT32_MAX && h < INT32_MAX);

	width = w;
	height = h;

	cells = new State[w * h];
	for (int i = 0; i < w * h; i++) cells[i] = State::OFF;
}

CellularAutomata::Field::~Field()
{
	if (cells != nullptr) delete[] cells;
}

CellularAutomata::Field::State CellularAutomata::Field::get(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		return cells[y * width + x];
	return State::OFF;
}

void CellularAutomata::Field::set(int x, int y, State s)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		cells[y * width + x] = s;
}
