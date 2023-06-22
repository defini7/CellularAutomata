#include "CellularAutomata.h"

CellularAutomata::CellularAutomata(uint32_t w, uint32_t h)
{
	CA_ASSERT(w > 0 && h > 0 && w < UINT32_MAX && h < UINT32_MAX, "Width and height should be valid");

	width = w;
	height = h;

	output = new Field(w, h);
	state = new Field(w, h);
}

CellularAutomata::~CellularAutomata()
{
	delete output;
	delete state;
}

void CellularAutomata::UpdateState(CA_Mode* mode)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			output->set(x, y, state->get(x, y));

	mode->UpdateState(state, output);
}

void CellularAutomata::UpdateBoth(int x, int y, CellularAutomata::Field::State s)
{
	state->set(x, y, s);
	output->set(x, y, s);
}

CellularAutomata::Field::Field(int w, int h)
{
	CA_ASSERT(w > 0 && h > 0 && w < INT32_MAX && h < INT32_MAX, "Width and height should be valid");

	width = w;
	height = h;

	cells = new State[w * h];
	for (int i = 0; i < w * h; i++) cells[i] = State::OFF;
}

CellularAutomata::Field::~Field()
{
	if (cells != nullptr) delete[] cells;
}

CellularAutomata::Field::State CellularAutomata::Field::get(int x, int y) const
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

int CellularAutomata::Field::count_neigh(int x, int y) const
{
	int ñount = 0;

	ñount += int(get(x - 1, y - 1) == CellularAutomata::Field::State::ON);
	ñount += int(get(x + 0, y - 1) == CellularAutomata::Field::State::ON);
	ñount += int(get(x - 1, y + 0) == CellularAutomata::Field::State::ON);
	ñount += int(get(x - 1, y + 1) == CellularAutomata::Field::State::ON);
	ñount += int(get(x + 1, y + 0) == CellularAutomata::Field::State::ON);
	ñount += int(get(x + 0, y + 1) == CellularAutomata::Field::State::ON);
	ñount += int(get(x + 1, y + 1) == CellularAutomata::Field::State::ON);
	ñount += int(get(x + 1, y - 1) == CellularAutomata::Field::State::ON);

	return ñount;
}

CA_Mode_Rules::CA_Mode_Rules(RuleContainer& rule)
{
	m_Rule = &rule;
}

void CA_Mode_Rules::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	CA_ASSERT(m_Rule != nullptr, "Rule should not be RNONE");

	for (int y = 0; y < output->height; y++)
		for (int x = 0; x < output->width; x++)
		{
			int c = (int)output->get(x, y);
			int l = (int)output->get(x - 1, y);
			int r = (int)output->get(x + 1, y);

			state->set(x, y + 1, (CellularAutomata::Field::State)m_Rule->at({ char(l + 48), char(c + 48), char(r + 48) }));
		}
}

void CA_Mode_ConwaysGoL::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	for (int y = 0; y < output->height; y++)
		for (int x = 0; x < output->width; x++)
		{
			int count = output->count_neigh(x, y);
			state->set(x, y, CellularAutomata::Field::State(m_ConwaysStateLookup[(int)output->get(x, y)][count]));
		}
}

void CA_Mode_BriansBrain::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	for (int y = 0; y < output->height; y++)
		for (int x = 0; x < output->width; x++)
		{
			switch (output->get(x, y))
			{
			case CellularAutomata::Field::State::DYING:	state->set(x, y, CellularAutomata::Field::State::OFF);	 break;
			case CellularAutomata::Field::State::ON:	state->set(x, y, CellularAutomata::Field::State::DYING); break;
			case CellularAutomata::Field::State::OFF:
			{
				if (output->count_neigh(x, y) == 2)
					state->set(x, y, CellularAutomata::Field::State::ON);
			}
			break;

			}
		}
}

CA_Mode_LanctonsAnt::CA_Mode_LanctonsAnt(int* antPosX, int* antPosY, int* antDir)
{
	m_AntPosX = antPosX;
	m_AntPosY = antPosY;
	m_AntDir = antDir;
}

void CA_Mode_LanctonsAnt::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	state->set(*m_AntPosX, *m_AntPosY, CellularAutomata::Field::State(!(bool)output->get(*m_AntPosX, *m_AntPosY)));

	switch (output->get(*m_AntPosX, *m_AntPosY))
	{
	case CellularAutomata::Field::State::ON:	Turn(Dir::LEFT);  break;
	case CellularAutomata::Field::State::OFF:	Turn(Dir::RIGHT); break;
	}

	switch (Dir(*m_AntDir))
	{
	case Dir::UP:	 (*m_AntPosY)--; break;
	case Dir::DOWN:  (*m_AntPosY)++; break;
	case Dir::LEFT:	 (*m_AntPosX)--; break;
	case Dir::RIGHT: (*m_AntPosX)++; break;
	}
}

void CA_Mode_LanctonsAnt::Turn(Dir dir)
{
	switch (dir)
	{
	case Dir::LEFT:	 (*m_AntDir)++; break;
	case Dir::RIGHT: (*m_AntDir)--; break;
	}

	if (*m_AntDir < 0)				 *m_AntDir = (int)Dir::RIGHT;
	if (*m_AntDir > (int)Dir::RIGHT) *m_AntDir = 0;
}
