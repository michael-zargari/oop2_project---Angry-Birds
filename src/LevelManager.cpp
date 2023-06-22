#include "LevelManager.h"

LevelManager::LevelManager(std::shared_ptr<World> world) : m_lvlsFile(), m_world{world} {
	m_lvlsFile.open("GameLevels.txt", std::ifstream::in);
}


void LevelManager::getNextLevel(std::vector<std::unique_ptr<Bird>> &birdsVec, std::vector<std::unique_ptr<StaticObjects>>& pigsVec, std::vector<std::unique_ptr<StaticObjects>> &objVec)
{
	birdsVec.clear();
	pigsVec.clear();
	objVec.clear();

	if (!m_lvlsFile.is_open() || !m_lvlsFile.good()) exit(0);  // <- EXIT GAME IF LVLS FINISHED / ENDED

	std::deque<std::string> objDeq{ ReadBirds() };
	birdsVec = std::move(CreateBirds(objDeq));
	 
	objDeq = ReadLevel();
	CreateObj(objDeq, pigsVec, objVec);

}

void LevelManager::getSpecificLevel(const int& lvl, std::vector<std::unique_ptr<Bird>>& birdsVec, std::vector<std::unique_ptr<StaticObjects>>& pigsVec, std::vector<std::unique_ptr<StaticObjects>>& objVec)
{
	birdsVec.clear();
	pigsVec.clear();
	objVec.clear();

	m_lvlsFile.clear();
	m_lvlsFile.seekg(0);
	std::string temp{};
	while (m_lvlsFile.is_open() && m_lvlsFile.good())
	{
		std::getline(m_lvlsFile, temp);
		if (temp.find("Level " + std::to_string(lvl)) != std::string::npos)
		{
			getNextLevel(birdsVec, pigsVec, objVec);
			return;
		}
	}

}
std::deque<std::string> LevelManager::ReadBirds()
{
	std::deque<std::string> objDeq;
	std::string temp;

	while (m_lvlsFile.is_open() && m_lvlsFile.good())
	{
		std::getline(m_lvlsFile, temp);

		if (std::all_of(temp.begin(), temp.end(), [](const char& c) {return c == ' '; }))
			break;
		if(temp.find("Birds") != std::string::npos )objDeq.push_back(temp.substr(temp.find("Birds: ") + 7));
	}
	return objDeq;
}

std::vector<std::unique_ptr<Bird>> LevelManager::CreateBirds(std::deque<std::string>& objDeq)
{
	float deltaX{ 200.f };
	float deltaY{ ROGATKA_Y + 50.f };
	std::vector<std::unique_ptr<Bird>> tempVec;

	while (!objDeq.empty())
	{
		std::string line = objDeq.back();

		for (size_t i = 0; i < line.size(); i++)
		{
			switch (line.at(line.size()-i-1))
			{
			case 'R': tempVec.emplace_back(std::move(ObjectFactory<Bird>::instance().create("RedBird", m_world, sf::Vector2f(ROGATKA_X - deltaX, deltaY), sf::Vector2f(20.f, 0.f), 0)));     break;
			case 'Y': tempVec.emplace_back(std::move(ObjectFactory<Bird>::instance().create("YellowBird", m_world, sf::Vector2f(ROGATKA_X - deltaX, deltaY), sf::Vector2f(20.f, 0.f), 1)));  break;
			case 'B': tempVec.emplace_back(std::move(ObjectFactory<Bird>::instance().create("BlueBird", m_world, sf::Vector2f(ROGATKA_X - deltaX, deltaY), sf::Vector2f(20.f, 0.f), 2)));  break;
			case 'K': tempVec.emplace_back(std::move(ObjectFactory<Bird>::instance().create("BlackBird", m_world, sf::Vector2f(ROGATKA_X - deltaX, deltaY), sf::Vector2f(20.f, 0.f), 3)));  break;

			default: break;
			}
			deltaX += 25.f;
			if (deltaX == 300) {
				deltaX = 200.f;
				deltaY -= 25.f;
			}
		}

		objDeq.pop_back();
	}

	return tempVec;
}

void LevelManager::CreateObj(std::deque<std::string> & objDeq, std::vector<std::unique_ptr<StaticObjects>>& pigsVec, std::vector<std::unique_ptr<StaticObjects>>& objVec)
{
	int xPos = WINDOW_WIDTH * 0.4;
	int yPos = 740-75;
	

	while (!objDeq.empty())
	{
		bool insideLine{ false };
		std::string line = objDeq.back();

		for (size_t i = 0; i < line.size(); i++)
		{
			insideLine = true;
			switch (line.at(i))
			{
			case '/':  break;
			case ' ': if(insideLine) xPos += 30; break;
			case '@': pigsVec.emplace_back(std::move(ObjectFactory<StaticObjects>::instance().create("Pigs",
				m_world, sf::Vector2f(xPos, yPos), sf::Vector2f(20.f, 0.f), 0)));
				xPos += 21;																							break;

			case '!': objVec.emplace_back(std::move(ObjectFactory<StaticObjects>::instance().create("wood",
				m_world, sf::Vector2f(xPos, yPos), sf::Vector2f(150.f, 40.f), 0)));
				objVec.back()->rotate(90); xPos += 41;																break;

			case '-': objVec.emplace_back(std::move(ObjectFactory<StaticObjects>::instance().create("wood",
						m_world, sf::Vector2f(xPos , yPos), sf::Vector2f(200.f, 20.f), 2)));   
				xPos += 202;																						break;

			default: break;
			}	
		}

		yPos -= 85;
		xPos = WINDOW_WIDTH * 0.4;
		objDeq.pop_back();
	}
	
}


std::deque<std::string> LevelManager::ReadLevel()
{
	std::deque<std::string> objDeq;
	std::string temp;
	while (m_lvlsFile.is_open() && m_lvlsFile.good())
	{
		std::getline(m_lvlsFile, temp);
		if (temp.find("=") != std::string::npos) break;

		if (!std::all_of(temp.begin(), temp.end(), [](const char& c) {return c == ' ' || isalnum(c); }))
			objDeq.push_back(temp);

			
	}
	return objDeq;
}

