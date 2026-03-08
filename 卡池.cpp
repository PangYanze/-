#include "卡池.h"
#include <format>
#include <unordered_map>
#include <stdexcept>
//#include <functional>
double 卡池::randomZeroToOne()
{
	return zero_one_dist(generator);
}

// 掷骰子，返回1-6的整数
int 卡池::rollDice() {
	return dice_dist(generator);
}

const 卡池::背包_t& 卡池::获取背包()const
{
	return this->背包;
}

std::string 卡池::概率提升()
{
	//常规, 常规岛1, 常规岛2, 概率提升, 概率提升岛1, 概率提升岛2
	switch (this->当前区域类型)
	{
	case 区域类型::常规:
		this->当前区域类型 = 区域类型::概率提升主区域;
		break;
	case 区域类型::常规岛1:
		this->当前区域类型 = 区域类型::概率提升岛1;
		break;
	case 区域类型::常规岛2:
		this->当前区域类型 = 区域类型::概率提升岛2;
		break;
	default:
		break;

	}
	return "概率提升，场地发生了变化\n";
}

void 卡池::概率恢复()
{
	switch (this->当前区域类型)
	{
	case 区域类型::概率提升主区域:
		this->当前区域类型 = 区域类型::常规;
		break;
	case 区域类型::概率提升岛1:
		this->当前区域类型 = 区域类型::常规岛1;
		break;
	case 区域类型::概率提升岛2:
		this->当前区域类型 = 区域类型::常规岛2;
		break;
	default:
		break;
	};
	return;
}

std::string 卡池::出A级角色()
{
	this->背包.A级角色数++;
	// 当第2~7次获得相同A级角色时，每次会变为获得1个济角色的心象碎片和6个失纬棋子
	// 这个在统计净抽数的时候进行了，不要重复
	return "获得A级角色\n";
}

std::string 卡池::出S级角色()
{
	int totalS = S保底计数;
	// 出金统计
	出金统计数组.push_back(totalS);
	// 出金区域统计
	特殊事件计数器.出金区域统计[this->当前区域类型]++;
	// 净抽数统计
	净抽数统计处理();

	// 重置出金次数
	S保底计数 = 0;

	this->背包.S级角色数++;
	// 概率提升恢复
	概率恢复();

	return std::format("恭喜你获得S级角色！！共抽取{}次\n", totalS);
}

void 卡池::净抽数统计处理()
{
	//auto 备份 = 上一金物品计数;

	double result = S保底计数 -
		(背包.质实骰子 - 上一金物品计数.质实骰子) -
		(背包.失纬棋子 - 上一金物品计数.失纬棋子) / 24.0 -
		(背包.A级角色数 - 上一金物品计数.A级角色数) / 四星平均失纬棋子;

	// 加入计数
	净抽数统计数组.push_back(static_cast<decltype(净抽数统计数组)::value_type>(result));

	// 刷新数据
	上一金物品计数.A级角色数 = 背包.A级角色数;
	上一金物品计数.失纬棋子 = 背包.失纬棋子;
	上一金物品计数.质实骰子 = 背包.质实骰子;
}

std::string 卡池::出A级弧盘()
{
	this->背包.A级弧盘数++;
	// 获得A级弧盘时，将额外获得4个失纬棋子。
	this->背包.失纬棋子 += 4;
	return "获得A级弧盘\n";
}

std::string 卡池::出B级弧盘()
{
	this->背包.B级弧盘数++;
	//获得B级弧盘时，将额外获得20个迷迭棋子。
	this->背包.迷迭棋子 += 20;
	return "获得B级弧盘\n";
}

std::string 卡池::出质实骰子(int n)
{
	this->背包.质实骰子 += n;
	return std::format("获得{}个质实骰子\n", n);
}

std::string 卡池::出失纬棋盒(int n)
{
	this->背包.失纬棋子 += n;
	return std::format("获得{}个失纬棋子\n", n);
}

std::string 卡池::出迷迭棋盒(int n)
{
	this->背包.迷迭棋子 += n;
	return std::format("获得{}个迷迭棋子\n", n);
}

std::string 卡池::走到学徒宝箱()
{
	std::string result{};
	double randnum = this->randomZeroToOne();
	if (randnum <= 0.0082) {
		result += this->出S级角色();
	}
	else {
		result += this->出B级弧盘();
	}
	return result;
}

std::string 卡池::走到勇者宝箱()
{
	std::string result{};
	double randnum = this->randomZeroToOne();
	if (randnum <= 0.016) {
		result += this->出S级角色();
	}
	else {
		result += this->出B级弧盘();
	}
	result += this->出失纬棋盒(2);
	return result;
}

std::string 卡池::走到角色A()
{
	return this->出A级角色();
}

std::string 卡池::走到角色S()
{
	return this->出S级角色();
}

std::string 卡池::走到再来一次()
{
	return this->出质实骰子(1);
}

std::string 卡池::走到多重惊喜()
{
	return this->出质实骰子(5);
}

std::string 卡池::走到失纬棋盒4()
{
	return this->出失纬棋盒(4);
}

std::string 卡池::走到失纬棋盒8()
{
	return this->出失纬棋盒(8);
}

std::string 卡池::走到迷迭棋盒30()
{
	return this->出迷迭棋盒(30);
}

std::string 卡池::走到迷迭棋盒50()
{
	return this->出迷迭棋盒(50);
}

std::string 卡池::走到其他()
{
	return std::string();
}

std::string 卡池::走到沉眠地()
{

	出A级弧盘();
	this->盒盒位置 = this->位置 + 9;
	if (this->盒盒位置 >= 卡池主区域大小) {
		this->盒盒位置 -= 卡池主区域大小;
	}
	this->盒盒倒计时 = 3;

	return std::format("获取A级弧盘。宝藏守护者已开启！位置：{}\n", this->盒盒位置);
}

const std::span<const 卡池::格子类型> 卡池::当前区域数组()const
{
	switch (this->当前区域类型) {
	case 区域类型::常规:
		return 常规卡池;
	case 区域类型::常规岛1:
		return 常规岛1卡池;
	case 区域类型::常规岛2:
		return 常规岛2卡池;
	case 区域类型::概率提升主区域:
		return 概率提升卡池;
	case 区域类型::概率提升岛1:
		return 概率提升岛1卡池;
	case 区域类型::概率提升岛2:
		return 概率提升岛2卡池;
	default:
#ifdef _DEBUG
		throw std::runtime_error("卡池类型错误");
#else
		return 常规卡池;
#endif // _DEBUG
	}
}

const std::span<const 卡池::格子类型> 卡池::预览区域数组(区域类型 预览类型) const
{
	switch (预览类型) {
	case 区域类型::常规:
		return 常规卡池;
	case 区域类型::常规岛1:
		return 常规岛1卡池;
	case 区域类型::常规岛2:
		return 常规岛2卡池;
	case 区域类型::概率提升主区域:
		return 概率提升卡池;
	case 区域类型::概率提升岛1:
		return 概率提升岛1卡池;
	case 区域类型::概率提升岛2:
		return 概率提升岛2卡池;
	default:
#ifdef _DEBUG
		throw std::runtime_error("卡池类型错误");
#else
		return 常规卡池;
#endif // _DEBUG


	}
}

int 卡池::区域id() const
{
	switch (this->当前区域类型) {
	case 区域类型::常规:
		return 0;
	case 区域类型::常规岛1:
		return 1;
	case 区域类型::常规岛2:
		return 2;
	case 区域类型::概率提升主区域:
		return 0;
	case 区域类型::概率提升岛1:
		return 1;
	case 区域类型::概率提升岛2:
		return 2;
	default:
		return -1;
	}
}

std::tuple<卡池::区域类型, size_t, std::string> 卡池::n点后区域和位置(size_t n) const
{
	std::string result{};
	卡池::区域类型 目标区域类型 = this->当前区域类型;
	size_t 目标位置 = this->位置;
	const auto nowData = this->当前区域数组();
	const auto* 当前区域数据 = nowData.data();
	auto 当前区域大小 = nowData.size();
	bool 主区域 = true;
	if (当前区域大小 <= this->卡池岛区域大小) {
		主区域 = false;
	}

	// 进岛特判
	if (主区域 && 当前区域数据[this->位置] == 小岛宝箱1) {			// 进岛特判-岛1	//this->位置 == 16
		目标区域类型 = 区域类型(this->当前区域类型 + 1);
		目标位置 = n - 1;		//下标从0开始
		result += "进入岛1\n";
	}
	else if (主区域 && 当前区域数据[this->位置] == 小岛宝箱2) {	// 进岛特判-岛2	//this->位置 == 43
		目标区域类型 = 区域类型(this->当前区域类型 + 2);
		目标位置 = n - 1;		//下标从0开始
		result += "进入岛2\n";
	}
	// 溢出特判（下一圈）
	else if (主区域 && (this->位置 + n >= 卡池主区域大小)) {
		目标位置 = n - (卡池主区域大小 - this->位置);
	}
	// 出岛特判
	else if (int 剩余步数 = static_cast<int>(n) - (static_cast<int>(卡池岛区域大小) - static_cast<int>(this->位置));
		!主区域 && 剩余步数 >= 0) {
		result += "回到主区域\n";
		if (this->区域id() == 1) {
			目标区域类型 = 区域类型(this->当前区域类型 - 1);
			目标位置 = 18ull + 剩余步数;		//1岛出口18
		}
		else {
			目标区域类型 = 区域类型(this->当前区域类型 - 2);
			目标位置 = 45ull + 剩余步数;		//2岛出口45
		}
	}
	else {
		目标位置 += n;
	}
	return { 目标区域类型,目标位置,std::move(result) };

}

std::string 卡池::GO(size_t n)
{
#ifdef _DEBUG
	if (n < 1 && n>6) {
		throw std::runtime_error("骰子点数错误");
		//throw std::runtime_error("骰子点数错误");
	}
#endif // _DEBUG
	//std::string result{};
	//const auto nowData = this->当前区域数组();
	//const auto* 当前区域数据 = nowData.data();
	//auto 当前区域大小 = nowData.size();
	//bool 主区域 = true;
	//if (当前区域大小 <= this->卡池岛区域大小) {
	//	主区域 = false;
	//}
	//// 进岛特判
	//if (主区域 && 当前区域数据[this->位置] == 小岛宝箱1) {// 进岛特判-岛1			//this->位置 == 16) {	
	//	this->当前区域类型 = 区域类型(this->当前区域类型 + 1);
	//	this->位置 = n - 1;
	//	result += "进入岛1\n";
	//}
	//else if (主区域 && 当前区域数据[this->位置] == 小岛宝箱2) {//this->位置 == 43) {// 进岛特判-岛2
	//	this->当前区域类型 = 区域类型(this->当前区域类型 + 2);
	//	this->位置 = n - 1;
	//	result += "进入岛2\n";
	//}
	//// 溢出特判（下一圈）
	//else if (主区域 && (this->位置 + n >= 卡池主区域大小)) {
	//	this->位置 = n - (卡池主区域大小 - this->位置);
	//}
	//// 出岛特判
	//else if (int 剩余步数 = static_cast<int>(n) - (static_cast<int>(卡池岛区域大小) - static_cast<int>(this->位置));
	//	!主区域 && 剩余步数 >= 0) {
	//	result += "回到主区域\n";
	//	if (this->区域id() == 1) {
	//		this->当前区域类型 = 区域类型(this->当前区域类型 - 1);
	//		this->位置 = 18ull + 剩余步数;		//1岛出口18
	//	}
	//	else {
	//		this->当前区域类型 = 区域类型(this->当前区域类型 - 2);
	//		this->位置 = 45ull + 剩余步数;		//2岛出口45
	//	}
	//}
	//else {
	//	this->位置 += n;
	//}
	auto&& [t, p, result] = n点后区域和位置(n);
	this->当前区域类型 = t;
	this->位置 = p;
	return result;
}

size_t 卡池::主区域距离(size_t nowP, size_t destP)
{
	// 盒盒在下一圈
	if (destP < nowP)
	{
		return 卡池主区域大小 - nowP + destP;
	}
	return destP - nowP;
}

inline size_t 卡池::主区域距离()
{
	return 主区域距离(this->位置, this->盒盒位置);
}

std::string 卡池::奇迹盒盒处理(int 步数)
{
	if (this->盒盒倒计时 <= 0)
	{		// 不存在
		return {};
	}
	--盒盒倒计时;
	if (this->主区域距离() <= 步数) { //获取奖励
		this->背包.失纬棋子 += 30;
		this->盒盒倒计时 = 0;
		// 统计
		特殊事件计数器.捉到宝箱守护者++;
		return "成功追上宝藏守护者，获得失纬棋子30\n";  //1.5抽
	}
	if (盒盒倒计时 == 0) //宝藏守护者消失
	{
		// 统计
		特殊事件计数器.放走宝箱守护者++;
		return "很遗憾，宝藏守护者逃走了\n";
	}
	//继续追
	this->盒盒位置 += 2;
	return std::format("宝藏守护者当前位置：{}\n", this->盒盒位置);

}
const std::vector<float>& 卡池::净抽数统计() const
{
	return 净抽数统计数组;
}
//学徒宝箱, 勇者宝箱, 角色A, 角色S, 再来一次, 多重惊喜,
//失纬棋盒4, 失纬棋盒8, 迷迭棋盒30, 迷迭棋盒50,
//沉眠地宝箱1, 沉眠地宝箱2, 其他, 奇迹盒盒,
std::string 卡池::格子类型to_string(const 格子类型 _Gt)
{
	static const std::unordered_map<格子类型, std::string> 格子类型映射 = {
		{格子类型::学徒宝箱, "学徒宝箱"},
		{格子类型::勇者宝箱, "勇者宝箱"},
		{格子类型::角色A, "角色A级"},
		{格子类型::角色S, "角色S级"},
		{格子类型::再来一次, "再来一次"},
		{格子类型::多重惊喜, "多重惊喜"},
		{格子类型::失纬棋盒4, "失纬棋盒*4"},
		{格子类型::失纬棋盒8, "失纬棋盒*8"},
		{格子类型::迷迭棋盒30, "迷迭棋盒*30"},
		{格子类型::迷迭棋盒50, "迷迭棋盒*50"},
		{格子类型::小岛宝箱1, "沉眠地宝箱"},
		{格子类型::小岛宝箱2, "沉眠地宝箱"},
		{格子类型::其他, "其他奖励"},
		{格子类型::奇迹盒盒, "奇迹盒盒"}
	};

	auto it = 格子类型映射.find(_Gt);
	return it != 格子类型映射.end() ? it->second : "未知格子类型";
}

std::string 卡池::区域类型to_string(const 卡池::区域类型 _Gt)
{
	//常规 = 0, 常规岛1, 常规岛2, 概率提升主区域, 概率提升岛1, 概率提升岛2
	static const std::unordered_map<区域类型, std::string> 区域类型映射 = {
		{区域类型::常规, "常规主区域"},
		{区域类型::常规岛1, "常规岛1"},
		{区域类型::常规岛2, "常规岛2"},
		{区域类型::概率提升主区域, "概率提升主区域"},
		{区域类型::概率提升岛1, "概率提升岛1"},
		{区域类型::概率提升岛2, "概率提升岛2"},
	};
	auto it = 区域类型映射.find(_Gt);
	return it != 区域类型映射.end() ? it->second : "未知区域类型";
}

卡池& 卡池::回到起点()
{
	位置 = 0;
	盒盒位置 = 0;
	盒盒倒计时 = 0;
	//常规 = 0, 常规岛1, 常规岛2, 概率提升主区域, 概率提升岛1, 概率提升岛2
	switch (当前区域类型) {
	case 区域类型::常规岛1:
		[[fallthrough]];
	case 区域类型::常规岛2:
		当前区域类型 = 区域类型::常规;
		break;
	case 区域类型::概率提升岛1:
		[[fallthrough]];
	case 区域类型::概率提升岛2:
		当前区域类型 = 区域类型::概率提升主区域;
		break;
	default:
		break;
	}
	return *this;
}

卡池& 卡池::清空S计数()
{
	S保底计数 = 0;
	概率恢复();
	return *this;
}

卡池& 卡池::清空A计数()
{
	A保底计数 = 0;
	return *this;
}

卡池& 卡池::清空背包()
{
	背包.S级角色数 = 0;
	背包.A级角色数 = 0;
	背包.A级弧盘数 = 0;
	背包.B级弧盘数 = 0;
	背包.质实骰子 = 0;
	背包.失纬棋子 = 0;
	背包.迷迭棋子 = 0;
	上一金物品计数.A级角色数 = 0;
	上一金物品计数.失纬棋子 = 0;
	上一金物品计数.质实骰子 = 0;
	return *this;
}

卡池& 卡池::清空统计()
{
	出金统计数组.clear();
	净抽数统计数组.clear();
	特殊事件计数器.捉到宝箱守护者 = 0;
	特殊事件计数器.放走宝箱守护者 = 0;
	for (auto& [p, q] : 特殊事件计数器.出金区域统计) {
		q = 0;
	}

	return *this;
}

卡池& 卡池::clear()
{
	回到起点();
	清空S计数();
	清空A计数();
	清空背包();
	清空统计();
	return *this;
}

std::string 卡池::格子类型to_string()
{
	return 格子类型to_string(this->当前区域数组()[this->位置]);
}

std::string 卡池::operator()()
{
	// 掷骰子决定走几步
	return 卡池::抽卡核心(this->rollDice());
}

std::string 卡池::operator()(size_t N)
{
	std::string result;
	for (auto i = 0ull; i < N; ++i)
	{
		result += this->operator()();
	}
	return result;
}

std::string 卡池::抽卡()
{
	return this->operator()();
}

std::string 卡池::抽卡核心(int steps)
{
	++S保底计数;
	std::string result = std::format("保底内第{}抽\n", this->S保底计数);
	if (this->S保底计数 == S保底计数上限) {	//90 抽
		result += this->出S级角色();
		result += "居然是罕见的超级硬保底！“恭喜”你获得了“非酋”的称号！";
	}
	else {

		// 掷骰子决定走几步
		//int steps = this->rollDice();
		result += 奇迹盒盒处理(steps);
		result += GO(steps);


		std::string positionT{};
		if (this->区域id() == 0) {
			positionT = std::format("主区域");
		}
		else {
			positionT = std::format("岛{}", this->区域id());
		}
		result += std::format("当前区域：{}的第{}号\n", positionT, this->位置);
		result.append("当前区域类型：").append(格子类型to_string()).append("\n");

		// 执行
		auto 当前格子类型 = this->当前区域数组()[this->位置];
		switch (当前格子类型)
		{
			//学徒宝箱, 勇者宝箱, 角色A, 角色S, 再来一次, 多重惊喜,
			//	失纬棋盒4, 失纬棋盒8, 迷迭棋盒30, 迷迭棋盒50,
			//	小岛宝箱1, 小岛宝箱2, 其他, 奇迹盒盒,
		case 格子类型::学徒宝箱:
			result += 走到学徒宝箱(); break;
		case 格子类型::勇者宝箱:
			result += 走到勇者宝箱(); break;
		case 格子类型::角色A:
			result += 走到角色A(); break;
		case 格子类型::角色S:
			result += 走到角色S(); break;
		case 格子类型::再来一次:
			result += 走到再来一次(); break;
		case 格子类型::多重惊喜:
			result += 走到多重惊喜(); break;
		case 格子类型::失纬棋盒4:
			result += 走到失纬棋盒4(); break;
		case 格子类型::失纬棋盒8:
			result += 走到失纬棋盒8(); break;
		case 格子类型::迷迭棋盒30:
			result += 走到迷迭棋盒30(); break;
		case 格子类型::迷迭棋盒50:
			result += 走到迷迭棋盒50(); break;
		case 格子类型::小岛宝箱1:
			result += 走到学徒宝箱(); break;
		case 格子类型::小岛宝箱2:
			result += 走到学徒宝箱(); break;
		case 格子类型::其他:
			result += 走到其他(); break;
		case 格子类型::奇迹盒盒:
			result += 走到沉眠地(); break;
		default:
			break;
		}
		if (this->S保底计数 == S概率提升点) {	//70
			result += this->概率提升();
		}
	}

	// 集点赠礼
	// 每10次掷骰必定额外获得1次'集点赠礼'，其中A级角色的概率为20%，A级弧盘的概率为80%。
	++A保底计数;
	if (A保底计数 == 10) {
		A保底计数 = 0;
		if (this->randomZeroToOne() <= 0.2) {
			result += "A级计数器保底,获得A级角色\n";
			背包.A级角色数++;
		}
		else {
			result += "A级计数器保底,获得A级弧盘\n";
			背包.A级弧盘数++;
		}

	}

	return result;
}

std::string 卡池::预览(size_t n)const
{
	std::string result{ std::format("前方第{}个", n) };
	//const auto nowData = this->当前区域数组();
	//const auto* 当前区域数据 = nowData.data();
	//auto 当前区域大小 = nowData.size();
	//bool 主区域 = true;
	//if (当前区域大小 <= this->卡池岛区域大小) {
	//	主区域 = false;
	//}
	//auto 预览当前区域类型 = this->当前区域类型;
	//auto 预览位置 = this->位置;
	//// 进岛特判
	//if (主区域 && 当前区域数据[this->位置] == 小岛宝箱1) {// 进岛特判-岛1			//this->位置 == 16) {	
	//	预览当前区域类型 = 区域类型(this->当前区域类型 + 1);
	//	预览位置 = n - 1;
	//	result += "进入岛1\n";
	//}
	//else if (主区域 && 当前区域数据[this->位置] == 小岛宝箱2) {//this->位置 == 43) {// 进岛特判-岛2
	//	预览当前区域类型 = 区域类型(this->当前区域类型 + 2);
	//	预览位置 = n - 1;
	//	result += "进入岛2\n";
	//}
	//// 溢出特判（下一圈）
	//else if (主区域 && (this->位置 + n >= 卡池主区域大小)) {
	//	预览位置 = n - (卡池主区域大小 - this->位置);
	//}
	//// 出岛特判
	////else if (int 剩余步数 = static_cast<int>(n) - (static_cast<int>(卡池岛区域大小) - static_cast<int>(this->位置));
	////	!主区域 && 剩余步数 >= 0)
	//else if (int 剩余步数 = static_cast<int>(n) - (static_cast<int>(卡池岛区域大小) - static_cast<int>(this->位置));
	//	!主区域 && 剩余步数 >= 0) {
	//	result += "回到主区域\n";
	//	if (this->区域id() == 1) {
	//		预览当前区域类型 = 区域类型(this->当前区域类型 - 1);
	//		预览位置 = 18 + 剩余步数;		//1岛出口18
	//	}
	//	else {
	//		预览当前区域类型 = 区域类型(this->当前区域类型 - 2);
	//		预览位置 = 45 + 剩余步数;		//2岛出口45
	//	}
	//}
	//else {
	//	预览位置 += n;
	//}
	auto&& [预览当前区域类型, 预览位置, s] = n点后区域和位置(n);

	return result.append(s).append("格子类型：").
		append(格子类型to_string(预览区域数组(预览当前区域类型)[预览位置])).
		append("\n");
}

卡池::卡池(double 平均A角色失纬棋子_) :四星平均失纬棋子(平均A角色失纬棋子_)
{
}

size_t 卡池::已获得金数() const
{
	return 出金统计数组.size();
}

int 卡池::获取S保底计数() const
{
	return this->S保底计数;
}

size_t 卡池::获取位置ID() const
{
	auto result = this->位置;
	result += static_cast<size_t>(当前区域类型) * 100ull;
	return result;
}

bool 卡池::上一抽出金() const
{
	if (S保底计数 != 0)
	{
		return false;
	}
	if (出金统计数组.empty())
	{
		return false;
	}
	return true;
}

const std::vector<int>& 卡池::出金统计() const
{
	return this->出金统计数组;
}

const 卡池::特殊事件计数_t& 卡池::获取特殊事件计数器() const
{
	return this->特殊事件计数器;
}

std::string 卡池::抽卡指定步数且预览(int steps)
{
	auto result = 抽卡核心(steps);
	if (S保底计数 == S保底计数上限 - 1) {
		result += std::format("马上{}抽硬保底了，下一步不会走了！\n", S保底计数上限);
	}
	for (int i = 1; i <= 6; ++i) {
		result += 预览(i);
	}
	return result;
}

std::string 卡池::背包_t::to_string() const
{
	std::string result{};
	//int S级角色数 = 0;
	//int A级角色数 = 0;
	//int A级弧盘数 = 0;
	//int B级弧盘数 = 0;
	//int 质实骰子 = 0;
	//int 失纬棋子 = 0;
	//int 迷迭棋子 = 0;
	result += std::format("S级角色数：{}\n", S级角色数);
	result += std::format("A级角色数：{}\n", A级角色数);
	result += std::format("A级弧盘数：{}\n", A级弧盘数);
	result += std::format("B级弧盘数：{}\n", B级弧盘数);
	result += std::format("坚实骰子：{}\n", 质实骰子);
	result += std::format("失纬棋子：{}\n", 失纬棋子);
	result += std::format("迷迭棋子：{}\n", 迷迭棋子);
	result += "注：没有计算A级角色返还的失纬棋子\n";
	return result;
}
