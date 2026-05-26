#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

struct Difficulty
{
    int gx, gy, mines;
    std::string name;
};

const Difficulty DIFFS[] = {
    {9, 9, 10, "Easy"},
    {16, 16, 40, "Medium"},
    {30, 16, 99, "Hard"}};

struct RecordEntry
{
    std::string diffName;
    int time;
};

enum GameState
{
    MENU,
    PLAYING,
    WON,
    LOST,
    RECORDS_VIEW
};

struct Cell
{
    int value = 0;
    bool isOpen = false, isFlagged = false;
};

// --- ФУНКЦИИ ЛОГИКИ ---

void openCells(std::vector<std::vector<Cell>> &field, int x, int y, int gx, int gy)
{
    if (x < 0 || x >= gx || y < 0 || y >= gy || field[x][y].isOpen || field[x][y].isFlagged)
        return;
    field[x][y].isOpen = true;
    if (field[x][y].value == 0)
    {
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
                if (dx != 0 || dy != 0)
                    openCells(field, x + dx, y + dy, gx, gy);
    }
}

bool chordCells(std::vector<std::vector<Cell>> &field, int x, int y, int gx, int gy)
{
    if (!field[x][y].isOpen || field[x][y].value == 0 || field[x][y].value == 9)
        return false;
    int flags = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
        {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < gx && ny >= 0 && ny < gy && field[nx][ny].isFlagged)
                flags++;
        }
    bool hitMine = false;
    if (flags == field[x][y].value)
    {
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
            {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < gx && ny >= 0 && ny < gy && !field[nx][ny].isOpen && !field[nx][ny].isFlagged)
                {
                    if (field[nx][ny].value == 9)
                        hitMine = true;
                    else
                        openCells(field, nx, ny, gx, gy);
                }
            }
    }
    return hitMine;
}

void generateField(std::vector<std::vector<Cell>> &field, int startX, int startY, int gx, int gy, int mines)
{
    int placed = 0;
    while (placed < mines)
    {
        int x = std::rand() % gx, y = std::rand() % gy;
        if ((std::abs(x - startX) <= 1 && std::abs(y - startY) <= 1) || field[x][y].value == 9)
            continue;
        field[x][y].value = 9;
        placed++;
    }
    for (int x = 0; x < gx; x++)
    {
        for (int y = 0; y < gy; y++)
        {
            if (field[x][y].value == 9)
                continue;
            int c = 0;
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < gx && ny >= 0 && ny < gy && field[nx][ny].value == 9)
                        c++;
                }
            field[x][y].value = c;
        }
    }
}

// Загрузка, сортировка и сохранение рекордов
void saveRecord(std::string diffName, int timeVal)
{
    std::vector<RecordEntry> allRecords;
    std::ifstream inFile("records.txt");
    std::string d;
    int t;

    while (inFile >> d >> t)
    {
        allRecords.push_back({d, t});
    }
    inFile.close();

    allRecords.push_back({diffName, timeVal});

    // Сортировка: сначала по названию сложности, потом по времени
    std::sort(allRecords.begin(), allRecords.end(), [](const RecordEntry &a, const RecordEntry &b)
              {
        if (a.diffName != b.diffName) return a.diffName < b.diffName;
        return a.time < b.time; });

    std::ofstream outFile("records.txt");
    for (const auto &r : allRecords)
    {
        outFile << r.diffName << " " << r.time << "\n";
    }
    outFile.close();
}

std::string loadRecordsFormatted()
{
    std::ifstream file("records.txt");
    if (!file.is_open())
        return "No records yet!";

    std::map<std::string, std::vector<int>> grouped;
    std::string d;
    int t;
    while (file >> d >> t)
    {
        grouped[d].push_back(t);
    }
    file.close();

    std::stringstream ss;
    ss << "--- HALL OF FAME ---\n";
    for (auto const &[name, times] : grouped)
    {
        ss << "\n[" << name << "]\n";
        int limit = times.size() > 5 ? 5 : times.size(); // Топ-5 для каждой сложности
        for (int i = 0; i < limit; i++)
        {
            ss << i + 1 << ". " << times[i] << " sec\n";
        }
    }
    return ss.str();
}

sf::Color getNumCol(int v)
{
    if (v == 1)
        return sf::Color::Blue;
    if (v == 2)
        return sf::Color(0, 128, 0);
    if (v == 3)
        return sf::Color::Red;
    return sf::Color(0, 0, 128);
}

// --- MAIN ---

int main()
{
    std::srand(time(0));
    sf::RenderWindow win(sf::VideoMode(400, 500), "Minesweeper C++");
    sf::Font font;
    if (!font.loadFromFile("main.ttf"))
        return -1;

    GameState state = MENU;
    Difficulty currentDiff;
    std::vector<std::vector<Cell>> field;
    bool isFirst = true, recordSaved = false;
    std::clock_t startT;
    int elapsed = 0;
    std::string recordsStr = "";

    while (win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                win.close();

            if (state == MENU && ev.type == sf::Event::MouseButtonPressed)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (ev.mouseButton.x > 100 && ev.mouseButton.x < 300 &&
                        ev.mouseButton.y > 60 + i * 60 && ev.mouseButton.y < 100 + i * 60)
                    {
                        currentDiff = DIFFS[i];
                        field.assign(currentDiff.gx, std::vector<Cell>(currentDiff.gy));
                        win.setSize(sf::Vector2u(currentDiff.gx * 32, currentDiff.gy * 32 + 40));
                        win.setView(sf::View(sf::FloatRect(0, 0, currentDiff.gx * 32, currentDiff.gy * 32 + 40)));
                        state = PLAYING;
                        isFirst = true;
                        recordSaved = false;
                        elapsed = 0;
                    }
                }
                if (ev.mouseButton.x > 100 && ev.mouseButton.x < 300 && ev.mouseButton.y > 240 && ev.mouseButton.y < 280)
                {
                    recordsStr = loadRecordsFormatted();
                    state = RECORDS_VIEW;
                }
            }
            else if (state == RECORDS_VIEW && ev.type == sf::Event::MouseButtonPressed)
                state = MENU;
            else if (state == PLAYING && ev.type == sf::Event::MouseButtonPressed)
            {
                int mx = ev.mouseButton.x / 32;
                int my = (ev.mouseButton.y - 40) / 32;
                if (mx < 0 || mx >= currentDiff.gx || my < 0 || my >= currentDiff.gy)
                    continue;

                bool isChord = (ev.mouseButton.button == sf::Mouse::Middle) ||
                               (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::isButtonPressed(sf::Mouse::Right));

                if (isChord)
                {
                    if (!isFirst && field[mx][my].isOpen && chordCells(field, mx, my, currentDiff.gx, currentDiff.gy))
                        state = LOST;
                }
                else if (ev.mouseButton.button == sf::Mouse::Left)
                {
                    if (field[mx][my].isOpen || field[mx][my].isFlagged)
                        continue;
                    if (isFirst)
                    {
                        generateField(field, mx, my, currentDiff.gx, currentDiff.gy, currentDiff.mines);
                        isFirst = false;
                        startT = std::clock();
                    }
                    if (field[mx][my].value == 9)
                        state = LOST;
                    else
                        openCells(field, mx, my, currentDiff.gx, currentDiff.gy);
                }
                else if (ev.mouseButton.button == sf::Mouse::Right && !isFirst)
                {
                    if (!field[mx][my].isOpen)
                        field[mx][my].isFlagged = !field[mx][my].isFlagged;
                }
                bool winCheck = true;
                for (auto &row : field)
                    for (auto &c : row)
                        if (c.value != 9 && !c.isOpen)
                            winCheck = false;
                if (winCheck && state != LOST)
                    state = WON;
            }
        }

        win.clear(sf::Color(180, 180, 180));

        if (state == MENU)
        {
            win.setSize(sf::Vector2u(400, 500));
            win.setView(sf::View(sf::FloatRect(0, 0, 400, 500)));
            for (int i = 0; i < 3; i++)
            {
                sf::RectangleShape btn(sf::Vector2f(200, 40));
                btn.setPosition(100, 60 + i * 60);
                btn.setFillColor(sf::Color(100, 100, 100));
                win.draw(btn);
                sf::Text txt(DIFFS[i].name, font, 20);
                txt.setPosition(160, 65 + i * 60);
                win.draw(txt);
            }
            sf::RectangleShape rBtn(sf::Vector2f(200, 40));
            rBtn.setPosition(100, 240);
            rBtn.setFillColor(sf::Color(80, 120, 80));
            win.draw(rBtn);
            sf::Text rTxt("Records", font, 20);
            rTxt.setPosition(160, 245);
            win.draw(rTxt);
        }
        else if (state == RECORDS_VIEW)
        {
            sf::Text rList(recordsStr, font, 18);
            rList.setFillColor(sf::Color::Black);
            rList.setPosition(50, 30);
            win.draw(rList);
        }
        else
        {
            if (state == PLAYING && !isFirst)
                elapsed = (std::clock() - startT) / CLOCKS_PER_SEC;
            sf::Text head("Time: " + std::to_string(elapsed), font, 18);
            head.setPosition(10, 10);
            head.setFillColor(sf::Color::Black);
            win.draw(head);

            for (int x = 0; x < currentDiff.gx; x++)
            {
                for (int y = 0; y < currentDiff.gy; y++)
                {
                    sf::RectangleShape tile(sf::Vector2f(30, 30));
                    tile.setPosition(x * 32 + 1, y * 32 + 41);
                    if (state == LOST && field[x][y].value == 9)
                        field[x][y].isOpen = true;
                    tile.setFillColor(field[x][y].isOpen ? sf::Color(220, 220, 220) : sf::Color(80, 80, 80));
                    if (!field[x][y].isOpen && field[x][y].isFlagged)
                        tile.setFillColor(sf::Color::Red);
                    win.draw(tile);
                    if (field[x][y].isOpen)
                    {
                        sf::Text t(field[x][y].value == 9 ? "X" : (field[x][y].value > 0 ? std::to_string(field[x][y].value) : ""), font, 18);
                        t.setPosition(x * 32 + 9, y * 32 + 45);
                        t.setFillColor(field[x][y].value == 9 ? sf::Color::Black : getNumCol(field[x][y].value));
                        win.draw(t);
                    }
                }
            }
            if (state == WON && !recordSaved)
            {
                saveRecord(currentDiff.name, elapsed);
                recordSaved = true;
            }
            if (state != PLAYING)
            {
                sf::Text res(state == WON ? "WIN!" : "BOOM!", font, 20);
                res.setPosition(currentDiff.gx * 32 - 80, 10);
                res.setFillColor(state == WON ? sf::Color::Green : sf::Color::Red);
                win.draw(res);
                sf::Text back("Click to Menu", font, 14);
                back.setPosition(currentDiff.gx * 32 / 2 - 40, 10);
                back.setFillColor(sf::Color::Blue);
                win.draw(back);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    state = MENU;
            }
        }
        win.display();
    }
    return 0;
}