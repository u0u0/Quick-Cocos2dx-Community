# Quick-Cocos2dx-Community

Quick-Cocos2dx-Community 是由[泰然网](http://www.tairan.com)负责开发维护基于 Quick-Cocos2d-x 引擎的社区增强版。

Quick-Cocos2dx-Community 具有 Cocos2d-x 和 Quick-Cocos2d-x 的所有优点。并在 Quick-Cocos2d-x 基础上更新了最新的平台和第三方功能。让开发者可以用更低的成本、更高的开发效率、更省的人力成本创造高品质手游产品。

#### 我们认为：一切导致崩溃，损失流畅度，增加包体积 的行为，都是耍流氓

## 稳定版本下载地址

github上并未提供已编译好的Player，请在下面的地址下载稳定版本。

[http://www.tairan.com/engines-download](http://www.tairan.com/engines-download)


## Q&A
- 你们为什么要制作 Quick-Cocos2dx-Community ？

	>首先，由于官方疏于维护，并表示不在发布v3.5以后的版本。
	>其次，原作者因为公司战略调整和所属部门的变化，无法将精力投入到引擎上，导致引擎全部合并到c++仓库中。我们并不赞成Lua分支合并到Cocos2d-x主版本中，因为我们使用Lua作为主要开发语言，并不需要工程中包含js等部分。
	>再者，官方版本日益臃肿，从而造成包体积增加，性能降低，这是我们使用Cocos2d-x引擎所不愿意看到的。
	>以上原因促使我们聚在一起，共同维护本社区独立版本。

- Quick-Cocos2dx-Community 衍生于官方哪一个版本？

	>Quick-Cocos2dx-Community 发展自用户公认的最稳定版本 Quick-Cocos2d-x v3.3。
	>并且我们认为v3.3是最适合Lua语言开发的版本。
	>我们保留了v3.3的所有优势：
	1. 独立的引擎代码而非一个插件包，优秀的模块化编译支持。
	2. Quick独有的Player快速测试代码。
	3. 使用Luajit加速Lua代码执行，并提供配套打包工具。
	4. 完整的Quick framework并不断改善，而非3.5中的阉割版。

- 如何升级？

	>[从Quick 3.3 升级到 Quick-Cocos2dx-Community 3.6](http://www.tairan.com/archives/10539/)

- Quick-Cocos2dx-Community 未来发展思路是什么？（tbd）

## 优势
Quick-Cocos2dx-Community 具有 Cocos2d-x 以及 Quick-Cocos2d-x 的所有优点。让开发商可以用更低的成本、更高的开发效率、更省的人力成本创造高品质手游产品。

快速开发： 框架自带手游需要的各种基础服务，开发商能够将有限的资源投入到产品的玩法和用户体验上。

降低成本： 选择 Quick-Cocos2dx-Community，整个产品的绝大部分代码都可以使用 Lua 脚本语言完成开发。相比 C++ 能够显著降低人员成本，在开发效率上也具有明显优势。

优化用户体验： Quick-Cocos2dx-Community 提供了更容易使用 UI 库和脚本编译打包加密和自动更新等功能。让开发商可以用更低的成本快速改善产品的体验。

成熟可靠： 目前使用 Cocos2d-x 平台开发的热门手游产品中，一半以上都用到了 Lua 脚本语言。而 Quick-Cocos2dx-Community 作为 Cocos2d-x + Lua 的升级解决方案，保持了成熟可靠的特点。

- Quick-Cocos2dx-Community 官网 http://www.tairan.com
- 新浪微博 http://weibo.com/678837188
- 微信 iTyran
- QQ 群 190864188

## 快速上手只需3步
1. 下载 Quick-Cocos2dx-Community 最新版，并解压到一个路径。
	> 解压的路径以及文件夹名称**不要有中文或空格**.
2. Mac系统：在终端中执行：setup_mac.sh
	> 命令前*不需要*加入sudo。当提示输入密码的时候，请输入当前用户的登陆密码。

   Windows系统：双击setup_win.bat即可。
3. 启动 player
	> 运行步骤2以后，脚本会自动创建player的快捷方式。

	 Mac 系统在 Applications 文件夹(或系统应用列表)中 player.app 。
	 Windows 系统在桌面上的 player.exe 快捷方式。

## 相关文档

### 更新说明
- [Quick-Cocos2dx-Community 3.6.1 Beta1 更新说明](http://www.tairan.com/archives/10607/)
- [Quick-Cocos2dx-Community 3.6 Release 更新说明](http://www.tairan.com/archives/10573/)
- [Quick-Cocos2dx-Community 3.6 Beta2 更新说明](http://tairan.com/archives/10563/)
- [Quick-Cocos2dx-Community 3.6 Beta1 内部测试版](http://tairan.com/archives/10516/)

### 使用指南

- [Quick-Cocos2dx-Community 在发布时加密资源文件](http://tairan.com/archives/10562/)
- [Quick-Cocos2dx-Community 中 64 位 Luajit 使用说明](http://tairan.com/archives/10561/)
- [quick-community-dev插件安装](http://tairan.com/archives/10542/)
- [在发布时加密lua源文件](http://tairan.com/archives/10157/)
- [开发环境搭建](http://tairan.com/archives/7344/)

### 参考手册

- [quick 中的事件](./docs/manual/events/zh.html)
- [quick 中的触摸事件](./docs/manual/touch-events/zh.html)
- [API 文档](./docs/api/index.html)

## 相关教程

### 使用Quick初学者系列教程

- [Quick初学者游戏教程1](http://tairan.com/archives/10184/)
- [Quick初学者游戏教程2](http://tairan.com/archives/10212/)
- [Quick初学者游戏教程3](http://tairan.com/archives/10287/)
- [Quick初学者游戏教程4](http://tairan.com/archives/10290/)
- [Quick初学者游戏教程5](http://tairan.com/archives/10299/)
- [Quick初学者游戏教程6](http://tairan.com/archives/10306/)
- [Quick初学者游戏教程7](http://tairan.com/archives/10312/)
- [Quick初学者游戏教程8](http://tairan.com/archives/10323/)
- [Quick初学者游戏教程9](http://tairan.com/archives/10328/)
- [Quick初学者游戏教程10](http://tairan.com/archives/10334/)
- [Quick初学者游戏教程11](http://tairan.com/archives/10339/)
- [Quick初学者游戏教程12](http://tairan.com/archives/10341/)

## 官方书籍

- [Cocos2d-x 之Lua 核心编程](http://tairan.com/cocos2d-lua-book)
