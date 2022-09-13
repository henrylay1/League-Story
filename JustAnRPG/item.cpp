/*Program Name: item
 *Name: Henry Lay
 *Date: 04/15/21
 *item class
 */
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

class item{
    private:
        //item stats
        string name;
        int statHP;
        int statATK;
        int statDEF;
        int price;
    public:
        //constructor
        item(string n, int hp, int atk, int def, int gld){
            //name of item has roman numerals attached(only I's)
            n+=" ";
            for(int i = 0; i<=(gld-75)/50;i++){
                n+= "I";
            }

            name = n;
            statHP = hp;
            statATK = atk;
            statDEF = def;
            price = gld;
        }

        //accessors
        string getName(){
            return name;
        }
        int getStatHP(){
            return statHP;
        }
        int getStatATK(){
            return statATK;
        }
        int getStatDEF(){
            return statDEF;
        }
        int getPrice(){
            return price;
        }
        //modifiers
        void setName(string n){
            name = n;
        }
        void setStatHP(int n){
            statHP = n;
        }
        void setStatATK(int n){
            statATK = n;
        }
        void setStatDEF(int n){
            statDEF = n;
        }
        void setPrice(int n){
            price = n;
        }

        //sorts (& = reference, alias to vector outside local; changes made in method affect in main; no return needed)
        //sort by price greatest to least
        static void sortMaxPrice(vector<item>& items){
            for(int i = 0; i<items.size()-1; i++){
                //sort until sorted subarray
                for(int j = 0; j<items.size()-i-1; j++){
                    if(items.at(j).getPrice()<items.at(j+1).getPrice()){
                        swap(items.at(j),items.at(j+1));
                    }
                }
            }
        }

        //sort items by HP stat greatest to least
        static void sortMaxHP(vector<item>& items){
            for(int i = 0; i<items.size()-1; i++){
                //sort until sorted subarray
                for(int j = 0; j<items.size()-i-1; j++){
                    if(items.at(j).getStatHP()<items.at(j+1).getStatHP()){
                        swap(items.at(j),items.at(j+1));
                    }
                }
            }
        }

        //sort items by ATK stat greatest to least
        static void sortMaxATK(vector<item>& items){
            for(int i = 0; i<items.size()-1; i++){
                //sort until sorted subarray
                for(int j = 0; j<items.size()-i-1; j++){
                    if(items.at(j).getStatATK()<items.at(j+1).getStatATK()){
                        swap(items.at(j),items.at(j+1));
                    }
                }
            }
        }

        //sort items by DEF stat greatest to least
        static void sortMaxDEF(vector<item>& items){
            for(int i = 0; i<items.size()-1; i++){
                //sort until sorted subarray
                for(int j = 0; j<items.size()-i-1; j++){
                    if(items[j].getStatDEF()<items[j+1].getStatDEF()){
                        swap(items.at(j),items.at(j+1));
                    }
                }
            }
        }

        //generates a random item from available items (most items depend on price)
        static item createItem(int itemPrice, int itemType){
            vector<item> availItems{
            //main items (1-7)
            {"Giants Belt",itemPrice/30*5,0,0,itemPrice},
            {"Bramble Vest",itemPrice/50*5,0,itemPrice/75,itemPrice},
            {"BF Sword",0,itemPrice/30,0,itemPrice},
            {"Phage",itemPrice/75*5,itemPrice/50,0,itemPrice},
            {"Cloth Armor",0,0,itemPrice/30,itemPrice},
            {"Wrath Elixir",-itemPrice/35*5,itemPrice/15,-itemPrice/65,itemPrice},
            {"TriForce",itemPrice/66*5,itemPrice/66,itemPrice/66,itemPrice},
            //unique items (8-9)
            {"Scrap",itemPrice/75,1,1,10}, //scraps has fixed price, ATK, DEF, but HP scales with RNG
            {"Blade of the Holy King",250,50,50,itemPrice},
            };

            return availItems.at(itemType-1);
        }

        //turns item to readable string
        string toString(){
            return(name+" (PRICE:"+to_string(price)+"g HP:"+to_string(statHP)+" ATK:"+to_string(statATK)+" DEF:"+to_string(statDEF)+")");
        }

        //turns item's stats to readable string
        string infoToString(){
            return(" (PRICE:"+to_string(price)+"g HP:"+to_string(statHP)+" ATK:"+to_string(statATK)+" DEF:"+to_string(statDEF)+")");
        }
};
