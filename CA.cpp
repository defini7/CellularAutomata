#define DGE_APPLICATION
#include "defGameEngine.h"

#include "CellularAutomata.h"

#include <algorithm>

using namespace std;

class AppDemo : public def::GameEngine
{
public:
	AppDemo()
	{
		SetTitle("Cellular Automata");
	}

	~AppDemo()
	{
		delete automata;
	}

private:
	float fTicks = 0.0f;
	float fDelay = 0.0f;

	CellularAutomata* automata;

protected:
	bool OnUserCreate() override
	{
		automata = new CellularAutomata(ScreenWidth(), ScreenHeight(), CellularAutomata::Mode::CONWAYSGOL);

		/*auto set = [&](int x, int y, std::string v)
		{
			for (int i = 0; i < v.size(); i++)
				automata->state->set(x + i, y, v[i] == '#');
		};*/

		// Gosper's glider gun
		/*set(60, 45, "........................#............");
		set(60, 46, "......................#.#............");
		set(60, 47, "............##......##............##.");
		set(60, 48, "...........#...#....##............##.");
		set(60, 49, "##........#.....#...##...............");
		set(60, 50, "##........#...#.##....#.#............");
		set(60, 51, "..........#.....#.......#............");
		set(60, 52, "...........#...#.....................");
		set(60, 53, "............##.......................");*/

		// Plane
		/*set(60, 45, "###.###");
		set(60, 46, "#.#.#.#");
		set(60, 47, "###.#.#");
		set(60, 48, "..#.#.#");
		set(60, 49, "###.###");*/

		automata->UpdateState(RULE_90);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::SPACE).bPressed)
			automata->UpdateState(RULE_90);

		if (GetKey(def::Key::LEFT_SHIFT).bHeld)
		{
			if (fTicks >= fDelay)
			{
				automata->UpdateState(RULE_90);
				fTicks = 0.0f;
			}
			
			fTicks += fDeltaTime;
		}

		if (GetKey(def::Key::LEFT).bHeld) fDelay -= fDeltaTime;
		if (GetKey(def::Key::RIGHT).bHeld) fDelay += fDeltaTime;

		fDelay = std::clamp(fDelay, 0.0f, 1.0f);

		if (GetMouse(0).bHeld) automata->UpdateBoth(MouseX(), MouseY(), true);
		if (GetMouse(1).bHeld) automata->UpdateBoth(MouseX(), MouseY(), false);
		if (GetMouse(2).bPressed)
		{
			for (int i = 0; i < ScreenWidth(); i++)
				for (int j = 0; j < ScreenHeight(); j++)
					automata->UpdateBoth(i, j, false);
		}

		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, automata->output->get(x, y) ? def::WHITE : def::BLACK);

		DrawString(2, 2, "Delay = " + std::to_string(fDelay), def::YELLOW);

		return true;
	}
};

int main()
{
	srand(clock());

	AppDemo game;

	game.Construct(160, 100, 8, 8);
	game.Run();

	return 0;
}

