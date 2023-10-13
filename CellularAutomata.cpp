#include "CellularAutomata.hpp"

CellularAutomata::CellularAutomata(uint32_t w, uint32_t h)
{
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

void CellularAutomata::SimulateEpochs(CA_Mode* mode, int epochs)
{
	for (int i = 0; i < epochs; i++)
		UpdateState(mode);
}

CellularAutomata::Field::Field(uint32_t w, uint32_t h)
{
	width = w;
	height = h;

	cells = new State[w * h];
	memset(cells, (int)State::OFF, sizeof(State) * w * h);
}

CellularAutomata::Field::~Field()
{
	if (cells != nullptr)
		delete[] cells;
}

CellularAutomata::Field::State CellularAutomata::Field::get(uint32_t x, uint32_t y) const
{
	if (x < width && y < height)
		return cells[y * width + x];
	return State::OFF;
}

void CellularAutomata::Field::set(uint32_t x, uint32_t y, State s)
{
	if (x < width && y < height)
		cells[y * width + x] = s;
}

uint32_t CellularAutomata::Field::count_neighbours(uint32_t x, uint32_t y) const
{
	uint32_t count = 0;

	count += uint32_t(get(x - 1, y - 1) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x + 0, y - 1) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x - 1, y + 0) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x - 1, y + 1) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x + 1, y + 0) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x + 0, y + 1) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x + 1, y + 1) == CellularAutomata::Field::State::ON);
	count += uint32_t(get(x + 1, y - 1) == CellularAutomata::Field::State::ON);

	return count;
}

CA_Mode_Rules::CA_Mode_Rules(RuleContainer& rule)
{
	m_Rule = &rule;
}

void CA_Mode_Rules::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	CA_ASSERT(m_Rule, "Rule should not be null");

	for (uint32_t y = 0; y < output->height; y++)
		for (uint32_t x = 0; x < output->width; x++)
		{
			state->set(x, y + 1, (CellularAutomata::Field::State)m_Rule->at({
				char((char)output->get(x - 1, y) + 48),
				char((char)output->get(x, y) + 48),
				char((char)output->get(x + 1, y) + 48)
			}));
		}
}

void CA_Mode_ConwaysGoL::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	for (uint32_t y = 0; y < output->height; y++)
		for (uint32_t x = 0; x < output->width; x++)
		{
			uint32_t count = output->count_neighbours(x, y);
			state->set(x, y, CellularAutomata::Field::State(m_ConwaysStateLookup[(uint32_t)output->get(x, y)][count]));
		}

}

void CA_Mode_BriansBrain::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
{
	for (uint32_t y = 0; y < output->height; y++)
		for (uint32_t x = 0; x < output->width; x++)
		{
			switch (output->get(x, y))
			{
			case CellularAutomata::Field::State::DYING:	state->set(x, y, CellularAutomata::Field::State::OFF);	 break;
			case CellularAutomata::Field::State::ON:	state->set(x, y, CellularAutomata::Field::State::DYING); break;
			case CellularAutomata::Field::State::OFF:
			{
				if (output->count_neighbours(x, y) == 2u)
					state->set(x, y, CellularAutomata::Field::State::ON);
			}
			break;

			}
		}
}

CA_Mode_LangtonsAnt::CA_Mode_LangtonsAnt(int32_t* antPosX, int32_t* antPosY, int32_t* antDir)
{
	m_AntPosX = antPosX;
	m_AntPosY = antPosY;
	m_AntDir = antDir;
}

void CA_Mode_LangtonsAnt::UpdateState(CellularAutomata::Field* state, CellularAutomata::Field* output)
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

void CA_Mode_LangtonsAnt::Turn(Dir dir)
{
	switch (dir)
	{
	case Dir::LEFT:	 (*m_AntDir)++; break;
	case Dir::RIGHT: (*m_AntDir)--; break;
	}

	if (*m_AntDir < 0)
		*m_AntDir = (int)Dir::RIGHT;

	if (*m_AntDir > (int32_t)Dir::RIGHT)
		*m_AntDir = 0;
}
