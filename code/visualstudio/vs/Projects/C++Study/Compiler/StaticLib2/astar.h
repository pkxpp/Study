#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
//#include "KVector2.h"
//#include "CoreMath.h"
#include "KVector2.h"

class BlockAllocator;

class AStar
{
public:
    typedef std::function<bool(const KVec2&)> Callback;

    /**
     * 搜索参数
     */
    struct Params
    {
        bool        corner;     // 允许拐角
        uint16_t    height;     // 地图高度
        uint16_t    width;      // 地图宽度
        KVec2       start;      // 起点坐标
        KVec2       end;        // 终点坐标
        Callback    can_pass;   // 是否可通过

        Params() : height(0), width(0), corner(false)
        {
        }
    };

    struct STest
    {
        int a;
        int b;
    };
    struct SSubTest : public STest
    {

    };

public:
    /**
     * 路径节点状态
     */
    enum NodeState
    {
        NOTEXIST = 0,               // 不存在
        IN_OPENLIST,            // 在开启列表
        IN_CLOSEDLIST           // 在关闭列表
    };

    /**
     * 路径节点
     */
    struct Node
    {
        uint16_t    g;          // 与起点距离
        uint16_t    h;          // 与终点距离
        KVec2 pos;        // 节点位置
        NodeState   state;      // 节点状态
        Node*       parent;     // 父节点

        /**
         * 计算f值
         */
        int f() const
        {
            return g + h;
        }

        inline Node(const KVec2 &pos)
            : g(0), h(0), pos(pos), parent(nullptr), state(NOTEXIST)
        {
        }
    };

public:
    AStar(BlockAllocator *allocator);

    ~AStar();

public:
    /**
     * 获取直行估值
     */
    int get_step_value() const;

    /**
     * 获取拐角估值
     */
    int get_oblique_value() const;

    /**
     * 设置直行估值
     */
    void set_step_value(int value);

    /**
     * 获取拐角估值
     */
    void set_oblique_value(int value);

    /**
     * 执行寻路操作
     */
    std::vector<KVec2> find(const Params& param);
    void find(Params& param, std::vector<KVec2> vecPath);
    //void test(STest& a);
    //void test4(SSubTest& a);
    //void test1(Params& a);
    //void test2(std::vector<int> vecTest);
    //void test3(std::vector<KVECTOR2> vecTest);
    //void test5(KVec2& v2Test);
    //void test6(std::vector<SSubTest> vecTest);
    ////void test7(KVec2Test& vec2Test);
    ////void test8(std::vector<KVec2Test> vecTest);
    //void test9(std::vector<KVec2> vecTest);
private:
    /**
     * 清理参数
     */
    void clear();

    /**
     * 初始化参数
     */
    void init(const Params &param);

    /**
     * 参数是否有效
     */
    bool is_vlid_params(const Params &param);

private:
    /**
     * 二叉堆上滤
     */
    void percolate_up(size_t hole);

    /**
     * 获取节点索引
     */
    bool get_node_index(Node *node, size_t *index);

    /**
     * 计算G值
     */
    uint16_t calcul_g_value(Node *parent, const KVec2&current);

    /**
     * 计算F值
     */
    uint16_t calcul_h_value(const KVec2&current, const KVec2&end);

    /**
     * 节点是否存在于开启列表
     */
    bool in_open_list(const KVec2&pos, Node *&out_node);

    /**
     * 节点是否存在于关闭列表
     */
    bool in_closed_list(const KVec2&pos);

    /**
     * 是否可通过
     */
    bool can_pass(const KVec2 &pos);

    /**
     * 当前点是否可到达目标点
     */
    bool can_pass(const KVec2 &current, const KVec2 &destination, bool allow_corner);

    /**
     * 查找附近可通过的节点
     */
    void find_can_pass_nodes(const KVec2 &current, bool allow_corner, std::vector<KVec2> *out_lists);

    /**
     * 处理找到节点的情况
     */
    void handle_found_node(Node *current, Node *destination);

    /**
     * 处理未找到节点的情况
     */
    void handle_not_found_node(Node *current, Node *destination, const KVec2 &end);

private:
    int                     step_val_;
    int                     oblique_val_;
    std::vector<Node*>      mapping_;
    uint16_t                height_;
    uint16_t                width_;
    Callback                can_pass_;
    std::vector<Node*>      open_list_;
    BlockAllocator*         allocator_;
};

#endif
