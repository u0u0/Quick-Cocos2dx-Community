
function __G__TRACKBACK__(errorMessage)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(errorMessage) .. "\n")
    print(debug.traceback("", 2))
    print("----------------------------------------")
end

package.path = package.path .. ";src/"
--添加资源目录
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():setPopupNotify(false)
require("app.MyApp").new():run()