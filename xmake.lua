target("opentools")
    set_languages("cxx11", "c11")
    add_rules("mode.debug", "mode.release")
    --  第三方库相关  --

    -- 添加painterengine库，这是一个C语言图形库
    add_includedirs("3rdparty/painterengine/core")
    add_files("3rdparty/painterengine/core/*.c")
    add_includedirs("3rdparty/painterengine/kernel")
    add_files("3rdparty/painterengine/kernel/*.c")
    add_includedirs("3rdparty/painterengine/runtime")
    add_files("3rdparty/painterengine/runtime/*.c")
    add_includedirs("3rdparty/painterengine/platform/modules")

    if is_plat("linux") then
        add_includedirs("3rdparty/painterengine/platform/linux")
        add_files("3rdparty/painterengine/platform/linux/*.c")
    elseif is_plat("windows") then
        add_includedirs("3rdparty/painterengine/platform/windows")
        add_files("3rdparty/painterengine/platform/windows/*.c")
    elseif is_plat("macosx") then
        add_includedirs("3rdparty/painterengine/platform/macos")
        add_files("3rdparty/painterengine/platform/macos/*.c")
    end

    -- 在Linux平台上启用GL、glut等库和POSIX
    if is_plat("linux") then 
        add_defines("_DEFAULT_SOURCE")
        add_defines("__USE_MISC")
        add_links("GL", "glut", "pthread", "hpdf")
        -- add_linkdirs("/usr/local/share/libharu")
        -- add_cxflags("-std=posix", {force = true})
    end
    add_includedirs("3rdparty/painterengine")

    -- 直接导入libarchive库
    add_links("archive")
    -- add_packages("haru")

    --  本系统实现代码  --

    add_files("src/*.c")
    add_includedirs("src")
    add_files("src/kernel/*.c")
    add_includedirs("src/kernel")
    add_files("src//widgets/*.c")
    add_includedirs("src/widgets")

    --  Release和Debug版本选择  --
    if is_mode("debug") then
        add_defines("__DEBUG_OTS__=1")
    end
