/*Program Name: finalProjectRPG
 *Name: Henry Lay
 *Date: 04/15/21
 * a simultaneous-turn RPG
 */
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include "mob.cpp"

using namespace std;

//output text and write to file
void logNPrint(string text){
    //creates/opens log txt file
    ofstream log("log.txt",fstream::app);
    //write to log
    log<<text<<endl;
    //output
    cout<<endl<<text<<endl;
    log.close();
}
//generate random number from x->y(INCLUSIVE)
int rng(int x, int y){
    return rand()%(y-x+1)+x;
}
//generate random name (adjective noun)
string rndName(){
    string name;
    string adjective[5] = {"Horrid","Angry","Unfriendly","Menacing","Giant"};
    string noun[5] = {"Grunt","Zombie","Thief","Troll","Giant"};

    name+= adjective[rng(0,4)];
    name+= " ";
    name+= noun[rng(0,4)];

    return name;
}

//main method (3 parts: startup, adventure phase, shop phase)
int main(){

//startup
    //needed for RNG
    srand(time(0));

    //creates/prepares log.txt file
    ofstream log("log.txt",fstream::app);

    //getting player's name
    string name;
    cout << "Greetings adventurer!\nWhat was your name again??\n\n";
    getline(cin,name);

    //base stats randomized (name,hp,atk,def,gold,attribute points)
    int pHP = rng(40,50);
    int pATK = rng(4,6);
    int pDEF = rng(2,4);

    //creating player; base gold 200 and ATR points 5
    mob player(name,pHP,pATK,pDEF,200,5);

    //output/log player once
    logNPrint(player.toString());

    //formating log for new adventure
    log<<"~~~~~~~~~~~~~~~~~~\nThis is the beginning of "<<player.getName()<<"!\n";
    log.close();

    //progression tracker and difficulty (rewards+enemy scales)
    int stage = 1;

    //defualt shop chance, shop chance 1/shopChance
    int dfShopChance = 4;
    int shopChance = dfShopChance;

    //ACTUAL GAME
    int advChoice;
    while(player.getHP()>0){ //play while alive

        //enemy generation
        int eHP = rng(2,stage+9)+2*stage;
        int eATK = rng(0,stage+3)+stage;
        int eDEF = rng(stage-1,stage+2);
        mob enemy(rndName(),eHP,eATK,eDEF);

        //enemy encounter
        logNPrint(player.getName()+" encounters "+enemy.getName()+"!");

        int advChoice;
        do{ //adventure phase

            cout<<enemy.toString()<<"...\nWhat will you do?: 1)battle\n\t\t   2)flee\n\t\t   3)spend ATR pts\n";
            cout<<"Enter int(1-3): ";
            cin>>advChoice;


            if(advChoice==1){ //battle

                //calculating and losing HP
                int dmgTaken = (mob::battle(player, enemy));
                player.changeHP(-dmgTaken);
                //check if player died
                if(player.getHP()==0){
                    //game over text
                    logNPrint(player.getName()+" DIED TO "+enemy.getName()+" ON STAGE "+to_string(stage));
                    exit(0);
                }
                //gain gold (RNG) reward for winning battle
                int reward = 10+rng(8+stage,13+stage)*stage;
                player.changeGLD(reward);
                logNPrint(player.getName()+" defeated the "+enemy.getName()+", taking "+to_string(dmgTaken)+" damage and gaining "+to_string(reward)+"g");
                //winning every 3 battles gains ATR point(s)
                if(stage%3==0){
                    //gain more atr further in
                    int atrToGain = stage/5 + 1;
                    player.gainATR(atrToGain);
                    logNPrint(player.getName()+" Recieved " + to_string(atrToGain)+" attribute point(s)");
                }
                //winning battle progresses difficulty
                stage++;

            }else if (advChoice==2){ //fleeing

                //fleeing drops gold (RNG)
                int gDrop = rng(0,10)+stage*10;
                if (player.getGLD()<gDrop){
                    gDrop = player.getGLD();
                }
                player.changeGLD(-gDrop);

                //fleeing heals 1/4 hp (rounded down)
                int heal;
                //calculating how much to heal and not to heal excess (excess is rounded to maxHP anyways; this is mostly for output/log)
                if (player.getHP()>player.getMaxHP()/4*3){
                    heal = player.getMaxHP()-player.getHP();
                }else{
                    heal = player.getMaxHP()/4;
                }
                //actually healing the player
                player.changeHP(+heal);
                logNPrint(player.getName()+" dropped "+to_string(gDrop)+"g while fleeing and recovered "+to_string(heal)+"HP");

            }else if(advChoice==3){ //spend ATR pts

                cout<<endl;
                //no ATR to spend message
                if(player.getATR()==0){
                    cout<<"No remaining ATR points.. "<<player.toString()<<endl;

                //spending ATR
                }else{
                    cout<<player.toString()<<endl;
                    //stay in menu as long as there is ATR
                    while(player.getATR()>0){
                        cout<<"Spend remaining "<<player.getATR()<<" ATR point(s): 1) health+"<<player.getAtrHpUp()<<"\n\t\t\t\t2) attack+"<<player.getAtrAtkUp()<<"\n\t\t\t\t3) defense+"<<player.getAtrDefUp()<<endl;
                        cout<<"Enter int(1-3): ";

                        //user input for what stat to spend ATR
                        int atrChoice;
                        cin>>atrChoice;
                        //only spend ATR if stat exists
                        if((atrChoice>=1)&&(atrChoice<=3)){
                            player.spendATR(atrChoice);
                            logNPrint("ATR point spent... "+player.toString());
                        }
                    }
                    cout<<endl;
                }
            }

        }while((advChoice!=1)&&(advChoice!=2)); //only move to shop phase after a battle (success) or fleeing; NOT spending ATR

        logNPrint(player.toString());

        if(rng(1,shopChance)==1){ //Shop phase(RNG)
            logNPrint("Shop encountered");
            //reset shop odds
            shopChance = dfShopChance;

            //create shop
            //vector<item> shop is arraylist-esque (no limited space) that contains items
            vector<item> shop;
            for(int i = 0; i<stage/3 + 4; i++){
                //generate random item (out of 7 main item types) based on price;
                shop.push_back(item::createItem(rng(75,200),rng(1,7)));
                //for every item in shop has chance (1/111) to generate a BOTHK
                if(rng(1,111)==1){
                    shop.push_back(item::createItem(rng(201,333),9));
                //otherwise '' (1/6) to generate a scrap
                }else if(rng(1,8)==1){
                    shop.push_back(item::createItem(rng(0,450),8));
                }
            }


            int shopChoice;
            do{ //in shop

                //displaying shop
                cout<<"Shop Contents...\n";
                for(int i = 0; i<shop.size(); i++){
                    string shopLine = to_string(i+1)+". "+shop[i].getName();
                    cout<<shopLine;
                    //list# + item name occupies 20 spaces...
                    if (shopLine.length()<=20){
                        for (int k = 0; k<20-shopLine.length(); k++){
                            cout<<" ";
                        }
                    //extra space if list# + item name occupies > 20 spaces...
                    }else{
                        cout<<" ";
                    }
                    //..followed by item info
                    cout<<shop[i].infoToString()<<endl;
                }

                //player input what to do in shop
                cout<<"Shop Options: 0)Leave\n\t      1-"<<shop.size()<<")purchase item \"x\" in shop\n\t      "<<shop.size()+1<<")see sorts\n";
                cout<<"Enter int(0-"<<shop.size()+1<<"): ";
                cin>>shopChoice;

                //leave shop if exit code
                if(shopChoice==0){
                    break;

                //purchasing item from shop
                }else if((shopChoice>0)&&(shopChoice<=shop.size())){
                    //if player can afford
                    if(player.getGLD()>=shop[shopChoice-1].getPrice()){
                        player.equip(shop[shopChoice-1]);
                        player.changeGLD(-shop[shopChoice-1].getPrice());
                        logNPrint(player.getName()+" bought "+shop[shopChoice-1].toString()+"... now "+player.toString());
                        shop.erase(shop.begin()+shopChoice-1);
                        cout<<endl;

                    //mid-shop end of adventure (wrath elixir kill)
                    if (player.getHP()==0){
                        //game over text
                        logNPrint(player.getName()+" DIED TO Wrath Elixir ON STAGE "+to_string(stage));
                        exit(0);

                    }

                    //' cant afford
                    }else{
                        cout<<"Not enough gold!\n\n";
                    }

                //sorting the shop
                }else if((shopChoice>=shop.size())&&(shopChoice<shop.size()+3)){
                    int sortChoice;
                    do{
                        //player input sort type
                        cout<<"\nSort types: 0)exit\n\t    1)max Price\n\t    2)max HP\n\t    3)max ATK\n\t    4)max DEF\n";
                        cout<<"Enter int (0-4): ";
                        cin>>sortChoice;
                        //leave sorting menu
                        if(sortChoice==0){
                            break;
                        //sorting options *
                        }else if(sortChoice==1){
                            item::sortMaxPrice(shop);
                        }else if(sortChoice==2){
                            item::sortMaxHP(shop);
                        }else if(sortChoice==3){
                            item::sortMaxATK(shop);
                        }else if(sortChoice==4){
                            item::sortMaxDEF(shop);
                        }
                        cout<<endl;
                    }while(sortChoice<0||sortChoice>4);
                    cout<<endl<<player.toString()<<endl;
                }
                //output text for empty shop
                if (shop.size()==0){
                    cout<<"Shop sold out!\n";
                }
            }while(shop.size()>0); //leave shop when empty

            //left the shop message
            logNPrint("Left Shop");
            cout<<endl<<player.toString()<<endl;

        //miss shop; more chance to hit shop next time
        }else{
            shopChance--;
        }
    }
    return 0;
}
