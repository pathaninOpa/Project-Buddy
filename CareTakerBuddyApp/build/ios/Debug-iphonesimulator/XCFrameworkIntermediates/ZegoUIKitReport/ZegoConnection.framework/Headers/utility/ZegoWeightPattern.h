#ifndef ZegoWeightPattern_h
#define ZegoWeightPattern_h
#include <algorithm>
#include <string>
#include <vector>
namespace ZEGO {
namespace BASE {
/* 集合权重类实现 
功能如下：
       指定N个包含不重复数据的集合(集合间数据也不重复)，指定每个集合的权重。 生成按权重集合的数据列表
注意：
     1、模板类型 T 可以是基础类型，或者类类型。类类型 必须重载实现T == 操作符。 后续的增，变，查需要使用

使用如下:
       指定N个数据不重复集合例如：set_a={data1, data2}, set_b={data3,data4}, set_c={data5,data6}; 
       指定N个集合各个集合的权重； set_a.weight = 3, set_b.weight = 2, set_c.weight = 1
       则可以通过 一直调用 GetNextWeightData 可按集合的权重比，拿到data 数据. 
       获取数据后若是不调用MarkPatternData ， 则再次调用GetNextWeightData 拿到的还是上次的数据
*/

//数据描述desc
struct PatternDataDesc {
    std::string tag_desc;
};

template <typename T_Key, typename T_Data> struct PatternData {
    T_Key key;        //集合数据key 用来判断数据是否相等
    T_Data user_data; //用户数据
    bool used = false;
    int used_count = 0; //user data 被选中得次数
};

template <typename T_Key, typename T_Data> struct PatternDataSet {
    std::vector<PatternData<T_Key, T_Data>> pattern_data_list;
    std::string set_name; //集合名称,
    int cur_weight = 0;   //当前集合每次被选中一次，且被Mark 后 cur_weight 就减一
    int src_weight = 0;   //集合原始权重值
    int set_select_count = 0; //当前集合被选中的次数
};

#define WeightContextInvaild -1

enum SelectWeightSetMode {
    Mode0 = 0, //当集合权重相同时，优先选择最久没有被选中过得集合
    Mode1 = 1, //当集合权重相同时，优先选择 原始权重高的集合
    Mode2 = 2, //优先选择没有被选中的集合
};

typedef int WeightContext;

template <typename T_Key, typename T_Data> class CWeightPattern {
  public:
    CWeightPattern() {}
    ~CWeightPattern() {}

    void CreatePattern(std::vector<PatternDataSet<T_Key, T_Data>> &&pattern_data_set_list,
                       SelectWeightSetMode select_mode = SelectWeightSetMode::Mode0) {
        if (pattern_data_set_list.empty()) {
            assert(false);
            return;
        }

        DestroyPattern();

        m_next_pattern_set_index = 0;
        m_select_weight_set_mode = select_mode;
        m_pattern_data_set_list = std::move(pattern_data_set_list);
    }

    void DestroyPattern() {
        m_pattern_data_set_list.clear();
        m_next_pattern_set_index = 0;
        m_select_weight_set_mode = SelectWeightSetMode::Mode0;
    }

    const std::vector<PatternDataSet<T_Key, T_Data>> &GetPattern() {
        return m_pattern_data_set_list;
    }

    //get next weight data, but not mark used, if you used used must call MarkPatternData. and next will get right data
    WeightContext GetNextWeightData(T_Key &key, T_Data &pattern_data, std::string &out_set_name) {
        int pattern_set_list_size = (int)m_pattern_data_set_list.size();
        if (pattern_set_list_size == 0) {
            return WeightContextInvaild;
        }

        //check next pattern_set index is invalid ?
        if (m_next_pattern_set_index < 0 || m_next_pattern_set_index >= pattern_set_list_size) {
            m_next_pattern_set_index = 0;
        }

        //find next have weight
        int have_weight_index = FindSetHaveWeight(m_next_pattern_set_index);
        if (-1 == have_weight_index) {
            m_next_pattern_set_index = 0; //not find index reset
            ResetAllSetWeight();
            have_weight_index = FindSetHaveWeight(m_next_pattern_set_index);
        }

        if (-1 == have_weight_index) {
            assert(false);
            return WeightContextInvaild; //impossible
        }

        auto &pattern_data_set = m_pattern_data_set_list[have_weight_index];
        if (!HaveUnusedDataInSet(pattern_data_set)) {
            ResetSetPatternDataToUnused(pattern_data_set);
        }

        FindUnusedDataInSet(pattern_data_set, key, pattern_data, out_set_name);

        return have_weight_index;
    }

    void MarkPatternData(WeightContext context, const T_Key &target_pattern_data) {
        int have_weight_index = context;
        int pattern_set_list_size = (int)m_pattern_data_set_list.size();

        if (have_weight_index < 0 || have_weight_index >= pattern_set_list_size) {
            assert(false);
            return;
        }

        auto &pattern_data_set = m_pattern_data_set_list[have_weight_index];
        MarkDataUsedInSet(pattern_data_set, target_pattern_data);
        pattern_data_set.set_select_count++;

        pattern_data_set.cur_weight--;
        if (pattern_data_set.cur_weight < 0)
            pattern_data_set.cur_weight = 0;

        MakeNextSetPatternIndex(have_weight_index);
    }

    bool FindPatternData(const T_Key &src_pattern_data_key, T_Data &out_pattern_data,
                         std::string &out_set_name) {
        for (const auto &pattern_set : m_pattern_data_set_list) {
            for (const auto &pattern_data : pattern_set.pattern_data_list) {
                if (pattern_data.key == src_pattern_data_key) {
                    out_pattern_data = pattern_data.user_data;
                    out_set_name = pattern_set.set_name;
                    return true;
                }
            }
        }
        return false;
    }

    bool UpdatePatternData(const T_Key &src_pattern_data_key, const T_Data &src_pattern_data) {
        for (auto &pattern_set : m_pattern_data_set_list) {
            for (auto &pattern_data : pattern_set.pattern_data_list) {
                if (pattern_data.key == src_pattern_data_key) {
                    pattern_data.user_data = src_pattern_data;
                    return true;
                }
            }
        }
        return false;
    }

    //move data to set back
    bool MoveDataToPatternDataSetBack(const T_Key &src_pattern_data_key) {
        for (int i = 0; i < (int)m_pattern_data_set_list.size(); i++) {
            auto &pattern_set = m_pattern_data_set_list[i];
            int index = -1;
            for (int j = 0; j < pattern_set.pattern_data_list.size(); j++) {
                if (pattern_set.pattern_data_list[j].key == src_pattern_data_key) {
                    index = j;
                    break;
                }
            }

            if (index >= 0) {
                std::rotate(pattern_set.pattern_data_list.begin() + index,
                            pattern_set.pattern_data_list.begin() + index + 1,
                            pattern_set.pattern_data_list.end());
                return true;
            }
        }
        return false;
    }

  private:
    void ResetAllSetWeight() {
        for (auto &pattern_set : m_pattern_data_set_list) {
            pattern_set.cur_weight = pattern_set.src_weight;
        }
    }

    void MarkDataUsedInSet(PatternDataSet<T_Key, T_Data> &pattern_data_set,
                           const T_Key &target_pattern_data_key) {
        if (pattern_data_set.pattern_data_list.empty()) {
            assert(false);
            return;
        }

        for (auto &pattern_data : pattern_data_set.pattern_data_list) {
            if (pattern_data.key == target_pattern_data_key) {
                pattern_data.used = true;
                pattern_data.used_count++;
                return;
            }
        }

        assert(false);
        return;
    }

    void FindUnusedDataInSet(const PatternDataSet<T_Key, T_Data> &pattern_data_set, T_Key &out_key,
                             T_Data &out_pattern_data, std::string &out_set_name) {
        if (pattern_data_set.pattern_data_list.empty()) {
            assert(false);
            return;
        }

        for (const auto &pattern_data : pattern_data_set.pattern_data_list) {
            if (!pattern_data.used) {
                out_key = pattern_data.key;
                out_pattern_data = pattern_data.user_data;
                out_set_name = pattern_data_set.set_name;
                return;
            }
        }
        assert(false);
        return;
    }

    bool HaveUnusedDataInSet(const PatternDataSet<T_Key, T_Data> &pattern_data_set) {
        for (const auto &pattern_data : pattern_data_set.pattern_data_list) {
            if (!pattern_data.used)
                return true;
        }

        return false;
    }

    void ResetSetPatternDataToUnused(PatternDataSet<T_Key, T_Data> &pattern_data_set) {
        for (auto &pattern_data : pattern_data_set.pattern_data_list) {
            pattern_data.used = false;
        }
    }

    int FindSetHaveWeight(int current_index) {
        int temp_current_index = current_index;
        int pattern_set_size = (int)m_pattern_data_set_list.size();
        if (temp_current_index < 0 || temp_current_index >= pattern_set_size)
            temp_current_index = 0;

        for (int i = 0; i < pattern_set_size; i++) {
            if (temp_current_index < 0 || temp_current_index >= pattern_set_size) {
                temp_current_index = 0;
            }

            auto &find_have_weight_set = m_pattern_data_set_list[temp_current_index];
            if (find_have_weight_set.cur_weight > 0) {
                return temp_current_index;
            }

            temp_current_index++;
        }

        return -1;
    }

    std::vector<int> FindHighCurrentWeightSetPatternIndex() {
        std::vector<int> high_weight_pattern_index_list;
        int high_weight = -1;
        for (int i = 0; i < m_pattern_data_set_list.size(); i++) {
            int set_weight = m_pattern_data_set_list[i].cur_weight;
            if (set_weight <= 0)
                continue;

            if (-1 == high_weight) {
                high_weight = set_weight;
                high_weight_pattern_index_list.push_back(i);
                continue;
            }

            if (set_weight < high_weight)
                continue;

            if (set_weight == high_weight) {
                high_weight_pattern_index_list.push_back(i);
                continue;
            }

            if (set_weight > high_weight) {
                high_weight_pattern_index_list.clear();
                high_weight_pattern_index_list.push_back(i);
            }
        }

        return high_weight_pattern_index_list;
    }

    void MakeNextSetPatternIndex(int current_set_pattern_index) {
        //mode2 模式 优先选择未选择过的集合，以集合为优先
        if (SelectWeightSetMode::Mode2 == m_select_weight_set_mode) {
            current_set_pattern_index++;
            m_next_pattern_set_index = current_set_pattern_index;
            if (m_next_pattern_set_index >= m_pattern_data_set_list.size())
                m_next_pattern_set_index = 0;

            return;
        }

        std::vector<int> high_weight_pattern_index_list = FindHighCurrentWeightSetPatternIndex();

        //所有集合权重都为0了，直接重置
        if (high_weight_pattern_index_list.empty()) {
            m_next_pattern_set_index = 0;
            return;
        }

        //根据模式选择 下一个需要选择的权重集合
        int next_pattern_index = -1;
        if (SelectWeightSetMode::Mode0 == m_select_weight_set_mode) {
            int min_set_select_count = -1;
            for (auto weight_pattern_index : high_weight_pattern_index_list) {
                int set_select_count =
                    m_pattern_data_set_list[weight_pattern_index].set_select_count;
                if (min_set_select_count == -1 || set_select_count < min_set_select_count) {
                    min_set_select_count = set_select_count;
                    next_pattern_index = weight_pattern_index;
                }
            }
        } else if (SelectWeightSetMode::Mode1 == m_select_weight_set_mode) {
            int max_src_weight = -1;
            for (auto weight_pattern_index : high_weight_pattern_index_list) {
                int src_weight = m_pattern_data_set_list[weight_pattern_index].src_weight;
                if (max_src_weight == -1 || src_weight > max_src_weight) {
                    max_src_weight = src_weight;
                    next_pattern_index = weight_pattern_index;
                }
            }
        }

        if (next_pattern_index == -1) {
            assert(false);
            next_pattern_index = 0;
        }

        m_next_pattern_set_index = next_pattern_index;
    }

  private:
    SelectWeightSetMode m_select_weight_set_mode = SelectWeightSetMode::Mode0;
    int m_next_pattern_set_index = 0; //next group set index
    std::vector<PatternDataSet<T_Key, T_Data>> m_pattern_data_set_list;
};

} // namespace BASE
} // namespace ZEGO

#endif
