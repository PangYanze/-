// 作者：BeMgCaSrBa
// 叠甲：本人没参加过异环内测，一切卡池消息均来自互联网。
//     尤其是很难找到完整的卡池规则展示。
//     如有错误，请指正。
//	    例如：A级保底机制不明。
// 参考：
// https://nga.178.com/read.php?tid=44462386&rand=231
// BV1P13JzCE6A
// BV1daffBrEJP
// ************************
//			需要C++20 ！
// ************************

#include <iostream>
#include "卡池.h"
#include <map>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <future>
#include <numeric>
struct 抽卡总结_t {
	std::map<int, int>出金抽数;
	std::map<int, int>出金净抽数;
	std::map<卡池::区域类型, int> 出金区域统计;
	int 捉到宝箱守护者 = 0;
	int 放走宝箱守护者 = 0;
	size_t 金数 = 0;
	double 平均抽数 = 0.0;
	double 平均净抽数 = 0.0;
};
// 辅助函数：用于单个线程执行抽卡任务
抽卡总结_t worker_task() {
	//const int per_thread_count = 100000;
	const int per_thread_count = 100;
	卡池 local_pool; // 每个线程拥有独立的卡池对象

	// 抽卡
	local_pool([&local_pool]()->bool {
		return (local_pool.已获得金数() < per_thread_count);
		}
	);
	抽卡总结_t result;
	std::map<int, int>& m = result.出金抽数;
	for (auto& p : local_pool.出金统计()) {
		result.平均抽数 += p;
		m[p]++;
	}
	result.金数 = local_pool.已获得金数();
	result.平均抽数 /= (1.0 * result.金数);

	for (auto& p : local_pool.净抽数统计()) {
		result.平均净抽数 += p;
		result.出金净抽数[static_cast<int>(ceil(p))]++;
	}
	result.平均净抽数 /= (1.0 * result.金数);


	result.捉到宝箱守护者 = local_pool.
		获取特殊事件计数器().捉到宝箱守护者;
	result.放走宝箱守护者 = local_pool.
		获取特殊事件计数器().放走宝箱守护者;
	result.出金区域统计 = local_pool.获取特殊事件计数器().出金区域统计;
	// 返回该线程的统计数据
	return result;
}

std::string 展示抽卡总结(const 抽卡总结_t& result) {
	std::string s;
	s += std::format("一共抽到了{}金\n", result.金数);
	s += std::format("平均抽数{:.4f}，平均净抽数{:.4f}\n", result.平均抽数, result.平均净抽数);
	s += std::format("捉到宝箱守护者：{}次\n放走宝箱守护者：{}次\n", result.捉到宝箱守护者, result.放走宝箱守护者);

	s.append("\n出金抽数统计\n");
	for (const auto& [p, q] : result.出金抽数) {
		s += std::format("{}：{}\n", p, q);
	}
	s.append("\n出金净抽数统计（向上取整）\n");
	for (const auto& [p, q] : result.出金净抽数) {
		s += std::format("{}：{}\n", p, q);
	}
	s.append("\n出金区域统计\n");
	for (const auto& [p, q] : result.出金区域统计) {
		s += std::format("{}：{}\n", 卡池::区域类型to_string(p), q);
	}
	return s;
}

int main() {
	// 一个测试的卡池对象，12.0代表每个A级角色返还12个失纬棋子（模拟后期）
	// 默认为6.0，表示即每个A级角色返还6个失纬棋子（模拟前期）
	卡池 测试{ 12.0 };
	std::cout << "抽卡开始\n";
	std::cout << 测试();		// 单抽，打印详情
	std::cout << 测试(10);	// 十连，打印详情
	std::cout << "\n***************\n"
		"获取S保底计数：" << 测试.获取S保底计数() << "\n***************\n";
	// 查看背包物品
	std::cout << "背包物品：\n" << 测试.获取背包().to_string() << "\n";

	测试([&测试]()->bool {		// 抽到返回false为止，这里是抽到2金
		return (测试.已获得金数() < 2);
		});
	std::cout << "2金后的背包物品：\n" << 测试.获取背包().to_string() << "\n***************\n" << "\n";

	// 多线程抽卡，并给出统计
	// 启动 10 个线程
	const int thread_count = 10;

	std::vector<std::future<抽卡总结_t>> futures;

	// 启动 10 个线程
	for (int i = 0; i < thread_count; ++i) {
		futures.push_back(std::async(std::launch::async, worker_task));
	}

	// 收集并合并结果
	抽卡总结_t global_stats;
	for (auto& f : futures) {
		抽卡总结_t partial_result = f.get(); // 等待线程结束并获取结果
		global_stats.捉到宝箱守护者 += partial_result.捉到宝箱守护者;
		global_stats.放走宝箱守护者 += partial_result.放走宝箱守护者;
		for (auto& [key, counts] : partial_result.出金抽数) {
			global_stats.出金抽数[key] += counts;
		}
		for (auto& [key, counts] : partial_result.出金净抽数) {
			global_stats.出金净抽数[key] += counts;
		}
		for (auto& [key, counts] : partial_result.出金区域统计) {
			global_stats.出金区域统计[key] += counts;
		}
		global_stats.平均净抽数 += partial_result.平均净抽数;
		global_stats.平均抽数 += partial_result.平均抽数;
		global_stats.金数 += partial_result.金数;

	}
	global_stats.平均净抽数 /= thread_count;
	global_stats.平均抽数 /= thread_count;

	std::cout << 展示抽卡总结(global_stats) << std::endl;

	return 0;
}


