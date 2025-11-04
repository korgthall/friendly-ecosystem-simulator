#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#define STARTAMOUNT 1000
#define SIZE 55

enum Tasks {
    GATHERGOLD,
    GATHERGLORBIUM,
    BUILD,
    ATTACK,
    IDLE,
    REPAIR,
    MOVING
};

enum ResourceType{
        GOLD,
        GLORBIUM
};

enum BuildingType {
    HUB,
    BARRACKS,
    GLORBSTRACTOR
};

class Position {

    int x=-1;
    int y=-1;

public:

    Position() {
        x = -1;
        y = -1;
    }

    Position(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    Position(const Position &position) {
        x = position.x;
        y = position.y;
    }

    Position &operator=(const Position &p) = default;

    friend std::ostream &operator<<(std::ostream &os, const Position &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }



    void setX(int set_x) {
        x = set_x;
    }

    void setY(int set_y) {
        y = set_y;
    }

    [[nodiscard]] int getX() const {
        return x;
    }

    [[nodiscard]] int getY() const {
        return y;
    }
};

class Resource {
    int amount;
    Position position = Position(0, 0);
    ResourceType type;
    public:

    Resource(const int amount, const Position &position,ResourceType type)
        : amount(amount),
          position(position),
          type(type){
    }

    Resource(const Resource& current_resource) = default;

    friend std::ostream &operator<<(std::ostream &os, const Resource &resource) {
        os << "Resource: Amount=" << resource.amount << " Position=" << resource.position << " Type=" << (resource.type == GOLD ? "GOLD" : "GLORBIUM");
        return os;
    }

    [[nodiscard]] int getAmount() const {
        return amount;
    }

    void setAmount(int current_amount) {
        amount = current_amount;
    }

    [[nodiscard]] Position getPosition() const {
        return position;
    }

    [[nodiscard]] ResourceType getType() const {
        return type;
    }

    void setPosition(const Position &current_position) {
        position = current_position;
    }
};


class Building {
    Position position = Position(-1,-1);
    int hitPoints;
    int owner=0;
    BuildingType type;
    public:
    Building(const Position &build_position, const BuildingType &build_type, const int hit_points,
             const int owner) : position(build_position), hitPoints(hit_points), owner(owner),
            type(build_type) {}

    friend std::ostream &operator<<(std::ostream &os, const Building &b) {
        os << "Building: Position=" << b.position << " HitPoints=" << b.hitPoints << " Owner=" << b.owner << " Type=" << (b.type == HUB ? "HUB" : (b.type == BARRACKS ? "BARRACKS" : "GLORBSTRACTOR"));
        return os;
    }


    [[nodiscard]] int getOwner() const {
        return owner;
    }
    void setPosition(const Position &new_position) {
        position = new_position;
    }

    void setOwner(const int new_owner) {
        owner = new_owner;
    }
    [[nodiscard]] Position getPosition() const {
        return position;
    }

    [[nodiscard]] BuildingType getType() const {
        return type;
    }
};

class Unit {

    Position position;
    int hitPoints;
    int attackDamage;
    int attackRange;
    int visionRange;
    int speed;
    int owner=0; // 1 for player 1, 2 for player 2, 0 for neutral
public:

    Unit() : hitPoints(0), attackDamage(0), attackRange(0), visionRange(0), speed(0) {
    }

    Unit(const int hit_points, const int attack_damage, const int attack_range, const int vision_range,const int speed)
        : hitPoints(hit_points),
          attackDamage(attack_damage),
          attackRange(attack_range),
          visionRange(vision_range), speed(speed) {
    }
    Unit(const int hit_points, const int attack_damage, const int attack_range, const int vision_range,const int speed,const int owner)
        : hitPoints(hit_points),
          attackDamage(attack_damage),
          attackRange(attack_range),
          visionRange(vision_range), speed(speed), owner(owner) {
    }
    explicit Unit(const Position & position) : hitPoints(0), attackDamage(0), attackRange(0), visionRange(0), speed(0){
    this->position = position;
    }

    Unit &operator=(const Position &new_position) {
        position = new_position;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Unit &unit) {
        os << "Unit: Hit-points=" << unit.hitPoints << " Damage=" << unit.attackDamage << " Attack range=" << unit.attackRange << " Vision range=" << unit.visionRange << " Position=" << unit.position << " Speed=" << unit.speed << " Owner=" << unit.owner;
        return os;
    }


    [[nodiscard]] int getSpeed() const {
        return speed;
    }
    [[nodiscard]] int getHitPoints() const {
        return hitPoints;
    }

    [[nodiscard]] int getAttackDamage() const {
        return attackDamage;
    }

    [[nodiscard]] int getAttackRange() const {
        return attackRange;
    }

    [[nodiscard]] int getVisionRange() const {
        return visionRange;
    }

    [[nodiscard]] Position getPosition() const {
        return position;
    }
    void setHitPoints(int hit_points) {
        hitPoints = hit_points;
    }

    void setAttackDamage(int attack_damage) {
        attackDamage = attack_damage;
    }

    void setAttackRange(int attack_range) {
        attackRange = attack_range;
    }

    void setVisionRange(int vision_range) {
        visionRange = vision_range;
    }

    void setPosition(const Position &current_position) {
        position = current_position;
    }

    [[nodiscard]] int getOwner() const {
        return owner;
    }
};

class Worker : public Unit {

    Tasks task;
    int totalCapacity;
    int currentCapacity;
    int gatherSpeed; /// amount/time_unit
    public:

    explicit Worker() : task(IDLE), totalCapacity(10), currentCapacity(0), gatherSpeed(2) {
    }
    explicit Worker(const Position& position) : Unit(position),task(IDLE), totalCapacity(10), currentCapacity(0), gatherSpeed(2) {
    }
    explicit Worker(const Tasks task, const int total_capacity,
           const int current_capacity, const int gather_speed,
           const Position& position) :
    Unit(position), task(task),totalCapacity(total_capacity),currentCapacity(current_capacity),gatherSpeed(gather_speed)
    {}

    friend std::ostream &operator<<(std::ostream &os, const Worker &w) {
        os << "Worker: Task=" << w.task << " TotalCapacity=" << w.totalCapacity << " CurrentCapacity=" << w.currentCapacity << " GatherSpeed=" << w.gatherSpeed << " Position=" << w.getPosition();
        return os;
    }

    [[nodiscard]] Tasks getTask() const {
        return task;
    }

    void setTask(Tasks current_task) {
        task = current_task;
    }

    [[nodiscard]] int getTotalCapacity() const {
        return totalCapacity;
    }

    void setTotalCapacity(int total_capacity) {
        totalCapacity = total_capacity;
    }

    [[nodiscard]] int getCurrentCapacity() const {
        return currentCapacity;
    }

    void setCurrentCapacity(int current_capacity) {
        currentCapacity = current_capacity;
    }

    [[nodiscard]] int getGatherSpeed() const {
        return gatherSpeed;
    }

    void setGatherSpeed(int gather_speed) {
        gatherSpeed = gather_speed;
    }

};

class Tile {
    Position position;
    Building* building = nullptr;
    Unit* unit = nullptr;
    Resource* resource = nullptr;
    bool isOccupied = false;

public:
    Tile() = default;
    Tile(int x, int y) : position(x, y) {}
    Tile(const Tile& tile) = default;
    Tile& operator=(const Tile &tile) = default;

    friend std::ostream &operator<<(std::ostream &os, const Tile &t) {
        os << "Tile: Position=" << t.position << " IsOccupied=" << t.isOccupied;
        if (t.building) os << " Building=" << *t.building;
        if (t.unit) os << " Unit=" << *t.unit;
        if (t.resource) os << " Resource=" << *t.resource;
        return os;
    }

    // ~Tile() {
    //     delete building;
    //     delete unit;
    //     delete resource;
    // }

    [[nodiscard]] bool isTileOccupied() const {
        return isOccupied;
    }

    void placeBuilding(Building* newBuilding) {
        if (isOccupied) {
            return;
        }
        building = newBuilding;
        unit = nullptr;
        isOccupied = true;
        building->setPosition(getPosition());
    }

    void placeUnit(Unit* newUnit) {
        if (isOccupied) {
            return;
        }
        unit = newUnit;
        unit->setPosition(position);
        building = nullptr;
        isOccupied = true;
    }

    void placeResource(Resource* newResource) {
        if (isOccupied) {
            return;
        }
        resource = newResource;
        unit = nullptr;
        building = nullptr;
        isOccupied = true;
    }

    void clearTile() {
        building = nullptr;
        unit = nullptr;
        isOccupied = false;
    }

    // [[nodiscard]] Building * getBuilding() const {
    //     return building;
    // }

    // [[nodiscard]] Building* getBuildingPtr() const { return building; }
    // [[nodiscard]] Unit* getUnit() const { return unit; }
     [[nodiscard]] Position getPosition() const { return position; }
    [[nodiscard]] bool hasBuilding() const { return building != nullptr; }
    [[nodiscard]] bool hasUnit() const { return unit != nullptr; }
    [[nodiscard]] ResourceType getResourceType() const { return resource ? resource->getType() : GOLD; }
};

class GameEngine {
    int gameTicks=0;
    std::vector<Unit> units;
    std::vector<Building> buildings;
    std::vector<Resource> resourceNodes;
    std::vector<Worker> workers;
    std::vector<std::vector<Position>> workerPaths;
    Tile tiles[SIZE][SIZE];
    //int playerResources[2][2] = {0};

    // Resource* findResourceAtPosition(const Position& pos) {
    //     for (auto& resource : resourceNodes) {
    //         if (resource.getPosition().getX() == pos.getX() &&
    //             resource.getPosition().getY() == pos.getY()) {
    //             return &resource;
    //             }
    //     }
    //     return nullptr;
    // }

    // Building* findBuildingAtPosition(const Position& pos) {
    //     for (auto& building : buildings) {
    //         Position buildingPos = building.getPosition();
    //         if (buildingPos.getX() == pos.getX() && buildingPos.getY() == pos.getY()) {
    //             return &building;
    //         }
    //     }
    //     return nullptr;
    // }

    // [[nodiscard]] Position findPlayerHub(int owner) const {
    //     for (const auto& building : buildings) {
    //         if (building.getOwner() == owner && building.getType() == HUB) {
    //             return building.getPosition();
    //         }
    //     }
    //     Position position{-1,-1};
    //     return position;
    // }
    // bool isPlayerHub(const Position& pos, int owner) {
    //     Building* building = findBuildingAtPosition(pos);
    //     return (building && building->getOwner() == owner && building->getType() == HUB);
    // }

    // [[nodiscard]] bool isResourceNode(const Position& pos, const ResourceType resource) const {
    //     return tiles[pos.getX()][pos.getY()].getResourceType()==resource;
    // }

    void moveUnit(Unit& unit,const Position& new_position) {
        int old_x = unit.getPosition().getX();
        int old_y = unit.getPosition().getY();
        int new_x = new_position.getX();
        int new_y = new_position.getY();

        tiles[old_x][old_y].clearTile();
        unit.setPosition(new_position);
        tiles[new_x][new_y].placeUnit(&unit);

        std::cout << "Worker moved to: " << new_position << std::endl;
    }

    // void HandleGathering() {
    //     int workerIndex = -1;
    //     for (auto& worker : workers) {
    //         workerIndex++;
    //         if (worker.getTask() == GATHERGOLD && worker.getCurrentCapacity()<worker.getTotalCapacity()) {
    //             for (auto & i : tiles) {
    //                 Position position(i->getPosition());
    //                 if(isResourceNode(position,GOLD)) {
    //                     Position endPosition=i->getPosition();
    //                     Position startPosition=worker.getPosition();
    //                     workerPaths.emplace_back(PathTo(startPosition,endPosition));
    //                     worker.setTask(MOVING);
    //                 }
    //             }
    //         }
    //         if (worker.getTask() == MOVING) {
    //             int moves = worker.getSpeed();
    //             for (auto i = 1; i <= moves; i++) {
    //                 if (workerPaths[workerIndex].empty()) {
    //                     worker.setTask(IDLE);
    //                     break;
    //                 }
    //                 moveUnit(worker,workerPaths[workerIndex][0]);
    //                 workerPaths.pop_back();
    //             }
    //         }
    //     }
    // }

    [[nodiscard]] std::vector<Position> PathTo(const Position& start_position, const Position& end_position) const {
    std::vector<Position> path;

    int start_x = start_position.getX();
    int start_y = start_position.getY();
    int end_x = end_position.getX();
    int end_y = end_position.getY();
    std::vector<std::vector<int>> grid(SIZE, std::vector<int>(SIZE, 0));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = tiles[i][j].isTileOccupied() ? 1 : 0;
        }
    }

    std::vector<std::vector<Position>> parent(SIZE, std::vector<Position>(SIZE, Position(-1, -1)));
    std::vector<std::vector<bool>> visited(SIZE, std::vector<bool>(SIZE, false));
    std::queue<Position> queue;

    queue.push(start_position);
    visited[start_x][start_y] = true;
    parent[start_x][start_y] = Position(-1, -1);

    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();

        int x = current.getX();
        int y = current.getY();

        if (x == end_x && y == end_y) {
            Position pos = current;
            while (!(pos.getX() == start_x && pos.getY() == start_y)) {
                path.push_back(pos);
                pos = parent[pos.getX()][pos.getY()];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            const int dy[] = {0, 0, -1, 1};
            const int dx[] = {-1, 1, 0, 0};
            int nx = x + dx[i];
            int ny = y + dy[i];


            if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && grid[nx][ny] == 0 && !visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = current;
                queue.emplace(nx, ny);
            }
        }
    }

    return path;
}

public:

    void StartGame() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                tiles[i][j] = Tile(i, j);
            }
        }

        Position position = Position(3, 3);
        int hitPoints = 2000;
        int red = 1;
        int blue = 2;

        buildings.emplace_back(position, HUB, hitPoints, red);
        tiles[position.getX()][position.getY()].placeBuilding(&buildings.back());

        position = Position(52, 52);
        buildings.emplace_back(position, HUB, hitPoints, blue);
        tiles[position.getX()][position.getY()].placeBuilding(&buildings.back());

        for (int indexResource = 1; indexResource <= 3 ; indexResource++) {
            position = Position(indexResource,1);
            resourceNodes.emplace_back(STARTAMOUNT, position, GOLD);
            tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());

            position = Position(1,indexResource);
            resourceNodes.emplace_back(STARTAMOUNT, position, GOLD);
            tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());

            position = Position(SIZE-1,SIZE-indexResource);
            resourceNodes.emplace_back(STARTAMOUNT, position, GOLD);
            tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());

            position = Position(SIZE-indexResource,SIZE-1);
            resourceNodes.emplace_back(STARTAMOUNT, position, GOLD);
            tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());
        }

        position = Position(3,5);
        resourceNodes.emplace_back(STARTAMOUNT, position, GLORBIUM);
        tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());

        position = Position(50,52);
        resourceNodes.emplace_back(STARTAMOUNT, position, GLORBIUM);
        tiles[position.getX()][position.getY()].placeResource(&resourceNodes.back());

        for (int indexWorker = 2; indexWorker <= 4; indexWorker++) {
            position = Position(indexWorker,4);
            workers.emplace_back(position);
            tiles[position.getX()][position.getY()].placeUnit(&workers.back());

            position = Position(SIZE-indexWorker,SIZE-4);
            workers.emplace_back(position);
            tiles[position.getX()][position.getY()].placeUnit(&workers.back());
        }

        position = Position(5,4);
        workers.emplace_back(GATHERGOLD,10,0,2,position);
        tiles[position.getX()][position.getY()].placeUnit(&workers.back());

        std::cout << "Game started with " << workers.size() << " workers" << std::endl;
        std::cout << "First worker position: " << workers.front().getPosition() << std::endl;
    }

    void Update() {
        if (gameTicks == 0 && !workers.empty()) {
            Position target(30, 30);
            std::vector<Position> path = PathTo(workers[0].getPosition(), target);

            if (!path.empty()) {
                std::cout << "Path found with " << path.size() << " steps" << std::endl;


                int steps = std::min(100, static_cast<int>(path.size()));
                for (int i = 0; i < steps; i++) {
                    moveUnit(workers[0], path[i]);
                }
            } else {
                std::cout << "No path found to target!" << std::endl;
            }
        }

        gameTicks++;

        if (gameTicks > 100) {
            std::cout << "Game ending after demonstration..." << std::endl;
        }
    }

    void EndGame() {
        units.clear();
        buildings.clear();
        resourceNodes.clear();
        workers.clear();
        workerPaths.clear();
    }

    [[nodiscard]] bool GameShouldEnd() const {
        return gameTicks > 100;
    }
};

int main() {
    // Test all getters and setters
    Position pos(10, 20);
    std::cout << "Original Position: " << pos << std::endl;
    pos.setX(15);
    pos.setY(25);
    std::cout << "Modified Position: " << pos << std::endl;
    std::cout << "X: " << pos.getX() << ", Y: " << pos.getY() << std::endl;

    Resource res(100, Position(5, 5), GOLD);
    std::cout << "Resource: " << res << std::endl;
    res.setAmount(200);
    res.setPosition(Position(6, 6));
    std::cout << "Modified Resource - Amount: " << res.getAmount() << ", Position: " << res.getPosition() << ", Type: " << res.getType() << std::endl;

    Building building(Position(1, 1), HUB, 1000, 1);
    std::cout << "Building: " << building << std::endl;
    building.setOwner(2);
    building.setPosition(Position(2, 2));
    std::cout << "Modified Building - Owner: " << building.getOwner() << ", Position: " << building.getPosition() << ", Type: " << building.getType() << std::endl;

    Unit unit(100, 10, 5, 10, 2, 1);
    std::cout << "Unit: " << unit << std::endl;
    unit.setHitPoints(150);
    unit.setAttackDamage(15);
    unit.setAttackRange(6);
    unit.setVisionRange(12);
    unit.setPosition(Position(3, 3));
    std::cout << "Modified Unit - HP: " << unit.getHitPoints() << ", Damage: " << unit.getAttackDamage()
              << ", Range: " << unit.getAttackRange() << ", Vision: " << unit.getVisionRange()
              << ", Speed: " << unit.getSpeed() << ", Owner: " << unit.getOwner() << std::endl;

    Worker worker(GATHERGOLD, 20, 5, 3, Position(4, 4));
    std::cout << "Worker: " << worker << std::endl;
    worker.setTask(MOVING);
    worker.setTotalCapacity(25);
    worker.setCurrentCapacity(10);
    worker.setGatherSpeed(4);
    std::cout << "Modified Worker - Task: " << worker.getTask() << ", TotalCap: " << worker.getTotalCapacity()
              << ", CurrentCap: " << worker.getCurrentCapacity() << ", GatherSpeed: " << worker.getGatherSpeed() << std::endl;

    Tile tile(7, 7);
    std::cout << "Tile: " << tile << std::endl;
    std::cout << "Tile Position: " << tile.getPosition() << ", HasBuilding: " << tile.hasBuilding()
              << ", HasUnit: " << tile.hasUnit() << ", ResourceType: " << tile.getResourceType() << std::endl;

    GameEngine game;
    game.StartGame();

    while (!game.GameShouldEnd()) {
        game.Update();
    }

    game.EndGame();
    std::cout << "Game ended successfully!" << std::endl;
    return 0;
}
