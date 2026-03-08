#pragma once
#include <random>
#include <span>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <concepts>

template<typename T, typename...Args>
concept fun2bool = requires(T fun, Args...args) {
	{ fun(args...) } -> std::convertible_to<bool>;
};

// 异环up卡池
class 卡池
{
public:
	struct 背包_t {
		int S级角色数 = 0;
		int A级角色数 = 0;
		int A级弧盘数 = 0;
		int B级弧盘数 = 0;
		int 质实骰子 = 0;
		int 失纬棋子 = 0;
		int 迷迭棋子 = 0;
		std::string to_string()const;
	};
	struct 上一金重要物品计数_t {	// 用于统计净抽数
		int A级角色数 = 0;
		int 质实骰子 = 0;
		int 失纬棋子 = 0;		//24换1抽
	};
	// 注意不要改区域类型枚举对应的数！！
	enum 区域类型
	{
		常规 = 0, 常规岛1, 常规岛2, 概率提升主区域, 概率提升岛1, 概率提升岛2
	};
	enum 格子类型
	{
		学徒宝箱, 勇者宝箱, 角色A, 角色S, 再来一次, 多重惊喜,
		失纬棋盒4, 失纬棋盒8, 迷迭棋盒30, 迷迭棋盒50,
		小岛宝箱1, 小岛宝箱2, 其他, 奇迹盒盒,
	};
	struct 特殊事件计数_t {
		int 捉到宝箱守护者 = 0;
		int 放走宝箱守护者 = 0;
		std::map<区域类型, int> 出金区域统计 = {
			{常规,0}, {常规岛1,0}, {常规岛2,0},
			{概率提升主区域,0}, {概率提升岛1,0}, {概率提升岛2,0},
		};
	};
	static constexpr 格子类型 常规卡池[54] = { 勇者宝箱,
		角色A,学徒宝箱,其他,学徒宝箱,学徒宝箱,勇者宝箱,							// 1
		学徒宝箱,学徒宝箱,迷迭棋盒30,角色A,学徒宝箱,勇者宝箱,				// 2
		学徒宝箱,学徒宝箱,迷迭棋盒50,小岛宝箱1,角色A,勇者宝箱,			// 3 (16,18)
		学徒宝箱,学徒宝箱,迷迭棋盒30,奇迹盒盒,学徒宝箱,勇者宝箱,			// 4
		学徒宝箱,学徒宝箱,迷迭棋盒50,学徒宝箱,角色A,勇者宝箱,				// 5
		学徒宝箱,学徒宝箱,迷迭棋盒30,学徒宝箱,学徒宝箱,勇者宝箱,			// 6
		角色A,学徒宝箱,迷迭棋盒30,学徒宝箱,迷迭棋盒50,勇者宝箱,			// 7
		小岛宝箱2,角色A,迷迭棋盒30,学徒宝箱,学徒宝箱,勇者宝箱,			// 8 (43,45)
		再来一次,学徒宝箱,奇迹盒盒,迷迭棋盒30,学徒宝箱,							// 9
	};
	static constexpr 格子类型 常规岛1卡池[9] = {
		角色S,其他,勇者宝箱,学徒宝箱,勇者宝箱,学徒宝箱,勇者宝箱,学徒宝箱,勇者宝箱,
	};
	static constexpr 格子类型 常规岛2卡池[9] = {
		勇者宝箱,其他,多重惊喜,勇者宝箱,勇者宝箱,勇者宝箱,勇者宝箱,勇者宝箱,勇者宝箱,
	};
	static constexpr 格子类型 概率提升卡池[54] = { 角色S,
	角色A,勇者宝箱,其他,学徒宝箱,学徒宝箱,角色S,							// 1
	学徒宝箱,学徒宝箱,迷迭棋盒30,角色A,学徒宝箱,角色S,				// 2
	学徒宝箱,勇者宝箱,迷迭棋盒50,小岛宝箱1,角色A,角色S,			// 3 (16,18)
	学徒宝箱,学徒宝箱,迷迭棋盒30,奇迹盒盒,学徒宝箱,角色S,			// 4
	学徒宝箱,勇者宝箱,迷迭棋盒50,学徒宝箱,角色A,角色S,				// 5
	学徒宝箱,学徒宝箱,迷迭棋盒30,学徒宝箱,学徒宝箱,角色S,			// 6

	角色A,勇者宝箱,迷迭棋盒30,学徒宝箱,迷迭棋盒50,角色S,			// 7
	小岛宝箱2,角色A,迷迭棋盒30,学徒宝箱,学徒宝箱,角色S,			// 8 (43,45)
	再来一次,勇者宝箱,奇迹盒盒,迷迭棋盒30,学徒宝箱,						// 9
	};
	static constexpr 格子类型 概率提升岛1卡池[9] = {
		角色S,学徒宝箱,角色S,学徒宝箱,角色S,学徒宝箱,角色S,学徒宝箱,角色S,
	};
	static constexpr 格子类型 概率提升岛2卡池[9] = {
		角色S,勇者宝箱,多重惊喜,勇者宝箱,角色S,勇者宝箱,角色S,勇者宝箱,角色S,
	};
	static constexpr size_t 卡池主区域大小 = 54;
	static constexpr size_t 卡池岛区域大小 = 9;
	// 当第2~7次获得相同A级角色时，每次会变为获得1个济角色的心象碎片和6个失纬棋子；
	// 第8次起，每次变为获得12个失纬棋子。
	static constexpr int S概率提升点 = 70;
	static constexpr int S保底计数上限 = 90;

public:
	// 构造函数
	// 输入：平均每个A级角色获得失纬棋子的数量。默认为6.0。
	// 不应小于0或大于12。超出会回到边界。
	// 当第2~7次获得相同A级角色时，每次会变为获得1个济角色的心象碎片和6个失纬棋子；
	// 第8次起，每次变为获得12个失纬棋子。
    卡池(double 平均A角色失纬棋子_);
	卡池() = default;

	// 信息查询
	size_t 已获得金数()const;
	const 背包_t& 获取背包()const;
	std::string 预览(size_t n)const;	// 预览后续格子！
	int 获取S保底计数()const;
	// 获取位置，0-53为主区域，100-108为岛1区域，200-208为岛2区域；
	// 概率提升会额外+300
	size_t 获取位置ID()const;
	// 如果使用了后门模式，可能会判断错误
	bool 上一抽出金()const;

	// 抽卡
	std::string operator()();	// 进行一次抽卡
	inline std::string 抽卡();	// 进行一次抽卡，不过更建议使用operator()
	std::string operator()(size_t N);	// 进行n次抽卡
	// 已知抽卡直到返回false，可搭配lammbda使用
	template<fun2bool T, typename...Args>
	std::string operator()(T fun, Args...args);

	// 抽卡统计
	const std::vector<int>& 出金统计()const;	
	const 特殊事件计数_t& 获取特殊事件计数器()const;
	const std::vector<float>& 净抽数统计()const;		// 抽到的失纬棋子等折合抽数减掉
	static std::string 格子类型to_string(const 格子类型 _Gt);
	static std::string 区域类型to_string(const 区域类型 _Gt);

	//	后门模式
	std::string 抽卡指定步数且预览(int steps);
	卡池& 回到起点();
	卡池& 清空S计数();
	卡池& 清空A计数();
	卡池& 清空背包();
	卡池& 清空统计();
	卡池& clear();		// 	回到起点，清空S计数，清空A计数，清空背包，清空统计

	// debug和内部使用，可以调整为private
	double randomZeroToOne();	// 生成[0,1)之间的随机浮点数
	int rollDice();	// 掷骰子，返回1-6的整数

private:
	// 抽卡状态存储
	背包_t 背包;
	int S保底计数 = 0;
	int A保底计数 = 0;
	区域类型 当前区域类型 = 区域类型::常规;
	size_t 位置 = 0;
	size_t 盒盒位置 = 0;
	int 盒盒倒计时 = 0;

	// 用于统计
	上一金重要物品计数_t 上一金物品计数;	// 用于统计净抽数
	std::vector<int> 出金统计数组;
	std::vector<float> 净抽数统计数组;
	特殊事件计数_t 特殊事件计数器;

	//特殊设置
	double 四星平均失纬棋子 = 6.0;

	// 随机数生成
	std::mt19937 generator{ std::random_device{}() };  // Mersenne Twister随机数引擎
	std::uniform_real_distribution<double> zero_one_dist{ 0.0, 1.0 };  // 0-1均匀分布
	std::uniform_int_distribution<int> dice_dist{ 1, 6 };  // 骰子分布(1-6)

private:
	std::string 概率提升();
	void 概率恢复();
	// 一定要在刷新出金数据前使用
	void 净抽数统计处理();
	std::string 出S级角色();
	std::string 出A级角色();
	std::string 出A级弧盘();
	std::string 出B级弧盘();
	std::string 出质实骰子(int n);
	std::string 出失纬棋盒(int n);
	std::string 出迷迭棋盒(int n);
	std::string 走到学徒宝箱();
	std::string 走到勇者宝箱();
	std::string 走到角色A();
	std::string 走到角色S();
	std::string 走到再来一次();
	std::string 走到多重惊喜();
	std::string 走到失纬棋盒4();
	std::string 走到失纬棋盒8();
	std::string 走到迷迭棋盒30();
	std::string 走到迷迭棋盒50();
	std::string 走到其他();
	std::string 走到沉眠地();

	// 获取获取当前区域数组
	const std::span<const 格子类型> 当前区域数组()const;

	// 获取获取任意区域数组
	const std::span<const 格子类型> 预览区域数组(区域类型 预览类型)const;

	// 区域判定(不考虑概率提升)
	int 区域id()const;

	// 注意对区域类型枚举敏感 ！！！！！
	std::tuple<区域类型, size_t, std::string> n点后区域和位置(size_t n)const;
	// 走到目标位置！
	std::string GO(size_t n);
	std::string 抽卡核心(int steps);

	// 用于辅助计算盒盒距离
	size_t 主区域距离(size_t nowP, size_t destP);
	inline size_t 主区域距离();
	// 在掷骰子后，走之前处理奇迹盒盒
	std::string 奇迹盒盒处理(int 步数);

	std::string 格子类型to_string();
};

template<fun2bool T, typename ...Args>
inline std::string 卡池::operator()(T fun, Args ...args)
{
	std::string result;
	while (fun(args...)) {
		result+=this->operator()();
	}

	return result;
}
