# netproxy
将业务层和通信层分开，采用通信代理的方式，业务层只需要向通信代理注册通信请求即可，
由通信代理统一对所有的通信进行数据交互，数据投递，心跳维护，重连等，当新增业务时只需要进行注册通信请求即可，
焦点放在数据处理上。

功能介绍
1.服务端和客户端两种代理模式
2.TCP和UDP两种代理协议（当前只支持TCP）
3.对频繁交互数据的业务进行动态线程切换，避免当前业务影响到其它业务
4.心跳发包，心跳检测和连接维护
5.对每个通信对象进行的数据统计，速率统计，发送量等信息维护
6.同步和异步数据发送方式
7.代理层同时支持多个代理，支持客户端模式，服务端模式混合模式，TCP和UDP等
8.对外屏蔽套接字，提供一个内部维护的唯一数，当进行重连后外部感知不到，不会影响到业务数据的正常交互

性能分析
1.采用IO复用模型，（当前为EPOLL模型，支持扩展），提高线程使用率
2.预分配缓冲区和循环队列，避免频繁内存分配和释放
3.对频繁交互的数据动态移动到单线程接收，避免IO复用模型频繁触发，影响到其它业务数据交互
4.采用单独的发送线程模块，和IO接收模型分离
在并发上没有进行测试，尽可能的减少了在网络模块的延时，所以预测不会太低