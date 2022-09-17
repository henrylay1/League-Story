/*Program Name: mob
 *Name: Henry Lay
 *Date: 04/15/21
 *mob class
 */
#include "item.cpp"

using namespace std;

//mob class
class mob{
    private:
        //mob stats
        string name;
        int HP;
        int maxHP;
        int ATK;
        int DEF;
        int GLD;
        int ATR;
        //conversion of 1 ATR to x stat
        const int atrHpUp= 8;
        const int atrAtkUp= 1;
        const int atrDefUp= 1;
    public:
        //constructors
        //for creating a playable character type
        mob(string n, int hp, int atk, int def, int gld, int atr){
            name = n;
            HP = hp;
            maxHP = hp;
            ATK = atk;
            DEF = def;
            GLD = gld;
            ATR = atr;

        }
        //for creating an enemy (no need for GLD or ATR);
        mob(string n, int hp, int atk, int def){
            name = n;
            HP = hp;
            maxHP = hp;
            ATK = atk;
            DEF = def;
            GLD = 0;
            ATR = 0;
        }

        //accessors
        string getName(){
            return name;
        }
        int getHP(){
            return HP;
        }
        int getMaxHP(){
            return maxHP;
        }
        int getATK(){
            return ATK;
        }
        int getDEF(){
            return DEF;
        }
        int getGLD(){
            return GLD;
        }
        int getATR(){
            return ATR;
        }
        int getAtrHpUp(){
            return atrHpUp;
        }
        int getAtrAtkUp(){
            return atrAtkUp;
        }
        int getAtrDefUp(){
            return atrDefUp;
        }

        //modifiers
        void setName(string n){
            name = n;
        }
        void setHP(int n){
            HP = n;
        }
         void setMaxHP(int n){
            HP = n;
        }
        void setATK(int n){
            ATK = n;
        }
        void setDEF(int n){
            DEF = n;
        }
        void setGLD(int n){
            GLD = n;
        }
        void setATR(int n){
            ATR = n;
        }

        //semi modifiers
        //modify hp, never above maxHP (overheal) or below 0 (overkill)
        void changeHP(int x){
            HP = HP + x;
            if(HP>maxHP){
                HP = maxHP;
            }
            if(HP<0){
                HP=0;
            }
        }

        //modify gold; never below 0 (no debt)
        void changeGLD(int g){
            GLD+=g;
            if(GLD<0){
                GLD = 0;
            }
        }
        //increase # attribute points
        void gainATR(int a){
            ATR+= a;
        }

        //assign ATR point to HP, ATK, or DEF
        void spendATR(int statType){
            if(statType == 1){
                HP += atrHpUp;
                maxHP += atrHpUp;
                ATR--;
            }else if(statType == 2){
                ATK+=atrAtkUp;
                ATR--;
            }else if(statType == 3){
                DEF+=atrDefUp;
                ATR--;
                //wrath potion -def; therefore
            }
        }

        //equiping item stats (item class from "item.cpp")
        void equip(item equipable){
            //using changeHP method to ensure equiping wrath elixir doesn't put hp below 0
            changeHP(equipable.getStatHP());
            maxHP+=equipable.getStatHP();
            ATK+=equipable.getStatATK();
            DEF+=equipable.getStatDEF();

        }

        //battle method; returns damage done to "ally" in battle between "ally" & "foe"
        static int battle(mob ally, mob foe){
            //complete once either mob's HP = 0
            if((ally.getHP()==0) || (foe.getHP()==0)){
                return 0;
            }else{
                //simultaneous strikes...
                ally.changeHP(-ally.damage(foe.getATK()));
                foe.changeHP(-foe.damage(ally.getATK()));
                //then strike again (recursion)
                return(battle(ally,foe) + ally.damage(foe.getATK()));
            }
        }

        //turns mob to readable string (could have used "friend ostream& operator<<(~~~)"?)
        string toString(){
            string text = name +" has "+ to_string(HP)+"/"+to_string(maxHP)+" HP, "+to_string(ATK)+" ATK, "+to_string(DEF)+" DEF";
            if(GLD>0){
                text+=" ("+to_string(GLD)+"g)";
            }
            return text;
        }

    //private helper method only used within class
    private:
        //dealing/recieving damgage;
        int damage(int incoming){
            //damage calculation for negative DEF
            if(DEF<0){
                return incoming+(-DEF/2);
            //always take at least 1 damage
            }else if(incoming-(DEF/2)<1){
                return 1;
            //damage>1 calculation with DEF mitigation
            }else{
                return incoming-(DEF/2);
            }

        }
};
