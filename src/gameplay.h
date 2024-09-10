//
// Created by lrfri on 14.05.2024.
//

#ifndef RAYLIBSTARTER_GAMEPLAY_H
#define RAYLIBSTARTER_GAMEPLAY_H

#include <vector>
#include "raylib.h"
#include "assestmanagergraphics.h"
#include "scene.h"
#include "ENEMIES/Enemy.h"
#include "GAME OBJECTS/gameobjects.h"
#include "maincharactermodus.h"
#include "maincharacter.h"
#include "Wall.h"
#include "GAME OBJECTS/stone.h"
#include <map>



class maincharacter;
class gameobjects;
class robot;
class Enemy;
class Enemy1;
class Enemy2;
class Enemy3;
class movableblocks;
class yellowblock;
class blueblock;
class bombs;

struct ActivatedFirebowls {
    int x;
    int y;
    float animationTimer;
    int currentFrame;
    int room;
};
struct ActivatedFirebowl {
    int x;
    int y;
    int room;
};

    class gameplay : public scene {
    private:
        //std::vector<Enemy*> enemies;
        void clearEnemies();
        bool isAlive;

        //firebowls
        std::vector<ActivatedFirebowls> activeFirebowlAnimations;
        std::vector<ActivatedFirebowl> activatedFirebowls;
        float activatedFirebowlAnimationSpeed = 0.1f;
        //movable blocks
        std::map<int, std::vector<movableblocks*>> blocksInRooms;
        std::map<int, std::vector<movableblocks*>> switchesInRooms;

        Texture2D heart_robot = assestmanagergraphics::getTexture("userinterface/heart_robot");
        Texture2D heart_soul = assestmanagergraphics::getTexture("userinterface/heart_soul");
        Texture2D icon_dust = assestmanagergraphics::getTexture("userinterface/icon_dust");
        Texture2D icon_dash = assestmanagergraphics::getTexture("userinterface/icon_dash");
        Texture2D icon_bomb = assestmanagergraphics::getTexture("userinterface/icon_bomb");
        Texture2D icon_stone = assestmanagergraphics::getTexture("userinterface/icon_stone");

    public:

        void update() override;
        void doRoomSwitch();

        scene *evaluateSceneChange() override;

        void draw() override;
        void drawtextonscreen();
        void drawhealthhearts();
        void drawicons();
        void drawmaincharacter();


        void drawDebug() override;

        //loads the necessary textures
        Texture2D heart = assestmanagergraphics::getTexture("userinterface/heart_smaller");

        //loads the textures on the map (Kachelsatz)
        //Texture2D tilesetgrass = assestmanagergraphics::getTexture("tilesets/greyboxing1");
        //Texture2D tileset_room1 = assestmanagergraphics::getTexture("tileset/level1");
        Texture2D tileset_final = assestmanagergraphics::getTexture("tileset/final");

        //attributes necessary for using the map
        std::vector<int> tiles;
        int mapWidth = 25;
        int mapHeight = 15;

        gameplay();

        maincharactermodus currentmodus = soulmodus;

        //stuff to handle character switch and when to show inactive robot
        bool soulleavesrobot=false;
        bool soulentersrobot=false;
        bool soulhasntchangedformsyet=true;
        bool soulcantakeover();
        int takeoverradius =40;
        Vector2 getRobotPosition() const;
        float getTakeoverRadius() const;

        //soul dust
        Texture2D activatedFirebowlTexture=assestmanagergraphics::getTexture("item/souldust");
        bool isAdjacentToFirebowl(Vector2 pos) const;
        std::pair<int, int> getNearestFirebowlTile(Vector2 pos) const;
        void activateFirebowl(int x, int y);
        bool isFirebowlActivated(int x, int y) const;
        void drawActivatedFirebowls(float deltaTime);
        bool areAllFirebowlsActivatedInRoom(int roomNumber) const;
        bool showDoorIsLockedMessage = false;
        void deactivateFirebowls();

        //stones
        std::vector<Stone*> stones;
        void spawnStone(Vector2 position);
        void updateStones();
        void drawStones();
        std::map<int, std::vector<Stone*>> stonesInRooms;
        void spawnStone(int room, Vector2 position);
        bool touchesStone(Vector2 tilePosition) const;
        Stone* getStoneAt(Vector2 mapPosition) const;
        void resetStonesToInitialPositions();
        std::map<int, std::vector<std::pair<Stone*, Vector2>>> initialStonePositions;

        //movable blocks
        void spawnBlock(Vector2 position, bool isYellow);
        void updateBlocks();
        void drawBlocks();
        bool isBlockAt(Vector2 tilePosition) const;
        void toggleBlockAt(Vector2 tilePosition);
        void spawnSwitch(Vector2 position, bool isYellow);
        void updateSwitches();
        void drawSwitches();
        bool isSwitchAt(Vector2 tilePosition) const;
        void toggleSwitchAt(Vector2 tilePosition);
        bool isAdjacentToSwitch(Vector2 position) const;

        //bombs
        std::vector<bombs*> activeBombs;
        void addBomb(bombs* bomb);

        //collision functions
        int getTileAt(float x, float y) const;
        bool touchesWall(Vector2 pos, float size);
        bool touchesAbyss(Vector2 pos, float size);
        bool touchesBars(Vector2 pos, float size);
        Rectangle getTouchedBars(Vector2 position, float size); //radius replaced with size
        Rectangle getTouchedWall(Vector2 position, float size);
        Rectangle getTouchedAbyss(Vector2 position, float size);
        bool isTileYouCantPushStoneOnto(int tileID) const;

        Vector2 touchWallPosition(Vector2 pos, Vector2 size);
        Rectangle getWallAt(Vector2 pos);
        std::vector<Wall*> walls;

        //lets different entities spawn
        const std::vector<Enemy*>& getEnemies() const;
        maincharacter *themaincharacter;
        std::vector<Enemy*> enemies;
        std::vector<gameobjects*> gameobjects;
        robot *therobot;

        //room switch things
        int room=1;
        int robotisinroom=1;
        int soulisinroom=1;
        bool hasbeeninroom1before =false;

        void loadMap();
        void reloadRoom();

        //doors
        float doortextarea=32*3;
        float doorfromroom1to2=32*2;
        float startposroom1to2=14*32+16;
        float doorfromroom2to1=14*32+20;
        float startposroom2to1=2*32+16;
        float doorfromroom2to3=2*32;
        float startposroom2to3=14*32+16;
        float doorfromroom3to2=14*32+20;
        float startposroom3to2=2*32+16;
        float doorfromroom2to4=24*32+16;
        float startposroom2to4=16;
        float doorfromroom4to2=16;
        float startposroom4to2=24*32+16;
        float doorfromroom4to5=2*32;
        float startposroom4to5=14*32+16;
        float doorfromroom5to4=14*32+20;
        float startposroom5to4=3*32;
        float doorfromroom5to6=3*32;
        float startposroom5to6=14*32+16;
        float doorfromroom6to5=14*32+20;
        float startposroom6to5=2*32+16;
        float doorfromroom5to7=32+16;

        bool showDemoMessage=false;

        static const int tree0ID=0;
        static const int tree1ID=1;
        static const int tree2ID=2;
        static const int tree3ID=3;
        static const int tree4ID=4;
        static const int tree5ID=5;
        static const int tree6ID=6;
        static const int tree7ID=7;
        static const int tree8ID=8;
        static const int grass0ID=9;
        static const int grass1ID=10;
        static const int grass2ID=11;
        static const int grass3ID=12;
        static const int grass4ID=13;
        static const int grass5ID=14;
        static const int grass6ID =15;
        static const int abyss0ID =16;
        static const int abyss1ID =17;
        static const int abyss2ID =18;
        static const int abyss3ID =19;
        static const int abyss4ID =20;
        static const int abyss5ID =21;
        static const int abyss6ID =22;
        static const int abyss7ID =23;
        static const int abyss8ID =24;
        static const int bush0ID =25;
        static const int bush1ID =26;
        static const int bush2ID =27;
        static const int firebowl0ID =28;
        static const int fence0ID =29;
        static const int fence1ID =30;
        static const int fence2ID =31;
        static const int fence3ID =32;
        static const int fence4ID =33;
        static const int fence5ID =34;
        static const int fence6ID =35;
        static const int fence7ID =36;
        static const int fence8ID =37;
        static const int stonewall0ID =38; //not currently used
        static const int stonewall1ID=39; //not currently used
        static const int stonewall2ID=40; //not currently used
        static const int stonewall3ID=41; //not currently used
        static const int stonewall4ID=42; //not currently used
        static const int doorhinge0ID=43;
        static const int door0ID=44;
        static const int doorhinge1ID=45;
        static const int doorhinge2ID=46;
        static const int door1ID=47;
        static const int doorhinge3ID=48;
        //5 stonewalls, not currently used
        //8 grey walls, not currently used
        static const int floor0ID=62;
        static const int abyss9ID=63;
        static const int abyss10ID=64;
        static const int abyss11ID=65;
        static const int abyss12ID=66;
        static const int abyss13ID=67;
        static const int abyss14ID=68;
        static const int abyss15ID=69;
        static const int abyss16ID=70;
        static const int abyss17ID=71;
        //4 conveyor belts, not currently used
        //1 stone and 2 pressure plates, not currently used
        static const int firebowl1ID=79;
        //4 switch and color block tiles, not currently used
        static const int fence9ID=84;
        static const int fence10ID=85;
        static const int fence11ID=86;
        static const int fence12ID=87;
        static const int fence13ID=88;
        static const int fence14ID=89;
        static const int fence15ID=90;
        static const int fence16ID=91;
        static const int fence17ID=92;
        static const int fence18ID=93;
        static const int fence19ID=94;
        static const int fence20ID=95;
        static const int fence21ID=96;
        static const int fence22ID=97;
        static const int fence23ID=98;
        static const int fence24ID=99;
        static const int greywallblock0ID=100;
        static const int stonewall5ID=101;
        static const int stonewall6ID=102;
        static const int stonewall7ID=103;
        static const int stonewall8ID=104;
        static const int stonewall9ID=105;
        static const int stonewall10ID=106;
        static const int stonewall11ID=107;
        static const int stonewall12ID=108;
        static const int stonewall13ID=109;
        static const int stonewall14ID=110;
        static const int stonewall15ID=111;
        static const int stonewall16ID=112;
        static const int stonewall17ID=113;
        static const int stonewall18ID=114;
        static const int stonewall19ID=115;
        static const int stonewall20ID=116;
        static const int stonewall21ID=117;
        static const int stonewall22ID=118;
        static const int stonewall23ID=119;
        static const int stonewall24ID=120;
        static const int stonewall25ID=121;
        static const int stonewall26ID=122;
        static const int stonewall27ID=123;
        static const int stonewall28ID=124;
        static const int stonewall29ID=125;
        static const int stonewall30ID=126;
        static const int stonewall31ID=127;
        //2 unused factory tiles
        static const int door2ID=130;
        static const int door3ID=131;
        static const int door4ID=132;
        static const int door5ID=133;
        static const int door6ID=134;
        static const int door7ID=135;
        //3 unused factory tiles
        static const int table0ID=139;
        static const int table1ID=140;
        static const int table2ID=141;
        static const int table3ID=142;
        static const int table4ID=143;
        static const int table5ID=144;
        static const int table6ID=145;
        static const int rug0ID=146;//geht bis 170

        std::vector<int> fenceIDs = {fence0ID, fence1ID, fence2ID, fence3ID, fence4ID, fence5ID, fence6ID, fence7ID, fence8ID, fence9ID, fence10ID, fence11ID, fence12ID, fence13ID, fence14ID, fence15ID, fence16ID, fence17ID, fence18ID, fence19ID, fence20ID, fence21ID, fence22ID, fence23ID, fence24ID};
        //std::vector<int> pipeIDs = {pipe0ID, pipe1ID, pipe2ID, pipe3ID, pipe4ID, pipe5ID, pipe6ID, pipe7ID, pipe8ID, pipe9ID};
        std::vector<int> treeIDs = {tree0ID, tree1ID, tree2ID, tree3ID, tree4ID, tree5ID, tree6ID, tree7ID, tree8ID, bush0ID, bush1ID, bush2ID};
        std::vector<int> doorIDs = {door0ID, door1ID, door2ID, door3ID, door4ID, door5ID, door6ID, door7ID};
        std::vector<int> doorhingeIDs = {doorhinge0ID, doorhinge1ID, doorhinge2ID, doorhinge3ID};
        std::vector<int> stonewallIDs ={greywallblock0ID, stonewall0ID, stonewall1ID, stonewall2ID, stonewall3ID, stonewall4ID,stonewall5ID, stonewall6ID, stonewall7ID, stonewall8ID, stonewall9ID, stonewall10ID, stonewall11ID, stonewall12ID, stonewall13ID, stonewall14ID, stonewall15ID, stonewall16ID, stonewall17ID, stonewall18ID, stonewall19ID, stonewall20ID, stonewall21ID, stonewall22ID, stonewall23ID, stonewall24ID, stonewall25ID, stonewall26ID, stonewall27ID, stonewall28ID, stonewall29ID, stonewall30ID, stonewall31ID};
        std::vector<int> abyssIDs={abyss0ID, abyss1ID, abyss2ID, abyss3ID, abyss4ID, abyss5ID, abyss6ID, abyss7ID, abyss8ID, abyss9ID, abyss10ID, abyss11ID, abyss12ID, abyss13ID, abyss14ID, abyss15ID, abyss16ID, abyss17ID};
        std::vector<int> firebowlIDs={firebowl0ID, firebowl1ID};
        std::vector<int> tableIDs={table0ID, table1ID, table2ID, table3ID, table4ID, table5ID, table6ID};
        std::vector<int> grassIDs={grass0ID, grass1ID, grass2ID, grass3ID, grass4ID, grass5ID, grass6ID};
        std::vector<int> factoryfloorIDs={floor0ID};
        std::vector<int> bigdoor={doorhinge0ID, doorhinge1ID, door0ID};

        std::vector<std::vector<int>> wallIDs = { treeIDs, doorhingeIDs,stonewallIDs, tableIDs};
        std::vector<std::vector<int>> fencesIDs = {fenceIDs};
        std::vector<std::vector<int>> firebowlsIDs={firebowlIDs};
        std::vector<std::vector<int>> allabyssIDs={abyssIDs};
        std::vector<std::vector<int>> floorIDs={grassIDs, factoryfloorIDs};
        std::vector<std::vector<int>> factorydoor={bigdoor};

        ~gameplay() {
            //std::cout << "Gameplay instance destroyed\n";
            for (auto& roomStones : stonesInRooms) {
                for (auto stone : roomStones.second) {
                    delete stone;
                }
            }
        }
    protected:
        std::vector<int>enemyID;

        void updateAllenemies();
    };

#endif //RAYLIBSTARTER_GAMEPLAY_H
