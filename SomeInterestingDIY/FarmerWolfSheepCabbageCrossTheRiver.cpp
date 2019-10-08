#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>

/*
使用不同的二进制位来表示当前河岸状态，可以组合为二进制掩码，
使用低四位来表示，从低到高分别表示狼，羊，农民，菜。
*/
enum Object {
    WOLF = 0x00000001,   /* 狼	0001*/
    SHEEP = 0x00000002,  /* 羊	0010*/
    FARMER = 0x00000004, /* 农民	0100*/
    CABBAGE = 0x00000008 /* 菜	1000*/
};

/*
农民撑船的运动方向
*/
enum Direction {
    TO_LEFT,  /* 从右岸到左岸 */
    TO_RIGHT, /* 从左岸到右岸 */
};

/*
记录农民一次撑船活动
*/
struct ResultItem {
    int dir;       /* 方向 */
    int obj;       /* 船上带的东西 */
    int StateMaks; /* 标识左右两岸现在分别存在哪些对象 */
};

class RiverBank {
private:
    int mask;

public:
    RiverBank();
    inline bool Empty();
    inline bool Find(Object obj);
    inline void Insert(Object obj);
    inline void Remove(Object obj);
    inline int GetMask() const;
};

RiverBank::RiverBank() { mask = 0; }

inline bool RiverBank::Empty() { return mask == 0; }

inline bool RiverBank::Find(Object obj) { return (mask & obj) != 0; }

inline void RiverBank::Insert(Object obj) { mask |= obj; }

inline void RiverBank::Remove(Object obj) { mask &= (~obj); }

inline int RiverBank::GetMask() const { return mask; }

const Object ObjectArray[] = {WOLF, SHEEP, FARMER, CABBAGE};

std::vector<ResultItem> ResultSet;                    /* 储存从最初状态到现在的往返信息 */
std::set<int> State;                                  /* 储存已经搜索过的状态 */
std::map<Object, Object> FoodChain, ReverseFoodChain; /* 记录食物链 */

void Initialize(RiverBank &Left, RiverBank &Right);             /* 初始化搜索状态 */
void PrintResultSet();                                          /* 输出搜索结果 */
std::string ObjectToString(int obj);                            /* 将枚举对象转化成可以输出的字符串 */
bool CheckFinalStatus(RiverBank &LeftNow, RiverBank &RightNow); /* 检查所以东西是否已经运抵右岸 */
bool CheckForSafety(RiverBank &side); /* 检查当前左右两岸是否安全（农民不在时不能出现食物链） */
void DFS(Direction dir, RiverBank &LeftNow, RiverBank &RightNow);

int main() {
    RiverBank Left, Right;
    Initialize(Left, Right);
    State.insert(Left.GetMask()); /* 存储初始状态 */
    DFS(TO_RIGHT, Left, Right);   /* 先从左岸到右岸 */
    return 0;
}

inline void Initialize(RiverBank &Left, RiverBank &Right) {
    /* 初始化食物链 */
    FoodChain[WOLF] = SHEEP;
    FoodChain[SHEEP] = CABBAGE;

    ReverseFoodChain[SHEEP] = WOLF;
    ReverseFoodChain[CABBAGE] = SHEEP;

    /* 初始化左岸状态 */
    Left.Insert(WOLF);
    Left.Insert(SHEEP);
    Left.Insert(FARMER);
    Left.Insert(CABBAGE);
}

void PrintResultSet() {
    if (ResultSet.empty()) {
        printf("No way!\n");
        return;
    }
    static int ResuluCount = 0;
    std::vector<ResultItem>::iterator iter;
    printf("=================Result %d==================\n", ++ResuluCount);
    for (iter = ResultSet.begin(); iter != ResultSet.end(); iter++) {

        if (iter->dir == TO_LEFT)
            printf("+ From right to left, ");
        else
            printf("+ From left to right, ");

        printf("farmer bring %-7s+   ", ObjectToString(iter->obj).c_str());
        std::string LeftString = "Left: { " + (((iter->StateMaks) & WOLF) ? ObjectToString(WOLF) + " " : "") +
                                 (((iter->StateMaks) & SHEEP) ? ObjectToString(SHEEP) + " " : "") +
                                 (((iter->StateMaks) & FARMER) ? ObjectToString(FARMER) + " " : "") +
                                 (((iter->StateMaks) & CABBAGE) ? ObjectToString(CABBAGE) : "") + " }";

        std::string RightString = "Right: { " + (!((iter->StateMaks) & WOLF) ? ObjectToString(WOLF) + " " : "") +
                                  (!((iter->StateMaks) & SHEEP) ? ObjectToString(SHEEP) + " " : "") +
                                  (!((iter->StateMaks) & FARMER) ? ObjectToString(FARMER) + " " : "") +
                                  (!((iter->StateMaks) & CABBAGE) ? ObjectToString(CABBAGE) : "") + " }";
        printf("%-*s%-*s\n", 30, LeftString.c_str(), 30, RightString.c_str());
    }
    printf("===========================================\n");
}

inline std::string ObjectToString(int obj) {
    std::string StringObject = "null";
    switch (obj) {
        case WOLF:
            StringObject = "wolf";
            break;
        case SHEEP:
            StringObject = "sheep";
            break;
        case FARMER:
            StringObject = "farmer";
            break;
        case CABBAGE:
            StringObject = "cabbage";
            break;
    }
    return StringObject;
}

inline bool CheckFinalStatus(RiverBank &LeftNow, RiverBank &RightNow) {
    /* 所以东西都已经运抵右岸，包括农民自己 */
    if (LeftNow.Empty() && RightNow.Find(WOLF) && RightNow.Find(SHEEP) && RightNow.Find(FARMER) &&
        RightNow.Find(CABBAGE))
        return true;
    return false;
}

inline bool CheckForSafety(RiverBank &side) {
    /* 农民不在时，羊和狼不能在同一岸，羊和菜不能再同一岸 */

    for (int i = 0; i < 4; i++) {

        Object obj = ObjectArray[i];

        if (side.Find(obj) == false)
            continue;

        if ((FoodChain.count(obj) != 0 && side.Find(FoodChain[obj]) && side.Find(FARMER) == false) ||

            (ReverseFoodChain.count(obj) != 0 && side.Find(ReverseFoodChain[obj]) && side.Find(FARMER) == false))
            return false;
    }
    return true;
}

void DFS(Direction dir, RiverBank &LeftNow, RiverBank &RightNow) {
    /* 检查是否已经找到解法 */
    if (CheckFinalStatus(LeftNow, RightNow)) {
        PrintResultSet();
        return;
    }

    RiverBank TempLeft, TempRight;

    if (dir == TO_RIGHT) { /* 当前是从左岸去右岸 */

        /* 尝试带左岸存在的对象 */
        for (int i = 0; i < 4; i++) {

            Object obj = ObjectArray[i];
            if (LeftNow.Find(obj) == false)
                continue;

            /* 预测把obj带到对岸的状态 */
            /* Left----> Farmer & obj ----->Right */
            TempLeft = LeftNow;
            TempLeft.Remove(obj);
            TempLeft.Remove(FARMER);
            TempRight = RightNow;
            TempRight.Insert(obj);
            TempRight.Insert(FARMER);

            /* 检查预测的状态是否可行 */
            if (CheckForSafety(TempLeft) && CheckForSafety(TempRight)) {

                /* 检查预测的状态之前是否已经出现过，如果出现则恢复成当前状态 */
                if (State.find(TempLeft.GetMask()) != State.end()) {
                    TempLeft = LeftNow;
                    TempRight = RightNow;
                    continue;
                }

                ResultSet.push_back(ResultItem{TO_RIGHT, obj, TempLeft.GetMask()}); /* 记录当前操作 */
                State.insert(TempLeft.GetMask());                                   /* 记录状态 */
                DFS(TO_LEFT, TempLeft, TempRight);                                  /* 基于当前状态搜索 */
                State.erase(TempLeft.GetMask()); /* 当前操作已经搜索过，清除状态，消除影响 */
                ResultSet.pop_back(); /* 当前操作已经搜索过，从结果集合中删掉，消除影响 */

            } else
                continue;
        }
    } else {

        for (int i = 0; i < 4; i++) {

            Object obj = ObjectArray[i];
            if (RightNow.Find(obj) == false)
                continue;

            TempRight = RightNow;
            TempRight.Remove(obj);
            TempRight.Remove(FARMER);
            TempLeft = LeftNow;
            TempLeft.Insert(obj);
            TempLeft.Insert(FARMER);

            if (CheckForSafety(TempLeft) && CheckForSafety(TempRight)) {

                if (State.find(TempLeft.GetMask()) != State.end()) {
                    TempLeft = LeftNow;
                    TempRight = RightNow;
                    continue;
                }

                ResultSet.push_back(ResultItem{TO_LEFT, obj, TempLeft.GetMask()});
                State.insert(TempLeft.GetMask());
                DFS(TO_RIGHT, TempLeft, TempRight);
                State.erase(TempLeft.GetMask());
                ResultSet.pop_back();

            } else
                continue;
        }
    }
}
